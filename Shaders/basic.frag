#version 420 core

layout( location = 0 ) out vec4 fragColor;
smooth in vec4 smoothColor;

void main() {
	fragColor = smoothColor;
}
