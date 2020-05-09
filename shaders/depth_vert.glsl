#version 330 core
layout (location = 0) in vec3 aPos; // Input vertex 

uniform mat4 lightSpace; // lightspace transformation
uniform mat4 model; // Model transformation

void main() {
    gl_Position = lightSpace * model * vec4(aPos, 1.0);
}