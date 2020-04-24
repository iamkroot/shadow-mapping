#include <fstream>
#include <sstream>
#include "Shader.h"

Shader::Shader(const std::string &vertPath, const std::string &fragPath) {
    GLuint vertShader = compileShader(vertPath, GL_VERTEX_SHADER),
            fragShader = compileShader(fragPath, GL_FRAGMENT_SHADER);
    id = glCreateProgram();
    glAttachShader(id, vertShader);
    glAttachShader(id, fragShader);
    glLinkProgram(id);
    GLint success;
    GLchar infoLog[512];
    glGetProgramiv(id, GL_LINK_STATUS, &success);
    if (not success) {
        glGetProgramInfoLog(id, 512, nullptr, infoLog);
        std::string msg = std::string("Program linking failed: ") + infoLog;
        throw std::runtime_error(msg);
    }
    glDeleteShader(vertShader);
    glDeleteShader(fragShader);
}

std::string Shader::readShaderFile(const std::string &path) {
    std::ifstream shaderFile(path);
    if (not shaderFile) {
        throw std::runtime_error("Error while opening shader file at " + path);
    }
    std::stringstream stringstream;
    stringstream << shaderFile.rdbuf();
    shaderFile.close();
    return stringstream.str();
}

GLuint Shader::compileShader(const std::string &path, GLenum type) {
    std::string code = readShaderFile(path);
    GLuint shader = glCreateShader(type);
    const char* cCode = code.c_str();
    glShaderSource(shader, 1, &cCode, nullptr);
    glCompileShader(shader);
    GLint success;
    GLchar infoLog[512];
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (not success) {
        glGetShaderInfoLog(shader, 512, nullptr, infoLog);
        std::string msg = std::string("Shader compilation failed: ") + path + "\n" + infoLog;
        throw std::runtime_error(msg);
    }
    return shader;
}

GLuint Shader::getLoc(const std::string &name) const {
    return glGetUniformLocation(id, name.c_str());
}

void Shader::use() {
    glUseProgram(id);
}

void Shader::set(const std::string &name, GLboolean value) const {
    glProgramUniform1i(id, getLoc(name), (int) value);
}

void Shader::set(const std::string &name, GLint value) const {
    glProgramUniform1i(id, getLoc(name), value);
}

void Shader::set(const std::string &name, GLfloat value) const {
    glProgramUniform1f(id, getLoc(name), value);
}

void Shader::set(const std::string &name, const std::vector<GLint> &values) const {
    glProgramUniform4iv(id, getLoc(name), 1, values.data());
}

void Shader::set(const std::string &name, const std::vector<GLfloat> &values) const {
    glProgramUniform4fv(id, getLoc(name), 1, values.data());
}

void Shader::set(const std::string &name, const glm::mat4x4 &value) const {
    glProgramUniformMatrix4fv(id, getLoc(name), 1, GL_FALSE, &value[0][0]);
}

void Shader::setf(const std::string &name, const std::vector<GLfloat> &values) const {
    glProgramUniform4fv(id, getLoc(name), 1, values.data());
}
