#version 420 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNorm;
layout (location = 2) in vec2 aUV;

uniform mat4 modelTransposeInverse;
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec3 fPos;
out vec3 fNorm;
out vec2 fUv;

void main() {
    fNorm = mat3(modelTransposeInverse) * aNorm;
    fPos = (model * vec4(aPos, 1.0)).xyz;
    fUv = aUV;
    gl_Position = projection * view * vec4(fPos, 1.0);
}