#version 420 core

out vec4 FragColor;

layout(binding = 0) uniform sampler2D gPosition;
layout(binding = 1) uniform sampler2D gNormal;
layout(binding = 2) uniform sampler2D gAlbedoSpec;

layout(binding = 5) uniform samplerCube gShadowMap;

uniform vec2 gScreenSize;
uniform vec3 gViewPos;

uniform float gFarPlane;

struct PointLight {
    vec3 position;  
  
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
	
    float constant;
    float linear;
    float quadratic;

    float isShadowCaster;
};
uniform PointLight pointLight;

vec3 sampleOffsetDirections[20] = vec3[] (
   vec3( 1,  1,  1), vec3( 1, -1,  1), vec3(-1, -1,  1), vec3(-1,  1,  1), 
   vec3( 1,  1, -1), vec3( 1, -1, -1), vec3(-1, -1, -1), vec3(-1,  1, -1),
   vec3( 1,  1,  0), vec3( 1, -1,  0), vec3(-1, -1,  0), vec3(-1,  1,  0),
   vec3( 1,  0,  1), vec3(-1,  0,  1), vec3( 1,  0, -1), vec3(-1,  0, -1),
   vec3( 0,  1,  1), vec3( 0, -1,  1), vec3( 0, -1, -1), vec3( 0,  1, -1)
);   

float shadowCalculation(vec3 fragPos) {
    // // get vector between fragment position and light position
    // vec3 fragToLight = fragPos - pointLight.position;
    // // use the light to fragment vector to sample from the depth map    
    // float closestDepth = texture(gShadowMap, fragToLight).r;
    // // it is currently in linear range between [0,1]. Re-transform back to original value
    // closestDepth *= gFarPlane;
    // // now get current linear depth as the length between the fragment and light position
    // float currentDepth = length(fragToLight);
    // // now test for shadows
    // float bias = 0.05; 
    // float shadow = currentDepth -  bias > closestDepth ? 1.0 : 0.0;

    // return shadow;
    vec3 fragToLight = fragPos - pointLight.position;
    float currentDepth = length(fragToLight);

    float shadow = 0.0;
    float bias   = 0.15;
    int samples  = 20;
    float viewDistance = length(gViewPos - fragPos);
    float diskRadius = 0.05;
    for(int i = 0; i < samples; ++i)
    {
        float closestDepth = texture(gShadowMap, fragToLight + sampleOffsetDirections[i] * diskRadius).r;
        closestDepth *= gFarPlane;   // undo mapping [0;1]
        if(currentDepth - bias > closestDepth)
            shadow += 1.0;
    }
    shadow /= float(samples);  
        
    return shadow;
}



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

    float shadow = shadowCalculation(FragPos) * pointLight.isShadowCaster;

    vec3 ret = (ambient + (1.0 - shadow) * (diffuse + specular)) * attenuation;
        
    FragColor = vec4(ret, 1.0);
}  