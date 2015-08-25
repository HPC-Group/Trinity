#version 410 core

// INPUT VARIABLES
layout (location=0) in vec3 inputPosition;
layout (location=1) in vec3 inputNormal;
layout (location=2) in vec2 inputTexcoord;

// OUTPUT VARIABLES
out vec3 entranceInModelSpace;
out vec3 viewspace;

// UNIFORM VARIABLES
uniform mat4 worldMatrixInverse;
uniform mat4 viewMatrixInverse;
uniform mat4 projectionMatrixInverse;

// Vertex Shader
void main(void)
{
  gl_Position = vec4(inputPosition, 1.0);
  
  vec4 near = (projectionMatrixInverse*vec4(inputPosition.x,inputPosition.y,-1,1));
  entranceInModelSpace = near.xyz/near.w;
  viewspace = entranceInModelSpace;
  entranceInModelSpace = (worldMatrixInverse*viewMatrixInverse*vec4(entranceInModelSpace,1)).xyz;
  entranceInModelSpace+= vec3(0.5,0.5,0.5);
}
