#version 420 core

layout(binding=0) uniform sampler2D compose;


uniform vec3 ColorOne;
uniform vec3 ColorTwo;

// INPUT VARIABLES
in vec2 vScreenPosition;

// OUTPUT VARIABLES
out vec4 outputColor;

// Pixel Shader
void main(void)
{
  vec4 Color = texture(compose,vScreenPosition);
  vec3 BackgroundColor = ColorOne * vScreenPosition.y + ColorTwo*(1-vScreenPosition.y);
  
  outputColor = Color * Color.w + vec4(BackgroundColor,1) * (1-Color.w);

outputColor = texture(compose,vScreenPosition);
}

