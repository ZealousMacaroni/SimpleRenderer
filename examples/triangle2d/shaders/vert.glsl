#version 410 core

layout(location = 0) in vec3 pPosition;

uniform mat4 uModel;
uniform mat4 uView;
uniform mat4 uPerspective;

void main() {
	gl_Position = uPerspective * uView * uModel * vec4(pPosition, 1.0);
}
