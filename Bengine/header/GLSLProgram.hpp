#ifndef GLSLPROGRAM_HPP
#define GLSLPROGRAM_HPP
#include <GL/glew.h>
#include <string>
#include "Error.hpp"

class GLSLProgram {

public:

    GLSLProgram();
    ~GLSLProgram();

    void compileShaders(const std::string& vertexShaderPath, const std::string& fragmentShaderPath);
    void linkShaders();
    void addAttribute(const std::string& attributeName);
    GLuint getUniformLocation(const std::string& uniformName);

    void use();
    void unuse();

private:
    void compile(const std::string& filePath, GLuint id);
    int _numAttributes;
    GLuint _programID;
    GLuint _vertexShaderID;
    GLuint _fragmentShaderID;

};

#endif //GLSLPROGRAM_HPP
