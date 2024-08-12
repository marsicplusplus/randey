#version 420 core

in vec3 TexCoord;

out vec4 FragColor;

layout(binding = 0) uniform samplerCube gCubemapTexture;

void main() {
    FragColor = texture(gCubemapTexture, TexCoord);
}