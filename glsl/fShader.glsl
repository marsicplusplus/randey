#version 330 core

struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
};

struct Light {
    vec3 position;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

in vec3 fPos;
in vec3 fNorm;

out vec4 FragColor;

uniform vec3 viewPos;

uniform Material material;
uniform Light light;

void main() {
    // Phong:
    vec3 ambient = material.ambient * light.ambient;

    vec3 norm = normalize(fNorm);
    vec3 lightDir = normalize(light.position - fPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * light.diffuse * material.diffuse;

    vec3 viewDir = normalize(viewPos - fPos);
    vec3 reflectedDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectedDir), 0.0), material.shininess);
    vec3 specular = spec * light.specular * material.specular;

    vec3 res = (ambient + diffuse + specular);
    FragColor = vec4(res, 1.0);
}