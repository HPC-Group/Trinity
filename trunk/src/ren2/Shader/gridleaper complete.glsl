#version 420 core

// get ray start points & color
layout(binding=0) uniform sampler2D rayStartPoint;
layout(binding=1) uniform sampler2D rayStartColor;
// 2 is transfer function (inside render method shader)
// 3 is volume pool metadata (inside volume pool)
// 4 is volume pool (inside volume pool)
// 5 is hastable (inside hash table)

#ifdef DEBUG
layout(binding=6) uniform sampler2D debugColor;
// 7 is hastable for working set tracking (inside hash table)
#endif

// get pixel coordinates
layout(pixel_center_integer) in vec4 gl_FragCoord;

// volume and view parameter
uniform float sampleRateModifier;
uniform mat4x4 mEyeToModel;

// import VolumePool functions
bool GetBrick(in vec3 normEntryCoords, inout uint iLOD, in vec3 direction,
              out vec3 poolEntryCoords, out vec3 poolExitCoords,
              out vec3 normExitCoords, out bool bEmpty,
              out vec3 normToPoolScale, out vec3 normToPoolTrans, out uvec4 brickCoords);
vec3 TransformToPoolSpace(in vec3 direction,
                          in float sampleRateModifier);
vec3 GetSampleDelta();
uint ComputeLOD(float dist);

// import Compositing functions
vec4 UnderCompositing(vec4 src, vec4 dst);

// helper functions -> may be better of in an external file
void OpacityCorrectColor(float opacity_correction, inout vec4 color) {
  color.a = 1.0 - pow(1.0 - color.a, opacity_correction);
}

// data from vertex stage
in vec3 vPosInViewCoords;

// outputs into render targets
layout(location=0) out vec4 accRayColor;
layout(location=1) out vec4 rayResumeColor;
layout(location=2) out vec4 rayResumePos;

#ifdef DEBUG
layout(location=3) out vec4 debugFBO;
#endif

// layout (depth_greater) out float gl_FragDepth;

void TerminateRay(bool bOptimalResolution) {
  if (bOptimalResolution) {
    rayResumePos.w = 1000;
    rayResumeColor = accRayColor;
  }
}

vec4 ComputeColorFromVolume(vec3 currentPoolCoords, vec3 modelSpacePosition, vec3 sampleDelta);

