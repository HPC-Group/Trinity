#version 420 core

layout(binding=0) uniform sampler2D compose;
layout(binding=1) uniform sampler2D debug;

uniform float blendfactor;

// INPUT VARIABLES
in vec2 vScreenPosition;

// OUTPUT VARIABLES
out vec4 outputColor;

// Pixel Shader
void main(void)
{
  vec4 imageColor = texture(compose,vScreenPosition);
  vec4 debugColor = texture(debug,vScreenPosition);

  outputColor = (imageColor * blendfactor) + ((1.0-blendfactor) * debugColor);
}
