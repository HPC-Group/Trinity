#version 420 core

// INPUT VARIABLES
layout (location=0) in vec3 inputPosition;

// OUTPUT VARIABLES
out vec2 vScreenPosition;

// UNIFORM VARIABLES

// Vertex Shader
void main(void)
{
  gl_Position = vec4(inputPosition, 1.0);
  vScreenPosition = vec2((inputPosition.xy + vec2(1.0,1.0))/vec2(2.0,2.0));
}
