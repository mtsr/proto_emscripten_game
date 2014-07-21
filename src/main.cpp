#include <stdlib.h>
#include <string>
#include <iostream>

#ifdef EMSCRIPTEN
#include <emscripten/emscripten.h>
#include <GL/glfw.h>
#else
#include <GLFW/glfw.h>
#endif

#include "main.h"

int main(int argc, const char * argv[])
{
    const GLint WIDTH = 640;
    const GLint HEIGHT = 480;
    
    // Set GLFW error callback before glfwInit
    glfwSetErrorCallback(glfw_error_callback);

    // Initialise GLFW
    if( !glfwInit() )
    {
        std::cout << "Failed to initialize GLFW" << std::endl;
        exit(EXIT_FAILURE);
    }

    // ensure newest opengl on mac os x
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    
    glfwWindowHint(GLFW_SAMPLES, 4);
    
    GLFWwindow* window;
    if (false) { // fullscreen
        GLFWmonitor* mon = glfwGetPrimaryMonitor();
        const GLFWvidmode* vmode = glfwGetVideoMode(mon);
        window = glfwCreateWindow(vmode->width, vmode->height, "Extended GL Init", mon, NULL);
    } else {
        window = glfwCreateWindow(WIDTH, HEIGHT, "Extended GL Init", NULL, NULL);
    }
    
    if (!window)
    {
        glfwTerminate();
        std::cout << "Failed to create window" << std::endl;
        exit(EXIT_FAILURE);
    }
    
    glfwMakeContextCurrent(window);
    
    glfwSetWindowCloseCallback(window, glfw_windowClose_callback);
    glfwSetKeyCallback(window, glfw_key_callback);

    // get version info
    const GLubyte* renderer = glGetString(GL_RENDERER); // get renderer string
    const GLubyte* version = glGetString(GL_VERSION); // version as a string
    std::cout << "Renderer: " << renderer << std::endl;
    std::cout << "OpenGL version supported " << version << std::endl;
    
    glfwSetWindowSizeCallback(window, glfw_windowSize_callback);

    while (!glfwWindowShouldClose(window))
    {
        glfwSwapBuffers(window);
        glfwPollEvents();
	}

    glfwDestroyWindow(window);
    glfwTerminate();
    
    return EXIT_SUCCESS;
}

static void glfw_error_callback(int error, const char* description)
{
    std::cerr << description << std::endl;
}

static void glfw_windowClose_callback(GLFWwindow* window) {
    std::cout << "Window closing" << std::endl;
}

static void glfw_key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);
}

static void glfw_windowSize_callback (GLFWwindow* window, int width, int height) {
    /* update any perspective matrices used here */
}
