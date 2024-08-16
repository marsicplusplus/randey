#version 420 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aUv;

out vec3 FragPos;
out vec3 Normal;
out vec2 TexCoords;

uniform mat4 modelTransposeInverse;
uniform mat4 view;
uniform mat4 projection;
uniform mat4 model;

void main() {
    Normal = mat3(modelTransposeInverse) * aNormal;
    FragPos = (model * vec4(aPos, 1.0)).xyz;
    TexCoords = aUv;
    gl_Position = projection * view * vec4(FragPos, 1.0);

}