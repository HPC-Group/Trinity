#version 410 core

// INPUT VARIABLES
in vec3 entranceInModelSpace;
in vec3 viewspace;

// OUTPUT VARIABLES
out vec4 outputColor;


// Pixel Shader
void main(void)
{
  outputColor = vec4(entranceInModelSpace,viewspace.z);
}
