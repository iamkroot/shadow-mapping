#version 330 core
out vec4 FragColor;

in vec4 FragPos;

uniform mat4 lightSpace;
uniform float near_plane;
uniform float far_plane;

void main() {
    vec4 fragPosLightSpace = lightSpace * FragPos;
    float depthValue = fragPosLightSpace.z * 3.5 / far_plane;
    FragColor = vec4(vec3(1 - depthValue), 1);
}
