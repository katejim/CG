#include "shader.hpp"
#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include <chrono>
#include <memory>

using std::string;
using std::ifstream;
using std::cerr;
namespace chrono = std::chrono;
using std::endl;
using std::ios;
using std::vector;
using std::unique_ptr;

GLuint create_shader(GLenum shader_type, string const &file_name) {
    ifstream input_filestream(file_name, ios::binary);

    if (!input_filestream.good())
        throw std::runtime_error(string("Shader source file ") + file_name + " access error");

    input_filestream.seekg(0, std::ios_base::end);
    size_t const size = (size_t) input_filestream.tellg();
    input_filestream.seekg(0, std::ios_base::beg);
    unique_ptr<char[]> text(new char[size + 1]);
    input_filestream.read(text.get(), size);

    text[size] = 0;
    GLchar const *gl_text = text.get();
    GLuint const shader = glCreateShader(shader_type);

    glShaderSource(shader, 1, &gl_text, NULL);
    glCompileShader(shader);

    GLint result;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &result);
    if (!result) {
        GLint info_log_length;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &info_log_length);
        if (info_log_length > 0) {
            string Buffer;
            Buffer.resize((unsigned long) info_log_length);
            glGetShaderInfoLog(shader, info_log_length, NULL, &Buffer[0]);
            throw std::runtime_error(Buffer);
        }
    }

    return shader;
}

GLuint create_program(GLuint vs, GLuint fs) {
    GLuint const program = glCreateProgram();
    glAttachShader(program, vs);
    glAttachShader(program, fs);
    glLinkProgram(program);

    GLint result;
    glGetProgramiv(program, GL_LINK_STATUS, &result);
    if (!result) {
        GLint info_log_length;
        glGetProgramiv(program, GL_INFO_LOG_LENGTH, &info_log_length);
        if (info_log_length > 0) {
            string Buffer;
            Buffer.resize((unsigned long) info_log_length);
            glGetProgramInfoLog(program, info_log_length, NULL, &Buffer[0]);
            throw std::runtime_error(Buffer);
        }
    }
    return program;
}