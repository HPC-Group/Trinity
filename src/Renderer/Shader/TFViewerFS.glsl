#version 420 core

layout(binding=0) uniform sampler1D compose;

uniform float mode;

// INPUT VARIABLES
in vec2 vScreenPosition;

// OUTPUT VARIABLES
out vec4 outputColor;

// Pixel Shader
void main(void)
{
  vec4 TFColor = texture(compose,vScreenPosition.x);


  if(mode == 0.0){
	  if(TFColor.w > vScreenPosition.y){
	  	outputColor = TFColor;
	  }else{
	  	outputColor = vec4(0,0,0,1);
	  }
  }else if(mode == 1.0){
	  outputColor = vec4(TFColor.x,0,0,1);
  }
  else if(mode == 2.0){
	  outputColor = vec4(0,TFColor.y,0,1);
  }
  else if(mode == 3.0){
	  outputColor = vec4(0,0,TFColor.z,1);
  }
}
