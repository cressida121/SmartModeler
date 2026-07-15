#version 460 core

layout (location = 0) in vec3 vertexColor;
layout (location = 1) in vec2 vertexUV;

layout (location = 0) out vec4 outColor;

uniform sampler2D theTexture;

void main() {
    outColor = texture(theTexture, vertexUV) * vec4(vertexColor.rgb, 1.0);
}