#version 330 core
out vec4 FragColor; // Fragment color

in vec4 FragPos; // Fragment position

uniform mat4 lightSpace; // lightspace transform
uniform float near_plane; // near clipping plane z value
uniform float far_plane; // far clipping plane z value

void main() {
    vec4 fragPosLightSpace = lightSpace * FragPos;
    float depthValue = fragPosLightSpace.z / fragPosLightSpace.w;// perspective divide
    depthValue = depthValue * 2 - 1;// NDC
    depthValue = (2 * near_plane) / (far_plane + near_plane - depthValue * (far_plane - near_plane));// linearize
    FragColor = vec4(vec3(1 - depthValue), 1);
}
