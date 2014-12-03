#ifndef SAMPLET_HPP
#define SAMPLET_HPP

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <AntTweakBar.h>

#include "shader.hpp"
#include "objloader.hpp"

using namespace glm;
class Cow
{
    TwBar *bar;
    bool wireframe_, colorframe_;
    GLuint vs_, fs_, program_;
    GLuint vs_color_, fs_color_, program_color_;
    GLuint vx_buf_, norm_buf_;
    GLuint vao, vao_color_;
    quat rotation_by_control_;
    float v_, k_;


    std::vector< glm::vec3 > vertices;
    std::vector< glm::vec2 > uvs;
    std::vector< glm::vec3 > normals;

public:
    Cow();
    void init_buffer();
    void init_vertex_array();
    void draw_frame(float time_from_start);

    ~Cow();
private:
    void writeFormulaParam(float time_from_start);
    void handleWireFrame();
    void handleColorFrame(float time_from_start);
};
#endif
