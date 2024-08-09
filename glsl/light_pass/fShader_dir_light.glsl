#version 420 core

#define EPSILON 0.00001

out vec4 FragColor;

layout(binding = 0) uniform sampler2D gPosition;
layout(binding = 1) uniform sampler2D gNormal;
layout(binding = 2) uniform sampler2D gAlbedoSpec;
layout(binding = 5) uniform sampler2DShadow gShadowMap;

uniform vec2 gScreenSize;
uniform vec3 gViewPos;
uniform mat4 lightView;
uniform mat4 lightProj;

struct DirectionalLight {
    vec3 direction;  
  
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

uniform DirectionalLight dirLight;

float shadowFactor(vec4 FragPosLightSpace, vec3 normal, vec3 lightDir) {
    vec3 projCoords = FragPosLightSpace.xyz / FragPosLightSpace.w;
    vec2 UVCoords;
    UVCoords.x = projCoords.x * 0.5 + 0.5;
    UVCoords.y = projCoords.y * 0.5 + 0.5;
    float z = projCoords.z * 0.5 + 0.5;

    float xOffset = 1.0/gScreenSize.x;
    float yOffset = 1.0/gScreenSize.y;

    float Factor = 0.0;

    float bias = max(0.05 * (1.0 - dot(normal, lightDir)), 0.005);  

    for (int y = -1 ; y <= 1 ; y++) {
        for (int x = -1 ; x <= 1 ; x++) {
            vec2 Offsets = vec2(x * xOffset, y * yOffset);
            vec3 UVC = vec3(UVCoords + Offsets, z - bias);
            Factor += texture(gShadowMap, UVC);
        }
    }

    return (0.5 + (Factor / 18.0));
}

void main() {             
    vec2 TexCoords = gl_FragCoord.xy / gScreenSize;

    // retrieve data from G-buffer
    vec3 FragPos = texture(gPosition, TexCoords).rgb;
    vec3 Normal = texture(gNormal, TexCoords).rgb;
    vec3 Albedo = texture(gAlbedoSpec, TexCoords).rgb;
    float Specular = texture(gAlbedoSpec, TexCoords).a;
    vec3 Ambient = Albedo.rgb;
    vec4 FragPosLightSpace = lightProj * lightView * vec4(FragPos, 1.0);

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

    float shadow = shadowFactor(FragPosLightSpace, Normal, lightDir);
    vec3 ret = ambient + shadow * (diffuse + specular);
        
    FragColor = vec4(ret, 1.0);
}