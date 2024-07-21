#version 420 core

out vec4 FragColor;

layout(binding = 0) uniform sampler2D gPosition;
layout(binding = 1) uniform sampler2D gNormal;
layout(binding = 2) uniform sampler2D gAlbedoSpec;

uniform vec2 gScreenSize;
uniform vec3 gViewPos;

struct PointLight {
    vec3 position;  
  
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
	
    float constant;
    float linear;
    float quadratic;
};

uniform PointLight pointLight;

void main() {             

    vec2 TexCoords = gl_FragCoord.xy / gScreenSize;

    // retrieve data from G-buffer
    vec3 FragPos = texture(gPosition, TexCoords).rgb;
    vec3 Normal = texture(gNormal, TexCoords).rgb;
    vec3 Albedo = texture(gAlbedoSpec, TexCoords).rgb;
    vec3 Ambient = Albedo;
    float Specular = texture(gAlbedoSpec, TexCoords).a;

    vec3 viewDir = normalize(gViewPos - FragPos);
    
    // Ambient
    vec3 ambient = Ambient * pointLight.ambient; 

    // Diffuse
    vec3 lightDir = normalize(pointLight.position - FragPos);
    vec3 diffuse = max(dot(Normal, lightDir), 0.0) * Albedo * pointLight.diffuse;

    // Specular
    vec3 reflectedDir = reflect(-lightDir, Normal);
    float spec = pow(max(dot(viewDir, reflectedDir), 0.0), 16.0);
    vec3 specular = spec * pointLight.specular * Specular;


    // Attenuation
    float dist = length(pointLight.position - FragPos);
    float attenuation = 1.0 / (pointLight.constant + pointLight.linear * dist + 
                    pointLight.quadratic * (dist * dist)); 

    vec3 ret = ambient + diffuse + specular;
        
    FragColor = vec4(ret * attenuation, 1.0);
}  