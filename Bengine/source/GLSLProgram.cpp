#include <fstream>
#include <cstdio>
#include <vector>
#include <print>
#include <string>
#include "GLSLProgram.hpp"

GLSLProgram::GLSLProgram() : _numAttributes(0), _programID(0), _vertexShaderID(0), _fragmentShaderID(0) {

}

GLSLProgram::~GLSLProgram() {

}
void GLSLProgram::use() {
    glUseProgram(_programID);
    for (int i = 0; i < _numAttributes; i++) {
        glEnableVertexAttribArray(i);
    }
}
void GLSLProgram::unuse() {
    glUseProgram(0);
    for (int i = 0; i < _numAttributes; i++) {
        glDisableVertexAttribArray(i);
    }
}

void GLSLProgram::compileShaders(const std::string& vertexShaderPath, const std::string& fragmentShaderPath) {
    _programID = glCreateProgram();
    _vertexShaderID = glCreateShader(GL_VERTEX_SHADER);
    if (_vertexShaderID == 0) {
        fatalError("Vertex Shader Failed to be Created!");
    }

    _fragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);
    if (_fragmentShaderID == 0) {
        fatalError("Fragment Shader Failed to be Created!");
    }

    compile(vertexShaderPath, _vertexShaderID);
    compile(fragmentShaderPath, _fragmentShaderID);
}

void GLSLProgram::compile(const std::string& filePath, GLuint id) {
    //Open and Load Files
    std::ifstream file(filePath);
    if (file.fail()) {
        std::perror(filePath.c_str());
        fatalError("Failed to Open: " + filePath);
    }

    std::string fileContents{};
    std::string line{};

    while (std::getline(file, line)) {
        fileContents += line + "\n";
    }
    file.close();

    //compile shader
    const char* contentsPtr = fileContents.c_str();
    glShaderSource(id, 1, &contentsPtr, nullptr);
    glCompileShader(id);
    GLint success = 0;
    glGetShaderiv(id, GL_COMPILE_STATUS, &success);
    if(success == GL_FALSE) {
        GLint maxLength = 0;
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &maxLength);

        std::vector<GLchar> errorLog(maxLength);
        glGetShaderInfoLog(id, maxLength, &maxLength, &errorLog[0]);
        glDeleteShader(id);
        std::print("{}\n", &errorLog[0]);
        fatalError("Shader " + filePath + "Failed to Compile");
    }

}
void GLSLProgram::addAttribute(const std::string& attributeName) {
    glBindAttribLocation(_programID, _numAttributes, attributeName.c_str());
    _numAttributes++;
}

GLuint GLSLProgram::getUniformLocation(const std::string& uniformName) {
    GLuint location = glGetUniformLocation(_programID, uniformName.c_str());
    if (location == GL_INVALID_INDEX) {
        fatalError("Uniform " + uniformName + " not found in shader");
    }
    return location;
}
void GLSLProgram::linkShaders() {
    // Attach our shaders to our program
    glAttachShader(_programID, _vertexShaderID);
    glAttachShader(_programID, _fragmentShaderID);

    // Link our program
    glLinkProgram(_programID);

    // Note the different functions here: glGetProgram* instead of glGetShader*.
    GLint success = 0;
    glGetProgramiv(_programID, GL_LINK_STATUS, &success);
    if (success == GL_FALSE) {
        GLint maxLength = 0;
        glGetProgramiv(_programID, GL_INFO_LOG_LENGTH, &maxLength);

        // The maxLength includes the NULL character
        std::vector<char> errorLog(maxLength);
        glGetProgramInfoLog(_programID, maxLength, &maxLength, &errorLog[0]);

        // We don't need the program anymore.
        glDeleteProgram(_programID);
        glDeleteShader(_vertexShaderID);
        glDeleteShader(_fragmentShaderID);
        std::print("{}\n", &errorLog[0]);
        fatalError("Shaders Failed to Link");

    }

    glDetachShader(_programID, _vertexShaderID);
    glDetachShader(_programID, _fragmentShaderID);
    glDeleteShader(_vertexShaderID);
    glDeleteShader(_fragmentShaderID);

}

