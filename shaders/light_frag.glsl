#version 330 core
out vec4 FragColor; // Fragment color

uniform vec3 lightColor; // Light's input color

void main() {
    FragColor = vec4(lightColor, 1);
}
