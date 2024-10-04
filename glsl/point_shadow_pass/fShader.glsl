#version 420

in vec4 FragPos;

uniform vec3 gLightPos;
uniform float gFarPlane;

void main() {
    float lightDistance = length(FragPos.xyz - gLightPos);
    lightDistance = lightDistance / gFarPlane; // map to [0; 1]
    gl_FragDepth = lightDistance;
} 