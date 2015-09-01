#version 430 core
in vec3 position;
out vec2 vScreenPosition;

void main() {
    gl_Position = vec4(position, 1.0f);
	vScreenPosition = position.xy * 0.5 + 0.5;
}
