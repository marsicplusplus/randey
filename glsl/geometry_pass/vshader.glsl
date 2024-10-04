#version 420 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNorm;
layout (location = 2) in vec2 aUV;
layout (location = 3) in vec3 aTangent;

uniform mat4 modelTransposeInverse;
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec3 fPos;
out vec3 fNorm;
out vec2 fUv;
out vec3 fTangent;

void main() {
    fPos = (model * vec4(aPos, 1.0)).xyz;
    fUv = aUV;
    fNorm = mat3(modelTransposeInverse) * aNorm;
    fTangent = mat3(modelTransposeInverse) * aTangent;
    // fNorm = (model * vec4(aNorm, 0.0)).xyz;
    // fTangent = (model * vec4(aTangent, 0.0)).xyz;
    gl_Position = projection * view * vec4(fPos, 1.0);
}