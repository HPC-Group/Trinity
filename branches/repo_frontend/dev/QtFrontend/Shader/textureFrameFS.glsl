#version 430 core

layout(binding=0) uniform sampler2D frameTexture;
in vec2 vScreenPosition;

layout(location = 0) out vec4 color;

void main()
{
	color = texture(frameTexture, vScreenPosition);
	//color = texelFetch(frameTexture, ivec2(gl_FragCoord.xy), 0);
	//color = vec4(0.5, 0.5, 0.5, 1.0);
}
