#include <stdio.h>
#include <stdlib.h>
#include <chrono>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <AntTweakBar.h>
#include <memory>
#include <iostream>


#include "task2.h"
#include "state.h"

using std::chrono::system_clock;
using std::chrono::duration;
using namespace std;

float g_Scale           = 1.0f;
float g_MouseWheelScale = 0.0f;

void mouse_buttons_callback(GLFWwindow *, int button, int action, int mods)
{
    TwEventMouseButtonGLFW(button, action);
}

void mouse_position_callback(GLFWwindow *, double x, double y)
{
    TwEventMousePosGLFW((int) x, (int) y);
}

void mouse_scroll_callback(GLFWwindow * window, double x, double y)
{
    TwEventMouseWheelGLFW((int) y);
    g_MouseWheelScale = y;
}

void framebuffer_size_callback(GLFWwindow * window, int width, int height)
{
    if (width <= 0 || height <= 0)
        return;

    glViewport(0, 0, width, height);
    TwWindowSize(width, height);
}

std::unique_ptr<Task2> g_sample;

void draw_func(float value)
{
    static system_clock::time_point const start = system_clock::now();
    g_sample->draw_frame(duration<float>(system_clock::now() - start).count(), value);
    TwDraw();
}

string usage[3] = {"quad", "sphere", "cylindre"};

void printUsage(string progName)
{
    for (int  i = 0 ; i != 3; ++i)
    {
        cout << progName << usage[i] << endl;
    }
}

STATE handleCommanLineParam(std::string progName, std::string param)
{
    STATE state;

    if (param == usage[0])
    {
        state = QUAD;
    }
    else  if (param == usage[1])
    {
        state = SPHERE;
    }
    else if (param == usage[2])
    {
        state = CYLINDER;
    }
    else
    {
        cout << "wrong command line param; try on of this:" << endl;
        state = INVALID;
        printUsage(progName);
    }

    return state;
}

int main(int argc, char * argv[])
{
    string progName = std::string(argv[0]);

    if (argc < 2)
    {
        cout << "wrong command line param; try on of this:" << endl;
        printUsage(std::string(argv[0]));
        exit(EXIT_FAILURE);
    }

    STATE state;
    state = handleCommanLineParam(std::string(argv[0]), std::string(argv[1]));

    if (state == INVALID)
        exit(EXIT_FAILURE);

    if(!glfwInit())
    {
        cout << "Failed to initialize GLFW" << endl;
        return -1;
    }

    float const default_width = 800;
    float const default_height = 800;
    char * default_title = "Cow";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    GLFWwindow * window = glfwCreateWindow(default_width, default_height, default_title, NULL, NULL);

    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    // Make the window's context current
    glfwMakeContextCurrent(window);
    // GLEW Initalize
    glewExperimental = true;

    if (glewInit() != GLEW_OK)
    {
        cout << "can't initialize GLEW" << endl;
        return -1;
    }

    try
    {
        g_sample.reset(new Task2(state, window));
    }
    catch (std::exception const & except)
    {
        std::cout << except.what() << std::endl;
        exit(EXIT_FAILURE);
    }

//    glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
//    glfwSetCursorPos(window, default_width/2, default_height/2);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetMouseButtonCallback(window, mouse_buttons_callback);
    glfwSetCursorPosCallback(window, mouse_position_callback);
    glfwSetScrollCallback(window, mouse_scroll_callback);
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    framebuffer_size_callback(window, default_width, default_height);

    while (!glfwWindowShouldClose(window))
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        /* Render here */
        draw_func(g_MouseWheelScale);
        g_MouseWheelScale = 0;
        /* Swap front and back buffers */
        glfwSwapBuffers(window);
        /* Poll for and process events */
        glfwPollEvents();
    }

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
