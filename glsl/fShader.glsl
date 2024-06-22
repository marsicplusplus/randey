#version 330 core

out vec4 FragColor;

in vec2 uvCoord;

uniform sampler2D tex;

void main() {
    FragColor = vec4(1.0, 0.8, 0.2, 1.0);    // FragColor = vec4(ourColor, 1.0);
    // FragColor = texture(tex, uvCoord);
}