// go
void main()
{
  // read ray parameters and start color
  ivec2 screenpos = ivec2(gl_FragCoord.xy);
  accRayColor     = texelFetch(rayStartColor, screenpos,0);
  rayResumeColor  = accRayColor;

#ifdef DEBUG
  debugFBO        = texelFetch(debugColor, screenpos,0);
  debugFBO.w      = 1;
#endif
    
  // fetch ray entry from texture and get ray exit point from vs-shader
  rayResumePos = texelFetch(rayStartPoint, screenpos,0);

  // if the ray has terminated already, exit early
  if (rayResumePos.w == 1000) return;

  vec4 exitParam = vec4((mEyeToModel * vec4(vPosInViewCoords.x, vPosInViewCoords.y, vPosInViewCoords.z, 1.0)).xyz, vPosInViewCoords.z);

  // for clarity, separate postions from depth (stored in w)
  vec3 normEntryPos = rayResumePos.xyz;
  float entryDepth = rayResumePos.w;
  vec3 normExitCoords = exitParam.xyz;
  float exitDepth = exitParam.w;

  // compute ray direction
  vec3 direction = normExitCoords-normEntryPos;
  float rayLength = length(direction);
  vec3 voxelSpaceDirection = TransformToPoolSpace(direction, sampleRateModifier);
  vec3 sampleDelta = GetSampleDelta();
  float stepSize = length(voxelSpaceDirection);

  // iterate over the bricks along the ray
  float t = 0;
  bool bOptimalResolution = true;

  float voxelSize = 0.125/2000.; // TODO make this a quater of one voxel (or smaller)
  vec3 currentPos = normEntryPos;

  // fetch brick coords at the current position with the 
  // correct resolution. if that brick is not present the
  // call returns false and the coords to a lower resolution brick
  vec3 poolEntryCoords;
  vec3 poolExitCoords;
  vec3 normBrickExitCoords;
  bool bEmpty;
  uvec4 brickCoords;
  uvec4 lastBrickCoords = uvec4(0,0,0,9999);

  if (rayLength > voxelSize) {
    for(uint j = 0;j<100;++j) { // j is just for savety, stop traversal after 100 bricks
      // compute the current LoD
      float currentDepth = mix(entryDepth, exitDepth, t);

      uint iLOD = ComputeLOD(currentDepth);

      vec3 normToPoolScale;
      vec3 normToPoolTrans;
      bool bRequestOK = GetBrick(currentPos,
                                 iLOD, direction,
                                 poolEntryCoords, poolExitCoords,
                                 normBrickExitCoords, bEmpty,
                                 normToPoolScale, normToPoolTrans, brickCoords);

      if (!bRequestOK && bOptimalResolution) {
        // for the first time in this pass, we got a brick
        // at lower than requested resolution then record this 
        // position
        bOptimalResolution = false;
        rayResumePos   = vec4(currentPos, currentDepth);
        rayResumeColor = accRayColor;
      }

      if (!bEmpty && lastBrickCoords != brickCoords) {
        // compute the number of steps it takes to

        // leave the brick
        int iSteps = int(ceil(length(poolExitCoords-poolEntryCoords)/stepSize));
        
        // or leave the entire voluem (min = whatever comes first)
        iSteps = min(iSteps, int(ceil(length((normExitCoords-currentPos)*normToPoolScale)/stepSize)));

        // compute opacity correction factor
        //float ocFactor = exp2(float(iLOD)) / sampleRateModifier;
        float ocFactor = 1.0 / sampleRateModifier; // use the nicer "opacity correction" for now

        // now raycast through this brick
        vec3 currentPoolCoords = poolEntryCoords;
        for (int i=0;i<iSteps;++i) {

          // execute sampling function
          vec4 color = ComputeColorFromVolume(currentPoolCoords, (currentPoolCoords-normToPoolTrans)/normToPoolScale, sampleDelta);

#ifdef COLOR_LODS
          switch (iLOD % 3) {
            case 0:  
              color.g = color.b = 0;
              break;
            case 1:
              color.r = color.b = 0;
              break;
            default:
              color.r = color.g = 0;
              break;
          }
#endif

          // apply oppacity correction
          OpacityCorrectColor(ocFactor, color);

          // compositing
          accRayColor = UnderCompositing(color, accRayColor);

          // early ray termination
          if (accRayColor.a > 0.99) {
#ifdef DEBUG            
            if (bOptimalResolution) {
              debugFBO.b = 1;
              debugFBO.w = 1;
            }
#endif            
            TerminateRay(bOptimalResolution);
            return;
          }

          // advance ray
          currentPoolCoords += voxelSpaceDirection;

#ifdef DEBUG
          if (bOptimalResolution) {
            debugFBO.r += 0.0025;
            debugFBO.w = 1;
          }
#endif
        }

        currentPos = (currentPoolCoords-normToPoolTrans)/normToPoolScale;
      } else {
        currentPos = normBrickExitCoords+voxelSize*direction/rayLength;
      }

      lastBrickCoords = brickCoords;

     // global position along the ray
      t = length(normEntryPos-normBrickExitCoords)/rayLength;

      if (t > 0.9999) {
        TerminateRay(bOptimalResolution);
        return;
      }

#ifdef DEBUG
      if (bEmpty && bOptimalResolution) {
        debugFBO.g += 0.05;
        debugFBO.w = 1;
      }
#endif

    }
  }

  TerminateRay(bOptimalResolution);
}


//---------------------------------------------------------------------------------------------
#version 420 core

layout(binding=2) uniform sampler1D transferFunction;

uniform float fTransScale;

float samplePool(vec3 coords);

vec4 ComputeColorFromVolume(vec3 currentPoolCoords, vec3 modelSpacePosition, vec3 sampleDelta) {
  // fetch volume
  float data = samplePool(currentPoolCoords);

  // apply 1D TF
  return texture(transferFunction, data*fTransScale);
}

/** ----------------------------------------------------------------
  \file  Compositing.glsl
*/

vec4 UnderCompositing(vec4 src, vec4 dst) {
  vec4 result = dst;
  result.rgb   += src.rgb * (1.0-dst.a)*src.a;
  result.a     += src.a   * (1.0-dst.a);
  return result;
}

//---------------------------------------------------------------------------------------------
#version 420 core

layout(binding = 5, r32ui) coherent uniform uimage1D hashTable;

uint Serialize(uvec4 bd) {
  return 1 + bd.x + bd.y * 2 + bd.z * 4 + bd.w * 8;
}

uint HashValue(uint serializedValue) {
  return uint(serializedValue % 509);
}

uint AccessHashTable(uint hashValue, uint serializedValue) {
  int hashPosition = int(hashValue);
  return imageAtomicCompSwap(hashTable,hashPosition, uint(0), serializedValue);
}

