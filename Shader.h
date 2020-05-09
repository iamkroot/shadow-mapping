#ifndef SHADER_H
#define SHADER_H

#include <string>
#include <vector>
#include "common.h"

/**
 * @brief The class to represent the Shader information
 */
class Shader
{
    /**
     * @brief Method to read the shader file
     * @param path The path to the shader
     */
    static std::string readShaderFile(const std::string &path);

    /**
     * @brief Method to compile a shader file
     * @param code The code of the shader file to be parsed
     * @param type The type of shader file
     */
    static GLuint compileShader(const std::string &code, GLenum type);

    /**
     * @brief Get location of a uniform variable in a shader file
     * @param name The name of the uniform variable to be queried
     */
    inline GLuint getLoc(const std::string &name) const;

public:
    GLuint id;

    /**
     * @brief Constructor of the Shader class
     * @param vertPath Path to the vertex shader
     * @param fragPath Path to the fragment shader
     */
    Shader(const std::string &vertPath, const std::string &fragPath);

    /**
     * @brief Method to call to use a compiler shader
     */
    void use();

    /**
     * @brief Method to set the value of uniform variable
     * @param name Name of the uniform object
     * @param value The value to set it to 
     */
    void set(const std::string &name, GLboolean value) const;

    /**
     * @brief Method to set the value of uniform variable
     * @param name Name of the uniform object
     * @param value The value to set it to 
     */
    void set(const std::string &name, GLint value) const;

    /**
     * @brief Method to set the value of uniform variable
     * @param name Name of the uniform object
     * @param value The value to set it to 
     */
    void set(const std::string &name, GLfloat value) const;

    /**
     * @brief Method to set the value of uniform variable
     * @param name Name of the uniform object
     * @param value The value to set it to 
     */
    void set(const std::string &name, const std::vector<GLint> &values) const;

    /**
     * @brief Method to set the value of uniform variable
     * @param name Name of the uniform object
     * @param value The value to set it to 
     */
    void set(const std::string &name, const std::vector<GLfloat> &values) const;

    /**
     * @brief Method to set the value of uniform variable
     * @param name Name of the uniform object
     * @param value The value to set it to 
     */
    void set(const std::string &name, const glm::vec3 &value) const;

    /**
     * @brief Method to set the value of uniform variable
     * @param name Name of the uniform object
     * @param value The value to set it to 
     */
    void set(const std::string &name, const glm::mat4x4 &value) const;

    /**
     * @brief Method to set the value of uniform variable
     * @param name Name of the uniform object
     * @param value The value to set it to 
     */
    void setf(const std::string &name, const std::vector<GLfloat> &values) const;
};

#endif //SHADER_H
