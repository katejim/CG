#ifndef SAMPLET_HPP
#define SAMPLET_HPP

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <AntTweakBar.h>

#include "shader.hpp"
#include "objloader.hpp"
#include "state.h"

#include "tangentspace.hpp"

#include <GLFW/glfw3.h>

using namespace glm;
class Task2
{
    TwBar * bar;
    bool wireframe_, nice_filtration_;
    GLuint vs_, fs_, program_;
    GLuint vx_buf_, norm_buf_, uv_buffer_, tangentbuffer, bitangentbuffer, elementbuffer;
    GLuint vao;
    quat rotation_by_control_;
    STATE state;

    float q;

    GLuint Texture, TextureID;
    GLuint TextureNormal, TextureIDNormal;

    GLuint LightID;

    double dir[3];
    float lightColor[3];
    float ambient[3];
    float specular[3];
    float power;
    int param;
    float specular_power;
    GLFWwindow * window;

    std::vector< glm::vec3 > vertices;
    std::vector< glm::vec2 > uvs;
    std::vector< glm::vec3 > normals;

    std::vector<glm::vec3> tangents;
    std::vector<glm::vec3> bitangents;

    int vertexCount;
    float zoomValue;
  public:
    Task2(STATE state, GLFWwindow * window);
    void init_buffer();
    void init_vertex_array();
    void draw_frame(float time_from_start, float zoomValue);

    ~Task2();
  private:
    void writeFormulaParam(float time_from_start, float zoomValue);
    void handleWireFrame();
    void handleColorFrame(float time_from_start, float zoomValue);
    GLuint loadTexture(const char * imagepath);
    void init_texture();
    void initTweakBar();

};
#endif
