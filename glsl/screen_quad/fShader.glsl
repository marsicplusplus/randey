#version 330 core
out vec4 FragColor;

in vec2 fUv;

uniform sampler2D screenTexture;

void main()
{
    vec3 col = texture(screenTexture, fUv).rgb;
    FragColor = vec4(col, 1.0);
} 