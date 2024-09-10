#version 330 core
layout (location = 0) out vec3 gPosition;
layout (location = 1) out vec3 gNormal;
layout (location = 2) out vec4 gAlbedoSpec;

in vec3 fPos;
in vec2 fUv;
in vec3 fNorm;

struct Material {
    sampler2D diffuseTexture;
    sampler2D specularTexture;
    sampler2D ambientTexture;
    sampler2D alphaTexture;
    float shininess;
    float useSpecular;
};

uniform Material material;

void main()
{    
    // store the fragment position vector in the first gbuffer texture
    gPosition = fPos;
    // also store the per-fragment normals into the gbuffer
    gNormal = normalize(fNorm);
    // and the diffuse per-fragment color
    gAlbedoSpec.rgb = texture(material.diffuseTexture, fUv).rgb;
    // store specular intensity in gAlbedoSpec's alpha component
    gAlbedoSpec.a = texture(material.specularTexture, fUv).r * material.useSpecular;
    // gAmbient.rgb = texture(material.ambientTexture, fUv).rgb;
}  