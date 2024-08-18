#version 420 core

struct Material {
    sampler2D diffuseTexture;
    sampler2D specularTexture;
    sampler2D ambientTexture;
    sampler2D alphaTexture;
    float shininess;
};

uniform Material material;

in vec3 fPos;
in vec3 fNorm;
in vec2 fUv;

out vec4 FragColor;

void main() {
    vec3 albedo = texture(material.diffuseTexture, fUv).rgb;
    float alpha = texture(material.alphaTexture, fUv).r;

    FragColor = vec4(albedo, alpha);
}