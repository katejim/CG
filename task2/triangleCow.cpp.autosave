#include "triangleCow.h"

Cow::Cow(STATE state_m) : state(state_m), wireframe_(false), nice_filtration_(false) , q(1.0){
    TwInit(TW_OPENGL_CORE, NULL);

    bar = TwNewBar("Parameters");
    TwDefine("Parameters size='500 180' color='70 100 120' valueswidth=220 iconpos=topleft");

    TwAddVarRW(bar, "Wireframe mode", TW_TYPE_BOOLCPP, &wireframe_, " true='ON' false='OFF' key=w ");

    TwAddVarRW(bar, "Nice filtration", TW_TYPE_BOOLCPP, &nice_filtration_, " true='ON' false='OFF' key=W ");


    TwAddVarRW(bar, "ObjRotation", TW_TYPE_QUAT4F, &rotation_by_control_,
               " label='Object orientation' opened=true help='Change the object orientation.' ");

    TwAddVarRW(bar, "Q param", TW_TYPE_FLOAT, &q, "");


    vs_ = create_shader(GL_VERTEX_SHADER, "vs1.glsl");
    fs_ = create_shader(GL_FRAGMENT_SHADER, "fs1.glsl");
    program_ = create_program(vs_, fs_);

    string name = state == SPHERE ? "sphere.obj" : "cylinder.obj";
    OBJLoader::loadOBJ(name.c_str(), vertices, uvs, normals);

    init_buffer();
    init_vertex_array();
    init_texture_coordinates();
}


void Cow::init_buffer() {
    //generate opengl buffer & save pointr to vertexbuffer
    glGenBuffers(1, &vx_buf_);
    glBindBuffer(GL_ARRAY_BUFFER, vx_buf_);
    if (state == QUAD){
        vec3 const data[6] =
        {
            // Left bottom triangle
            vec3(-1.0f, 1.0f, 0.0f), vec3(-1.0f, -1.0f, 0.0f), vec3(1.0f, -1.0f, 0.0f),
            // Right top triangle
            vec3(1.0f, -1.0f, 0.0f), vec3(1.0f, 1.0f, 0.0f), vec3(-1.0f, 1.0f, 0.0f)
        };
        glBufferData(GL_ARRAY_BUFFER, sizeof(vec3) * 6, data, GL_STATIC_DRAW);
        vertexCount = sizeof(vec3) * 6;
    } else {
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), &vertices[0], GL_STATIC_DRAW);
        vertexCount = vertices.size() * sizeof(glm::vec3);
    }

    glGenBuffers(1, &uv_buffer_);
    glBindBuffer(GL_ARRAY_BUFFER, uv_buffer_);

    if (state == QUAD){
        vec2  const data[6] = {
            // Left bottom triangle
            vec2(0.0f, 0.0f), vec2(0.0f, 1.0f), vec2(1.0f, 1.0f),
            vec2(1.0f, 1.0f), vec2(1.0f, 0.0f), vec2(0.0f, 0.0f)
        };
        glBufferData(GL_ARRAY_BUFFER, sizeof(vec2) * 6, data, GL_STATIC_DRAW);
    } else {
        glBufferData(GL_ARRAY_BUFFER, uvs.size() * sizeof(glm::vec3), &uvs[0], GL_STATIC_DRAW);
    }

    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Cow::init_texture_coordinates(){
    Texture = loadTexture("texture.bmp");
    TextureID  = glGetUniformLocation(program_, "myTextureSampler");
}

void Cow::draw_frame(float time_from_start) {
    handleWireFrame();
    handleColorFrame(time_from_start);
    glDrawArrays(GL_TRIANGLES, 0, vertexCount);

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
    mat4 const view = lookAt(vec3(0, 0, 6), vec3(0, 0, 0), vec3(0, 1, 0));
    mat4 const modelview = view * mat4_cast(rotation_by_control_);
    mat4 const mvp = proj * modelview;

    GLuint program;

    program = program_;
    GLuint const model_location = glGetUniformLocation(program, "model");
    glUniformMatrix4fv(model_location, 1, GL_FALSE, &modelview[0][0]);
    glBindVertexArray(vao);


    if (!nice_filtration_){
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    } else {
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glGenerateMipmap(GL_TEXTURE_2D);
    }

    glUseProgram(program_);
    glUniformMatrix4fv(glGetUniformLocation(program_, "mvp"), 1, GL_FALSE, &mvp[0][0]);
    glUniform1ui(glGetUniformLocation(program_, "wireframe"), wireframe_);
    glUniform1f(glGetUniformLocation(program_, "q"), (GLfloat)q);

    //Bind our texture in Texture Unit 0
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, Texture);

    // Set our "myTextureSampler" sampler to user Texture Unit 0
    glUniform1i(TextureID, 0);
}

