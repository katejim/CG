#ifndef SAMPLET_HPP
#define SAMPLET_HPP

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <AntTweakBar.h>

#include "shader.hpp"
#include "state.h"

using namespace glm;
class sample_t
{
    TwBar *bar;
    bool wireframe_;
    GLuint vs_, fs_, program_;
    GLuint vx_buf_;
    GLuint vao_;
    quat rotation_by_control_;
    STATE state;
public:
    sample_t(STATE state);
    void init_buffer();
    void init_vertex_array();
    void draw_frame(float time_from_start);
    ~sample_t();
};
#endif
