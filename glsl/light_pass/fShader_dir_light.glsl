#version 420 core

out vec4 FragColor;

layout(binding = 0) uniform sampler2D gPosition;
layout(binding = 1) uniform sampler2D gNormal;
layout(binding = 2) uniform sampler2D gAlbedoSpec;

uniform vec2 gScreenSize;
uniform vec3 gViewPos;

struct DirectionalLight {
    vec3 direction;  
  
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

uniform DirectionalLight dirLight;

void main() {             
    vec2 TexCoords = gl_FragCoord.xy / gScreenSize;

    // retrieve data from G-buffer
    vec3 FragPos = texture(gPosition, TexCoords).rgb;
    vec3 Normal = texture(gNormal, TexCoords).rgb;
    vec3 Albedo = texture(gAlbedoSpec, TexCoords).rgb;
    float Specular = texture(gAlbedoSpec, TexCoords).a;
    vec3 Ambient = Albedo.rgb;

    vec3 viewDir = normalize(gViewPos - FragPos);
    
    // Ambient
    vec3 ambient = Ambient * dirLight.ambient; 

    // Diffuse
    vec3 lightDir = normalize(-dirLight.direction);
    vec3 diffuse = max(dot(Normal, lightDir), 0.0) * Albedo * dirLight.diffuse;

    // Specular
    vec3 reflectedDir = reflect(-lightDir, Normal);
    float spec = pow(max(dot(viewDir, reflectedDir), 0.0), 16.0);
    vec3 specular = spec * dirLight.specular * Specular;

    vec3 ret = ambient + diffuse + specular;
        
    FragColor = vec4(ret, 1.0);
}