#version 330 core

// INPUT VARIABLES
in vec3 normalview;
in vec3 entranceInViewSpace;

// OUTPUT VARIABLES
layout(location=0) out vec4 outputColor;

// Pixel Shader
void main(void)
{
  outputColor = vec4(normalview,entranceInViewSpace.z);
}
