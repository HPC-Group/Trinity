#version 420 core

layout(binding=0) uniform sampler2D outerPosition;
layout(binding=1) uniform sampler2D innerPosition;

layout(binding=2) uniform sampler2D outerNormal;
layout(binding=3) uniform sampler2D innerNormal;


uniform vec3 ColorOne;
uniform vec3 ColorTwo;
uniform vec3 volumePosition = vec3(0.0,0.0,0.0);
uniform float rad = 0.16;
uniform float grad = 1.0;
uniform vec3 scale;

// INPUT VARIABLES
in vec2 vScreenPosition;

// OUTPUT VARIABLES
out vec4 outputColor;





vec3 Lighting(vec3 vEyePos, 
              vec3 vPosition, vec3 vNormal, vec3 vLightAmbient,
              vec3 vLightDiffuse, vec3 vLightSpecular, vec3 vLightDir) {
  vec3 vViewDir    = normalize(vEyePos-vPosition);
  vec3 vReflection = normalize(reflect(vViewDir, vNormal));
  return clamp(
    vLightAmbient +
    vLightDiffuse * max(abs(dot(vNormal, vLightDir)),0.0) +
    vLightSpecular * pow(max(dot(vReflection, vLightDir),0.0),8.0), 0.0,1.0
  );
}

vec3 Lighting(vec3 vPosition, vec3 vNormal, vec3 vLightAmbient,
              vec3 vLightDiffuse, vec3 vLightSpecular, vec3 vLightDir) {

  return Lighting(vec3(0.0,0.0,0.0), vPosition, vNormal, vLightAmbient,
                  vLightDiffuse, vLightSpecular, vLightDir);
}






// Pixel Shader
void main(void)
{
    vec3 outPos = texture(outerPosition,vScreenPosition).xyz;
    if  (outPos.x == 0 &&
        outPos.y == 0 &&
        outPos.z == 0
    ){
        outputColor = vec4(0,0,0,1);
        return;
    }

    vec3 inPos = texture(innerPosition,vScreenPosition).xyz;

    float outW = texture(outerPosition,vScreenPosition).w;
    float inW = texture(innerPosition,vScreenPosition).w;

    vec3 outNormal = normalize(texture(outerNormal,vScreenPosition).xyz);
    vec3 inNormal = normalize(texture(innerNormal,vScreenPosition).xyz);

    ivec2 screenpos = ivec2(gl_FragCoord.xy);

    vec3 outNDistance = normalize(texelFetch(outerNormal, ivec2(screenpos.x,screenpos.y+1),0).xyz)-outNormal;
    outNDistance += normalize(texelFetch(outerNormal, ivec2(screenpos.x,screenpos.y-1),0).xyz)-outNormal;
    outNDistance += normalize(texelFetch(outerNormal, ivec2(screenpos.x+1,screenpos.y),0).xyz)-outNormal;
    outNDistance += normalize(texelFetch(outerNormal, ivec2(screenpos.x-1,screenpos.y),0).xyz)-outNormal;

    float curv = length(outNDistance);


      vec3 distanceVector = outPos-volumePosition;
      distanceVector = distanceVector*scale;
      float distance = length(distanceVector);

      float trans = 1.0-clamp(max(curv,distance/rad),0.0,1.0);

      

      vec3 lightOut = Lighting(outPos,outNormal,vec3(0.1,0.1,0.1),vec3(0.9,0.9,0.9),vec3(0.2,0.2,0.2),vec3(0,1,0));
      vec3 lightIn = Lighting(inNormal,inNormal,vec3(0.1,0.0,0.0),vec3(1.0,0.2,0.0),vec3(0.8,0.1,0.1),vec3(0,1,0));

      if(distance < rad)
      {
         outputColor =  vec4(mix(lightOut,lightIn,vec3(trans,trans,trans)),1);
         return;
      }
       if(distance > rad && distance < rad+0.005f)
      {
         outputColor = vec4(lightOut,1) * vec4(0.2,0.2,1.0,1.0);
         return;
      }
      outputColor = vec4(lightOut,1);

}
