#version 420 core

struct Material {
    sampler2D diffuseTexture;
    sampler2D specularTexture;
    sampler2D ambientTexture;
    sampler2D alphaTexture;
    float shininess;
};

struct PointLight {
    vec3 position;  
  
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
	
    float constant;
    float linear;
    float quadratic;
};

struct DirectionalLight {
    vec3 direction;  
  
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
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