#include "triangleCow.h"

Cow::Cow() : wireframe_(false), k_(8), v_(1), colorframe_(false) {
    TwInit(TW_OPENGL_CORE, NULL);

    bar = TwNewBar("Parameters");
    TwDefine("Parameters size='500 180' color='70 100 120' valueswidth=220 iconpos=topleft");

    TwAddVarRW(bar, "Wireframe mode", TW_TYPE_BOOLCPP, &wireframe_, " true='ON' false='OFF' key=w ");
    TwAddVarRW(bar, "Color mode", TW_TYPE_BOOLCPP, &colorframe_, " true='ON' false='OFF' key=c ");
    TwAddVarRW(bar, "K param", TW_TYPE_FLOAT, &k_, "");
    TwAddVarRW(bar, "V param", TW_TYPE_FLOAT, &v_, "");


    TwAddVarRW(bar, "ObjRotation", TW_TYPE_QUAT4F, &rotation_by_control_,
               " label='Object orientation' opened=true help='Change the object orientation.' ");

    vs_ = create_shader(GL_VERTEX_SHADER, "vs1.glsl");
    fs_ = create_shader(GL_FRAGMENT_SHADER, "fs1.glsl");
    program_ = create_program(vs_, fs_);

    vs_color_ = create_shader(GL_VERTEX_SHADER, "vs2.glsl");
    fs_color_ = create_shader(GL_FRAGMENT_SHADER, "fs2.glsl");
    program_color_ = create_program(vs_color_, fs_color_);

    OBJLoader::loadOBJ("model.obj", vertices, uvs, normals);

    init_buffer();
    init_vertex_array();
}

void Cow::writeFormulaParam(float time_from_start){
    vec3 center = vec3(0,0,0);
    int size = vertices.size();
    for (int i =0 ; i != size; ++i)
        center+=vertices[i];

    center /= size;

    float dist = 0;
    for (vec3 el:vertices)
        dist = glm::max(dist, glm::length(el - center));

    glUseProgram(program_color_);

    glUniformMatrix3fv(glGetUniformLocation(program_color_, "center"), 1, GL_FALSE, &center[0]);

    glUniform1i(glGetUniformLocation(program_color_, "size"), (GLint)size);

    glUniform1f(glGetUniformLocation(program_color_, "k"), (GLfloat)k_);

    glUniform1f(glGetUniformLocation(program_color_, "v"), (GLfloat)v_);

    glUniform1f(glGetUniformLocation(program_color_, "dist"), (GLfloat)dist);

    glUniform1f(glGetUniformLocation(program_color_, "time"), (GLfloat)time_from_start);
}

void Cow::init_buffer() {
    //generate opengl buffer & save pointr to vertexbuffer
    glGenBuffers(1, &vx_buf_);
    glBindBuffer(GL_ARRAY_BUFFER, vx_buf_);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), &vertices[0], GL_STATIC_DRAW);

    glGenBuffers(1, &norm_buf_);
    glBindBuffer(GL_ARRAY_BUFFER, norm_buf_);
    glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(glm::vec3), &normals[0], GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Cow::draw_frame(float time_from_start) {
    handleWireFrame();
    handleColorFrame(time_from_start);
    glDrawArrays(GL_TRIANGLES, 0, 3 * vertices.size());

    if (wireframe_)
        glDisable(GL_POLYGON_OFFSET_LINE);
}

void Cow::handleWireFrame(){
    if (wireframe_) {
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        glEnable(GL_POLYGON_OFFSET_LINE);
        glPolygonOffset(-1, 0);
    }
    else {
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        glClearColor(0.2f, 0.2f, 0.2f, 1);
        glClearDepth(1);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }
}

void Cow::handleColorFrame(float time_from_start){
    float const w = (float) 800;
    float const h = (float) 800;
    // строим матрицу проекции с aspect ratio (отношением сторон) таким же, как у окна
    mat4 const proj = perspective(45.0f, w / h, 0.1f, 100.0f);
    // преобразование из СК мира в СК камеры
    mat4 const view = lookAt(vec3(0, 0, 16), vec3(0, 0, 0), vec3(0, 1, 0));
    mat4 const modelview = view * mat4_cast(rotation_by_control_);
    mat4 const mvp = proj * modelview;

    GLuint program;
    if (colorframe_) {
        program = program_color_;
        writeFormulaParam(time_from_start);
        glBindVertexArray(vao_color_);
    }
    else {
        program = program_;
        GLuint const model_location = glGetUniformLocation(program, "model");
        glUniformMatrix4fv(model_location, 1, GL_FALSE, &modelview[0][0]);
        glBindVertexArray(vao);
    }

    glUseProgram(program);
    glUniformMatrix4fv(glGetUniformLocation(program, "mvp"), 1, GL_FALSE, &mvp[0][0]);
    glUniform1ui(glGetUniformLocation(program, "wireframe"), wireframe_);
}

void Cow::init_vertex_array() {
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    glBindBuffer(GL_ARRAY_BUFFER, vx_buf_);
    GLuint pos_location = glGetAttribLocation(program_, "in_pos");
    glVertexAttribPointer(pos_location, 3, GL_FLOAT, GL_FALSE, sizeof(vec3), 0);
    glEnableVertexAttribArray(pos_location);


    glBindBuffer(GL_ARRAY_BUFFER, norm_buf_);
    GLuint norm_location = glGetAttribLocation(program_, "in_norm");
    glVertexAttribPointer(norm_location, 3, GL_FLOAT, GL_FALSE, sizeof(vec3), 0);
    glEnableVertexAttribArray(norm_location);

    glGenVertexArrays(1, &vao_color_);
    glBindVertexArray(vao_color_);

    glBindBuffer(GL_ARRAY_BUFFER, vx_buf_);
    GLuint pos_color_location = glGetAttribLocation(program_color_, "in_pos");
    glVertexAttribPointer(pos_color_location, 3, GL_FLOAT, GL_FALSE, sizeof(vec3), 0);
    glEnableVertexAttribArray(pos_color_location);

    glBindVertexArray(0);
}

Cow::~Cow() {
    glDeleteProgram(program_);
    glDeleteShader(vs_);
    glDeleteShader(fs_);
    glDeleteVertexArrays(1, &vao);
    glDeleteVertexArrays(1, &vao_color_);
    glDeleteBuffers(1, &vx_buf_);
    glDeleteBuffers(1, &norm_buf_);

    TwDeleteAllBars();
    TwTerminate();
}
