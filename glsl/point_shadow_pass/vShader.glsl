#version 420 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNorm;
layout (location = 2) in vec2 aUV;

uniform mat4 model;
uniform mat4 modelTransposeInverse;

void main() {
    gl_Position = model * vec4(aPos, 1.0);
} 