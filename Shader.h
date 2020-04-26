#ifndef SHADER_H
#define SHADER_H

#include <string>
#include <vector>
#include "common.h"

class Shader {
    static std::string readShaderFile(const std::string &path);

    static GLuint compileShader(const std::string &code, GLenum type);

    inline GLuint getLoc(const std::string &name) const;

public:
    GLuint id;

    Shader(const std::string &vertPath, const std::string &fragPath);

    void use();

    void set(const std::string &name, GLboolean value) const;

    void set(const std::string &name, GLint value) const;

    void set(const std::string &name, GLfloat value) const;

    void set(const std::string &name, const std::vector<GLint> &values) const;

    void set(const std::string &name, const std::vector<GLfloat> &values) const;

    void set(const std::string &name, const glm::vec3 &value) const;

    void set(const std::string &name, const glm::mat4x4 &value) const;

    void setf(const std::string &name, const std::vector<GLfloat> &values) const;
};


#endif //SHADER_H