void Cow::init_vertex_array() {
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    glBindBuffer(GL_ARRAY_BUFFER, vx_buf_);
    GLuint pos_location = glGetAttribLocation(program_, "in_pos");
    glVertexAttribPointer(pos_location, 3, GL_FLOAT, GL_FALSE, sizeof(vec3), 0);
    glEnableVertexAttribArray(pos_location);


//    glBindBuffer(GL_ARRAY_BUFFER, norm_buf_);
//    GLuint norm_location = glGetAttribLocation(program_, "in_norm");
//    glVertexAttribPointer(norm_location, 3, GL_FLOAT, GL_FALSE, sizeof(vec3), 0);
//    glEnableVertexAttribArray(norm_location);

    glBindBuffer(GL_ARRAY_BUFFER, uv_buffer_);
    GLuint const pos_location_texture = (GLuint const) glGetAttribLocation(program_, "vertexUV");
    glVertexAttribPointer(pos_location_texture, 2, GL_FLOAT, GL_FALSE, sizeof(vec2), 0);
    glEnableVertexAttribArray(pos_location_texture);


    glBindVertexArray(0);
}

Cow::~Cow() {
    glDeleteProgram(program_);
    glDeleteShader(vs_);
    glDeleteShader(fs_);
    glDeleteVertexArrays(1, &vao);
    glDeleteBuffers(1, &vx_buf_);
    glDeleteBuffers(1, &norm_buf_);

    TwDeleteAllBars();
    TwTerminate();
}


GLuint Cow::loadTexture(const char * imagepath){
    printf("Reading image %s\n", imagepath);

    // Data read from the header of the BMP file
    unsigned char header[54];
    unsigned int dataPos;
    unsigned int imageSize;
    unsigned int width, height;
    // Actual RGB data
    unsigned char * data;

    // Open the file
    FILE * file = fopen(imagepath,"rb");
    if (!file)							    {printf("%s could not be opened. Are you in the right directory ? Don't forget to read the FAQ !\n", imagepath); getchar(); return 0;}

    // Read the header, i.e. the 54 first bytes

    // If less than 54 bytes are read, problem
    if ( fread(header, 1, 54, file)!=54 ){
        printf("Not a correct BMP file\n");
        return 0;
    }
    // A BMP files always begins with "BM"
    if ( header[0]!='B' || header[1]!='M' ){
        printf("Not a correct BMP file\n");
        return 0;
    }
    // Make sure this is a 24bpp file
    if ( *(int*)&(header[0x1E])!=0  )         {printf("Not a correct BMP file\n");    return 0;}
    if ( *(int*)&(header[0x1C])!=24 )         {printf("Not a correct BMP file\n");    return 0;}

    // Read the information about the image
    dataPos    = *(int*)&(header[0x0A]);
    imageSize  = *(int*)&(header[0x22]);
    width      = *(int*)&(header[0x12]);
    height     = *(int*)&(header[0x16]);

    // Some BMP files are misformatted, guess missing information
    if (imageSize==0)    imageSize=width*height*3; // 3 : one byte for each Red, Green and Blue component
    if (dataPos==0)      dataPos=54; // The BMP header is done that way

    // Create a buffer
    data = new unsigned char [imageSize];

    // Read the actual data from the file into the buffer
    fread(data,1,imageSize,file);

    // Everything is in memory now, the file wan be closed
    fclose (file);

    // Create one OpenGL texture
    GLuint textureID;
    glGenTextures(1, &textureID);

    // "Bind" the newly created texture : all future texture functions will modify this texture
    glBindTexture(GL_TEXTURE_2D, textureID);

    // Give the image to OpenGL
    glTexImage2D(GL_TEXTURE_2D, 0,GL_RGB, width, height, 0, GL_BGR, GL_UNSIGNED_BYTE, data);

    // OpenGL has now copied the data. Free our own version
    delete [] data;

    // Return the ID of the texture we just created
    return textureID;
}
