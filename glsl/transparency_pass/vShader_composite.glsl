#version 420 core

layout (location = 0) in vec3 aPos;
layout (location = 2) in vec2 aUv;

uniform mat4 view;
uniform mat4 projection;
uniform mat4 model;

out vec2 UVCoords;

void main() {
    gl_Position = projection * view * model * vec4(aPos, 1.0);
    UVCoords = aUv;
}