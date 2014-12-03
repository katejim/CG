#ifndef SHADER_HPP
#define SHADER_HPP

#include <string>
#include <GL/glew.h>

GLuint create_shader(GLenum shader_type, std::string const &file_name);

GLuint create_program(GLuint vs, GLuint fs);

#endif /* end of include guard: SHADER_HPP */


