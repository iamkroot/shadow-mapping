#version 330 core
out vec4 FragColor;

in vec4 FragPos;

uniform mat4 lightSpace;
uniform float near_plane;
uniform float far_plane;

void main() {
    vec4 fragPosLightSpace = lightSpace * FragPos;
    float depthValue = fragPosLightSpace.z / fragPosLightSpace.w;// perspective divide
    depthValue = depthValue * 2 - 1;// NDC
    depthValue = (2 * near_plane) / (far_plane + near_plane - depthValue * (far_plane - near_plane));// linearize
    FragColor = vec4(vec3(1 - depthValue), 1);
}
