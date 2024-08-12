#version 420 core

layout (location = 0) in vec3 aPos;

uniform mat4 gView;
uniform mat4 gProjection;

out vec3 TexCoord;

void main() {
    mat4 view = mat4(mat3(gView));
    vec4 pos = gProjection * view * vec4(aPos, 1.0);
    gl_Position = pos.xyww;
    TexCoord = aPos;
}