uint Hash(uvec4 bd) {
  uint rehashCount = 0;
  uint serializedValue =  Serialize(bd);

  do {
    uint hash = HashValue(serializedValue+rehashCount);
    uint valueInImage = AccessHashTable(hash, serializedValue);
    if (valueInImage == 0 || valueInImage == serializedValue)
      return rehashCount;
  } while (++rehashCount < 10);

  return uint(10);
}
//---------------------------------------------------------------------------------------------

#version 420 core

layout(binding = 3) uniform usampler3D metaData;
#define iMetaTextureSize uvec3(347, 347, 347)
// #define iMetaTextureWidth 347

#define BI_CHILD_EMPTY 1
#define BI_EMPTY 2
#define BI_MISSING 0
#define BI_FLAG_COUNT 3

layout(binding = 4) uniform sampler3D volumePool;
#define iPoolSize ivec3(300, 300, 300)
#define volumeSize vec3(400, 296, 352)
#define poolAspect vec3(1, 1, 1)
#define poolCapacity ivec3(300, 300, 300)
// the total size of a brick in the pool, including the boundary
#define maxTotalBrickSize ivec3(1, 1, 1)
// just the addressable (inner) size of a brick
#define maxInnerBrickSize  ivec3(1, 1, 1)
// brick overlap voxels (in pool texcoords)
#define overlap vec3(0, 0, 0)
uniform float fLoDFactor;
uniform float fLevelZeroWorldSpaceError;
uniform vec3 volumeAspect;
#define iMaxLOD 1
uniform uint vLODOffset[2] = uint[](uint(0), uint(41676800));
uniform vec3 vLODLayout[2] = vec3[](
  vec3(399.99993896484375, 295.99996948242187, 351.99996948242187),// Level 0
  vec3(199.99996948242187, 147.99998474121094, 175.99998474121094)// Level 1
);
uniform uvec2 iLODLayoutSize[3] = uvec2[](
   uvec2(0, 0),// dummy Level that we'll never access to fix ATI/AMD issue where we cannot access the first element in a uniform integer array (does always return 0)
   uvec2(400, 118400),// Level 0
   uvec2(200, 29600)// Level 1
);

uint Hash(uvec4 brick);

uint ReportMissingBrick(uvec4 brick) {
  return Hash(brick);
}

ivec3 GetBrickIndex(uvec4 brickCoords) {
  uint iLODOffset  = vLODOffset[brickCoords.w];
  uvec2 iAMDFixedLODLayoutSize = iLODLayoutSize[brickCoords.w+1];
  uint iBrickIndex = iLODOffset + brickCoords.x + brickCoords.y * iAMDFixedLODLayoutSize.x + brickCoords.z * iAMDFixedLODLayoutSize.y;
  return ivec3(iBrickIndex % iMetaTextureSize[0],
               (iBrickIndex / iMetaTextureSize[0]) % iMetaTextureSize[1],  iBrickIndex /   (iMetaTextureSize[0]*iMetaTextureSize[1]));
}

uint GetBrickInfo(uvec4 brickCoords) {
  return texelFetch(metaData, GetBrickIndex(brickCoords), 0).r;
}

uvec4 ComputeBrickCoords(vec3 normEntryCoords, uint iLOD) {
  return uvec4(normEntryCoords*vLODLayout[iLOD], iLOD);
}

void GetBrickCorners(uvec4 brickCoords, out vec3 corners[2]) {
  corners[0] = vec3(brickCoords.xyz)   / vLODLayout[brickCoords.w];
  corners[1] = vec3(brickCoords.xyz+1) / vLODLayout[brickCoords.w];
}

vec3 BrickExit(vec3 pointInBrick, vec3 dir, in vec3 corners[2]) {
  vec3 div = 1.0 / dir;
  ivec3 side = ivec3(step(0.0,div));
  vec3 tIntersect;
  tIntersect.x = (corners[side.x].x - pointInBrick.x) * div.x;
  tIntersect.y = (corners[side.y].y - pointInBrick.y) * div.y;
  tIntersect.z = (corners[side.z].z - pointInBrick.z) * div.z;
  return pointInBrick +
         min(min(tIntersect.x, tIntersect.y), tIntersect.z) * dir;
}
 
uvec3 InfoToCoords(in uint brickInfo) {
  uint index = brickInfo-BI_FLAG_COUNT;
  uvec3 vBrickCoords;
  vBrickCoords.x = index % poolCapacity.x;
  vBrickCoords.y = (index / poolCapacity.x) % poolCapacity.y;
  vBrickCoords.z = index / (poolCapacity.x*poolCapacity.y);
  return vBrickCoords;
}
 
