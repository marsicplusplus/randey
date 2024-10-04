#version 420 core
layout (location = 0) out vec3 gPosition;
layout (location = 1) out vec3 gNormal;
layout (location = 2) out vec4 gAlbedoSpec;

in vec3 fPos;
in vec2 fUv;
in vec3 fNorm;
in vec3 fTangent;

struct Material {
    sampler2D diffuseTexture;
    sampler2D specularTexture;
    sampler2D ambientTexture;
    sampler2D alphaTexture;
    sampler2D normalTexture;
    float shininess;
    float useSpecular;
    float useNormal;
};

uniform Material material;

vec3 calcBumpedNormal() {
    vec3 normal = normalize(fNorm);
    vec3 tangent = normalize(fTangent);
    tangent = normalize(tangent - dot(tangent, normal) * normal);
    vec3 bitangent = cross(tangent, normal);

    vec3 bumpMapNormal = texture(material.normalTexture, fUv).xyz;
    bumpMapNormal = 2.0 * bumpMapNormal - vec3(1.0, 1.0, 1.0);
    vec3 newNormal;
    mat3 TBN = mat3(tangent, bitangent, normal);
    newNormal = TBN * bumpMapNormal;
    return newNormal;
}

void main()
{    
    // store the fragment position vector in the first gbuffer texture
    gPosition = fPos;
    // also store the per-fragment normals into the gbuffer
    // vec3 bumpedNormal = calcBumpedNormal();
    // gNormal = (bumpedNormal * material.useNormal) + (fNorm * (1.0 - material.useNormal));
    gNormal = (fNorm * (1.0 - material.useNormal));
    gNormal = normalize(gNormal);

    // and the diffuse per-fragment color
    gAlbedoSpec.rgb = texture(material.diffuseTexture, fUv).rgb;
    // gAlbedoSpec.rgb = vec3(material.useNormal, material.useNormal, material.useNormal);

    // store specular intensity in gAlbedoSpec's alpha component
    gAlbedoSpec.a = texture(material.specularTexture, fUv).r * material.useSpecular;
    // gAmbient.rgb = texture(material.ambientTexture, fUv).rgb;
}  