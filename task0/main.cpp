#include <stdio.h>
#include <stdlib.h>
#include <chrono>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <AntTweakBar.h>
#include <memory>
#include <iostream>

#include "triangle.h"

using std::chrono::system_clock;
using std::chrono::duration;
using namespace std;

void mouse_buttons_callback(GLFWwindow *, int button, int action, int mods) {
    TwEventMouseButtonGLFW(button, action);
}

void mouse_position_callback(GLFWwindow *, double x, double y) {
    TwEventMousePosGLFW((int) x, (int) y);
}

void mouse_scroll_callback(GLFWwindow *window, double x, double y) {
    TwEventMouseWheelGLFW((int) y);
}

void framebuffer_size_callback(GLFWwindow *window, int width, int height) {
    if (width <= 0 || height <= 0)
        return;

    glViewport(0, 0, width, height);
    TwWindowSize(width, height);
}

std::unique_ptr<Triangle> g_sample;

void draw_func() {
    static system_clock::time_point const start = system_clock::now();

    g_sample->draw_frame(duration<float>(system_clock::now() - start).count());

    TwDraw();
}

string usage[3] = {"simple", "screen", "notscreen"};

STATE handleCommanLineParam(std::string progName, std::string param){
    STATE state;
    string simple = "simple";
    string screen = "screen";
    string notscreen = "notscreen";
    if (param == usage[0]){
        state = SIMPLE;
    } else  if (param == usage[1]){
        state = SCREEN;
    } else if (param == usage[2]){
        state = NOTSCREEN;
    } else {
        cout << "wrong command line param; try on of this:" <<endl;
        state = INVALID;
        cout <<progName <<" "<< simple<< endl
                      << progName << " "<< screen <<endl
                      << progName <<" " <<  notscreen <<endl;
    }
    return state;
}

void printUsage(string progName){
    for (int  i = 0 ; i != 3; ++i){
        cout << progName << usage[i] <<endl;
    }
}
int main(int argc, char *argv[]){
    string progName = std::string(argv[0]);

    if (argc < 2) {
        string simple = "simple";
        string screen = "screen";
        string notscreen = "notscreen";

        cout << "wrong command line param; try on of this:" <<endl;
        printUsage(std::string(argv[0]));
        exit(EXIT_FAILURE);
    }

    STATE state;

    state = handleCommanLineParam(std::string(argv[0]), std::string(argv[1]));
    if (state == INVALID)
        exit(EXIT_FAILURE);


    float const default_width = 800;
    float const default_height = 800;
    char * default_title = "spinning triangle";


    if(!glfwInit()){
        cout << "Failed to initialize GLFW" <<endl;
        return -1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(default_width, default_height, default_title, NULL, NULL);
    if (!window) {
        glfwTerminate();
        return -1;
    }
    // Make the window's context current
    glfwMakeContextCurrent(window);

    // GLEW Initalize
    glewExperimental=true;
    if (glewInit() != GLEW_OK) {
        cout << "can't initialize GLEW" <<endl;
        return -1;
    }
    try {
        g_sample.reset(new Triangle(state));
    }
    catch (std::exception const &except) {
        std::cout << except.what() << std::endl;
        exit(EXIT_FAILURE);
    }

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetMouseButtonCallback(window, mouse_buttons_callback);
    glfwSetCursorPosCallback(window, mouse_position_callback);
    glfwSetScrollCallback(window, mouse_scroll_callback);

    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glEnable(GL_DEPTH_TEST);
    framebuffer_size_callback(window, default_width, default_height);
    while (!glfwWindowShouldClose(window))
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        /* Render here */

        draw_func();
        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