void BrickPoolCoords(in uint brickInfo,  out vec3 corners[2]) {
  uvec3 poolVoxelPos = InfoToCoords(brickInfo) * maxTotalBrickSize;
  corners[0] = (vec3(poolVoxelPos)                   / vec3(iPoolSize))+ overlap;
  corners[1] = (vec3(poolVoxelPos+maxTotalBrickSize) / vec3(iPoolSize))- overlap;
}
 
void NormCoordsToPoolCoords(in vec3 normEntryCoords,
in vec3 normExitCoords,
in vec3 corners[2],
in uint brickInfo,
out vec3 poolEntryCoords,
out vec3 poolExitCoords,
out vec3 normToPoolScale,
out vec3 normToPoolTrans) {
  vec3 poolCorners[2];
  BrickPoolCoords(brickInfo, poolCorners);
  normToPoolScale = (poolCorners[1]-poolCorners[0])/(corners[1]-corners[0]);
  normToPoolTrans = poolCorners[0]-corners[0]*normToPoolScale;
  poolEntryCoords  = (normEntryCoords * normToPoolScale + normToPoolTrans);
  poolExitCoords   = (normExitCoords  * normToPoolScale + normToPoolTrans);
}

bool GetBrick(in vec3 normEntryCoords, inout uint iLOD, in vec3 direction,
              out vec3 poolEntryCoords, out vec3 poolExitCoords,
              out vec3 normExitCoords, out bool bEmpty,
              out vec3 normToPoolScale, out vec3 normToPoolTrans, out uvec4 brickCoords) {
  normEntryCoords = clamp(normEntryCoords, 0.0, 1.0);
  bool bFoundRequestedResolution = true;
  brickCoords = ComputeBrickCoords(normEntryCoords, iLOD);
  uint  brickInfo   = GetBrickInfo(brickCoords);
  if (brickInfo == BI_MISSING) {
    uint iStartLOD = iLOD;
    ReportMissingBrick(brickCoords);
    // when the requested resolution is not present look for lower res
    bFoundRequestedResolution = false;
    do {
      iLOD++;
      brickCoords = ComputeBrickCoords(normEntryCoords, iLOD);
      brickInfo   = GetBrickInfo(brickCoords);
          } while (brickInfo == BI_MISSING);
  }
  // next line check for BI_EMPTY or BI_CHILD_EMPTY (BI_MISSING is
  // excluded by code above!)
  bEmpty = (brickInfo <= BI_EMPTY);
  if (bEmpty) {
    // when we find an empty brick check if the lower resolutions are also empty
    for (uint ilowResLOD = iLOD+1; ilowResLOD<iMaxLOD;++ilowResLOD) {
      uvec4 lowResBrickCoords = ComputeBrickCoords(normEntryCoords, ilowResLOD);
      uint lowResBrickInfo = GetBrickInfo(lowResBrickCoords);
      if (lowResBrickInfo == BI_CHILD_EMPTY) {
        brickCoords = lowResBrickCoords;
        brickInfo = lowResBrickInfo;
        iLOD = ilowResLOD;
      } else {
        break;
      }
    }
  }
  vec3 corners[2];
  GetBrickCorners(brickCoords, corners);
  normExitCoords = BrickExit(normEntryCoords, direction, corners);
  if (bEmpty) 
    return bFoundRequestedResolution;
  NormCoordsToPoolCoords(normEntryCoords, normExitCoords, corners,
                         brickInfo, poolEntryCoords, poolExitCoords,
                         normToPoolScale, normToPoolTrans);
  return bFoundRequestedResolution;
}

vec3 GetSampleDelta() {
  return 1.0/vec3(iPoolSize);
}

vec3 TransformToPoolSpace(in vec3 direction, in float sampleRateModifier) {
  // normalize the direction
  direction *= volumeSize;
  direction = normalize(direction);
  // scale to volume pool's norm coordinates
  direction /= vec3(iPoolSize);
  // do (roughly) two samples per voxel and apply user defined sample density
  return direction / (2.0*sampleRateModifier);
}
 
float samplePool(vec3 coords) {
 return texture(volumePool, coords).r;
}
 
float samplePoolAlpha(vec3 coords) {
 return texture(volumePool, coords).a;
}
 
vec3 samplePool3(vec3 coords) {
 return texture(volumePool, coords).rgb;
}
 
vec4 samplePool4(vec3 coords) {
 return texture(volumePool, coords);
}
 
uint ComputeLOD(float dist) {
  // opengl -> negative z-axis hence the minus
  return min(iMaxLOD, uint(log2(fLoDFactor*(-dist)/fLevelZeroWorldSpaceError)));
}
