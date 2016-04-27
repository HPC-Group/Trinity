#version 330 core

// INPUT VARIABLES
in vec3 entranceInViewSpace;

// OUTPUT VARIABLES
layout(location=0) out vec4 outputColor;

// Pixel Shader
void main(void)
{
  outputColor = vec4(entranceInViewSpace.xyz,1.0);
}
