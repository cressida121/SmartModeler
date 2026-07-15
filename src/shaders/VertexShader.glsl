#version 460 core

layout (location = 0) in vec3 inPosition;
layout (location = 1) in vec3 inColor;
layout (location = 2) in vec2 inUV;

layout (location = 0) out vec3 vertexColor;
layout (location = 1) out vec2 vertexUV;

layout (location = 0) uniform mat4 cameraMatrix;
layout (location = 1) uniform mat4 projectionMatrix;

void main() {
	gl_Position = projectionMatrix * cameraMatrix * vec4(inPosition, 1.0);
	vertexColor = inColor;
	vertexUV = inUV;
}