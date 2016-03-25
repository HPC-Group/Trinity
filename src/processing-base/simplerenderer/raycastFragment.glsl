#version 330 core

// INPUT VARIABLES
in vec3 entranceInViewSpace;

// OUTPUT VARIABLES
layout(location=0) out vec4 outputColor;

// TEXTURES
uniform sampler1D transferfunc;
uniform sampler3D volume;
uniform sampler2D rayExit;

/*
 void opacityCorrectColor(float opacity_correction, inout vec4 color) {
  color.a = 1.0 - pow(1.0 - color.a, opacity_correction);
}
*/

vec4 underCompositing(vec4 src, vec4 dst) {
  vec4 result = dst;
  result.rgb   += src.rgb * (1.0-dst.a)*src.a;
  result.a     += src.a   * (1.0-dst.a);
  return result;
}


// Pixel Shader
void main(void)
{
  ivec2 screenpos = ivec2(gl_FragCoord.xy);
  vec3 rayExitPos = texelFetch(rayExit, screenpos,0).xyz;
  vec3 rayEntryPos = entranceInViewSpace;
  
  vec3 rayDir = rayExitPos-rayEntryPos;
  float rayDist = length(rayDir);
  rayDir /= rayDist;
  
  vec3 rayDelta = rayDir / 128;
  vec3 pos = rayEntryPos;
  
  outputColor = vec4(0,0,0,0);
  while (length(pos-rayEntryPos) < rayDist) {
    float volVal = texture(volume,pos).r;
    vec4 color = texture(transferfunc,volVal);
    
    outputColor = underCompositing(color, outputColor);
    
    if (outputColor.a > 0.99) {
      return;
    }
    
    pos += rayDelta;
  }
}
