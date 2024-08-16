#version 420 core

#define MAX_POINT_LIGHTS 16
#define MAX_DIR_LIGHTS 16

layout (location = 0) out vec4 outData;
layout (location = 1) out float outAlpha;

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords;

struct Material {
    layout (binding = 0) sampler2D diffuseTexture;
    layout (binding = 1) sampler2D specularTexture;
    layout (binding = 2) sampler2D ambientTexture;
    layout (binding = 3) sampler2D alphaTexture; 
    float shininess;
};

// struct PointLight {
//     vec4 position;  
//     vec4 ambient;
//     vec4 diffuse;
//     vec4 specular;
//     vec4 attenuationTerms; // [constant, linear, quadratic]
// };

// layout(std140, binding = 0) uniform PointLightsBlock
// {
//     PointLight lights[MAX_POINT_LIGHTS];
// } block_pointLights;

// struct DirectionalLight {
//     vec3 direction;  
  
//     vec3 ambient;
//     vec3 diffuse;
//     vec3 specular;
// };

uniform vec2 gScreenSize;
uniform vec3 gViewPos;
uniform Material material;
// uniform PointLight pointLights[MAX_POINT_LIGHTS];
// uniform DirectionalLight directionalLights[MAX_DIR_LIGHTS];
// uniform int numPointLights;
// uniform int numDirectionalLights;

// vec3 calcPointLight(uint idx) {
//     PointLight light = block_pointLights.lights[idx];
//     vec3 viewDir = normalize(gViewPos - FragPos);
//     vec3 lightDir = normalize(light.position.xyz - FragPos);
//     // diffuse shading
//     float diff = max(dot(Normal, lightDir), 0.0);
//     // specular shading
//     vec3 reflectDir = reflect(-lightDir, Normal);
//     float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
//     // attenuation
//     float constant = light.attenuationTerms.x;
//     float linear = light.attenuationTerms.y;
//     float quadratic = light.attenuationTerms.z;
//     float dist          = length(light.position.xyz - FragPos);
//     float attenuation   = 1.0 / (constant + linear * dist + 
//                     quadratic * (dist * dist)); 

//     // combine results
//     vec3 ambient  = light.ambient.xyz  * vec3(texture(material.ambientTexture, TexCoords));
//     vec3 diffuse  = light.diffuse.xyz  * diff * vec3(texture(material.diffuseTexture, TexCoords));
//     vec3 specular = light.specular.xyz * spec * vec3(texture(material.specularTexture, TexCoords));
//     ambient  *= attenuation;
//     diffuse  *= attenuation;
//     specular *= attenuation;
//     return (ambient + diffuse + specular);
// }

// vec3 calcDirLight(uint idx) {
//     DirectionalLight light = directionalLights[idx];
//     vec3 viewDir = normalize(gViewPos - FragPos);
    
//     // Ambient
//     vec3 ambient  = light.ambient  * vec3(texture(material.ambientTexture, TexCoords));

//     // Diffuse
//     vec3 lightDir = normalize(-light.direction);
//     vec3 diffuse = max(dot(Normal, lightDir), 0.0) * vec3(texture(material.diffuseTexture, TexCoords)) * light.diffuse;

//     // Specular
//     vec3 reflectedDir = reflect(-lightDir, Normal);
//     float spec = pow(max(dot(viewDir, reflectedDir), 0.0), 16.0);
//     vec3 specular = spec * light.specular * vec3(texture(material.specularTexture, TexCoords));

//     return ambient + diffuse + specular;
// }

void main() {
    vec3 color = texture(material.diffuseTexture, TexCoords).rgb;
    float alpha = texture(material.alphaTexture, TexCoords).r;
    float dist = sqrt(TexCoords.x * TexCoords.x + TexCoords.y * TexCoords.y);
    // float z = gl_FragCoord.z;
    float w = gl_FragCoord.w;

    vec4 outColor = vec4(color.rgb, alpha * (1.0 - smoothstep(0.85, 1.0, dist)));
    float csz = abs(1.0 / w);

    float factor = 1.0/200.0;
    float z = factor * csz;
    float weight = clamp((0.03 / (1e-5 + pow(z, 4.0))), 1e-4, 3e3);

    // vec4 premultipliedColor = vec4(color.rgb * alpha, alpha);
    // float a = min(1.0, premultipliedColor.a) * 8.0 + 0.01;
    // float b = -z * 0.95 + 1.0;
    // float w = clamp(a * a * a * 1e8 * b * b * b, 1e-2, 3e2);

    // outData = premultipliedColor * w;
    outData = vec4(weight);
    outAlpha = outColor.a;

    // for(uint i = 0; i < numPointLights; ++i) {
    //     color += calcPointLight(i);
    // } 

    // for(uint i = 0; i < numDirectionalLights; ++i) {
    //     color += calcDirLight(i);
    // }

    // FragColor = vec4(color, texture(material.alphaTexture, TexCoords));
    // FragColor = vec4(color, 1.0);
}