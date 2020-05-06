#version 330 core
out vec4 FragColor;

in vec4 FragPosLightSpace;

uniform float near_plane;
uniform float far_plane;

void main() {
    float depthValue = FragPosLightSpace.z * 3.5 / far_plane;
    FragColor = vec4(vec3(1 - depthValue), 1);
}
