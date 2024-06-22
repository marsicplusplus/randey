#version 330 core

in vec3 fPos;
in vec3 fNorm;

out vec4 FragColor;

uniform vec3 lightPos;
uniform vec3 lightColor;
uniform vec3 objColor;
uniform vec3 viewPos;

void main() {
    float ambientStrength = 0.1;
    vec3 ambient = ambientStrength * lightColor;

    vec3 norm = normalize(fNorm);
    vec3 lightDir = normalize(lightPos - fPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;

    float specularStrength = 0.5;
    vec3 viewDir = normalize(viewPos - fPos);
    vec3 reflectedDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectedDir), 0.0), 32.0);
    vec3 specular = spec * specularStrength * lightColor;

    vec3 res = (ambient + diffuse + specular) * objColor;
    FragColor = vec4(res, 1.0);
}