#ifndef SAMPLET_HPP
#define SAMPLET_HPP

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <AntTweakBar.h>

#include "shader.hpp"
#include "objloader.hpp"
#include "state.h"

using namespace glm;
class Cow
{
    TwBar *bar;
    bool wireframe_, nice_filtration_;
    GLuint vs_, fs_, program_;
    GLuint vx_buf_, norm_buf_,uv_buffer_;
    GLuint vao;
    quat rotation_by_control_;
    STATE state;

     float q;

    GLuint Texture, TextureID;


    std::vector< glm::vec3 > vertices;
    std::vector< glm::vec2 > uvs;
    std::vector< glm::vec3 > normals;
    int vertexCount;

public:
    Cow(STATE state);
    void init_buffer();
    void init_vertex_array();
    void draw_frame(float time_from_start);

    ~Cow();
private:
    void writeFormulaParam(float time_from_start);
    void handleWireFrame();
    void handleColorFrame(float time_from_start);
    GLuint loadTexture(const char * imagepath);
    void init_texture_coordinates();
};
#endif
