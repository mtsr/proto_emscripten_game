//
//  RenderSystem.cpp
//  CrossPlatform
//
//  Created by Jonas Matser on 27-07-14.
//
//

#include "RenderSystem.h"

#include <glm/gtc/type_ptr.hpp>

#include "../components/Camera.h"
#include "../components/Transform.h"
#include "../components/Sprite.h"

RenderSystem::RenderSystem() {
    GLint viewport[4];
    glGetIntegerv(GL_VIEWPORT, viewport);
    
    width = viewport[2];
    height = viewport[3];
}

void RenderSystem::configure(entityx::EventManager &event_manager) {
    std::cout << "Configuring RenderSystem" << std::endl;

    event_manager.subscribe<ResizeEvent>(*this);
    
//    glEnable(GL_DEPTH_TEST); // enable depth-testing
//    glDepthFunc(GL_LESS); // depth-testing interprets a smaller value as "closer"
//    glEnable (GL_CULL_FACE); // cull face
//    glCullFace (GL_BACK); // cull back face
    glFrontFace (GL_CW); // GL_CCW for counter clock-wise
}

void print_error() {
    bool doExit = false;
    GLenum error = glGetError();
    while (error != GL_NO_ERROR) {
        switch (error) {
            case GL_INVALID_ENUM:
                std::cout << "GL_INVALID_ENUM" << std::endl;
                doExit = true;
                break;
            case GL_INVALID_VALUE:
                std::cout << "GL_INVALID_VALUE" << std::endl;
                doExit = true;
                break;
            case GL_INVALID_OPERATION:
                std::cout << "GL_INVALID_OPERATION" << std::endl;
                doExit = true;
                break;
            case GL_INVALID_FRAMEBUFFER_OPERATION:
                std::cout << "GL_INVALID_FRAMEBUFFER_OPERATION" << std::endl;
                doExit = true;
                break;
            case GL_OUT_OF_MEMORY:
                std::cout << "GL_OUT_OF_MEMORY" << std::endl;
                doExit = true;
                break;
        }
        error = glGetError();
    }
    if (doExit) {
        exit(1);
    }
}

void RenderSystem::update(entityx::EntityManager &es, entityx::EventManager &events, double dt) {
    
    entityx::ComponentHandle<Camera> camera;
    for (entityx::Entity entity : es.entities_with_components(camera)) {
        if (resized) {
            camera->mWidth = width;
            camera->mHeight = height;
            resized = false;
        }
        glClearColor(.0f, .0f, .0f, .0f);
        glClear(
            GL_COLOR_BUFFER_BIT
//          | GL_DEPTH_BUFFER_BIT
        );
        
        glm::mat4 projection = camera->getProjection();
        glm::mat4 view = camera->getView();
        
        entityx::ComponentHandle<Sprite> sprite;
        entityx::ComponentHandle<Transform> transform;
        for (entityx::Entity entity : es.entities_with_components(sprite, transform)) {
            // TODO remove
            transform->rotate(dt * 10, glm::vec3(0.f, 1.f, 0.f));
            if (!Sprite::initialized) {
                GLfloat points[] = {
                    -0.5f,  0.5f,  0.0f,
                    0.5f, -0.5f,  0.0f,
                    -0.5f, -0.5f,  0.0f,
                    -0.5f,  0.5f,  0.0f,
                    0.5f, 0.5f,  0.0f,
                    0.5f, -0.5f,  0.0f
                };
                
                glGenBuffers(1, &(Sprite::vertexBufferObject));
                print_error();
                glBindBuffer(GL_ARRAY_BUFFER, Sprite::vertexBufferObject);
                print_error();
                glBufferData(GL_ARRAY_BUFFER, 6*3 * sizeof (GLfloat), &points[0], GL_STATIC_DRAW);
                print_error();
                
                Sprite::shaderProgram = loadShaders();
                
#ifndef EMSCRIPTEN
                GLuint vertexArrayObject;
                glGenVertexArrays(1, &vertexArrayObject);
                print_error();
                glBindVertexArray(vertexArrayObject);
                print_error();
#endif
                
                const char* attribute_name = "vertexposition";
                Sprite::attribute_position = glGetAttribLocation(Sprite::shaderProgram, attribute_name);
                print_error();
                if (Sprite::attribute_position == -1) {
                    std::cout << "Could not bind attribute " << attribute_name << std::endl;
                    exit(1);
                }
                
//                const char* uniform_name;
//                uniform_name = "modelview";
//                Sprite::uniform_modelview = glGetUniformLocation(Sprite::shaderProgram, uniform_name);
//                print_error();
//                if (Sprite::uniform_modelview == -1) {
//                    std::cout << "Could not bind uniform " << uniform_name << std::endl;
//                    exit(1);
//                }
//                
//                uniform_name = "projection";
//                Sprite::uniform_projection = glGetUniformLocation(Sprite::shaderProgram, uniform_name);
//                print_error();
//                if (Sprite::uniform_projection == -1) {
//                    std::cout << "Could not bind uniform " << uniform_name << std::endl;
//                    exit(1);
//                }
                
                glVertexAttribPointer(
                    Sprite::attribute_position, // attribute
                    3,                 // number of elements per vertex, here (x,y,z)
                    GL_FLOAT,          // the type of each element
                    GL_FALSE,          // take our values as-is
                    0,                 // no extra data between each position
                    0                  // offset of first element
                );
                print_error();

                glBindBuffer(GL_ARRAY_BUFFER, 0);
                print_error();
                
                Sprite::initialized = true;

//                // TODO call
//                glDeleteProgram(shaderProgram);
//                glDeleteBuffers(1, &vertexBufferObject);
            }
            
            transform->transform = glm::mat4(1.f);
            glm::mat4 modelview = view * transform->transform;

            glUseProgram(Sprite::shaderProgram);
            print_error();
            glBindBuffer(GL_ARRAY_BUFFER, Sprite::vertexBufferObject);
            print_error();
            glEnableVertexAttribArray(Sprite::attribute_position);
            print_error();
//            glUniformMatrix4fv(Sprite::uniform_modelview, 1, GL_FALSE, glm::value_ptr(modelview));
//            print_error();
//            glUniformMatrix4fv(Sprite::uniform_projection, 1, GL_FALSE, glm::value_ptr(projection));
//            print_error();
            glDrawArrays(GL_TRIANGLES, 0, 6);
            print_error();
            glDisableVertexAttribArray(Sprite::attribute_position);
            print_error();
            glBindBuffer(GL_ARRAY_BUFFER, 0);
            print_error();
        }

    }
}

void RenderSystem::receive(const ResizeEvent &resizeEvent) {
    resized = true;
    width = resizeEvent.width;
    height = resizeEvent.height;
}

void RenderSystem::print_log(GLuint object)
{
    GLint log_length = 0;
    if (glIsShader(object)) {
        print_error();
        glGetShaderiv(object, GL_INFO_LOG_LENGTH, &log_length);
        print_error();
    } else if (glIsProgram(object)) {
        print_error();
        glGetProgramiv(object, GL_INFO_LOG_LENGTH, &log_length);
        print_error();
    } else {
        std::cout << "printlog: Not a shader or a program" << std::endl;
        return;
    }
    
    char* log = (char*)malloc(log_length);
    
    if (glIsShader(object)) {
        print_error();
        glGetShaderInfoLog(object, log_length, NULL, log);
        print_error();
    } else if (glIsProgram(object)) {
        print_error();
        glGetProgramInfoLog(object, log_length, NULL, log);
        print_error();
    }
    std::cout << log << std::endl;
    free(log);
}

GLuint RenderSystem::loadShaders()
{
    // Create the shaders
    GLuint vertexShaderID = glCreateShader(GL_VERTEX_SHADER);
    print_error();
    GLuint fragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);
    print_error();
    
    // Read the Vertex Shader code from the file
    std::string vertexShaderCode = "#version 100\n"
    "precision mediump float;\n"
    "attribute vec3 vertexposition;\n"
    "\n"
//    "uniform mat4 modelview;\n"
//    "uniform mat4 projection;\n"
    "\n"
    "void main () {\n"
//    "    gl_Position = projection * modelview * vec4(vertexposition, 1.0);\n"
    "    gl_Position = vec4(vertexposition, 1.0);\n"
    "}";
    
    // Read the Fragment Shader code from the file
    std::string fragmentShaderCode = "#version 100\n"
    "precision mediump float;\n"
    "\n"
    "void main () {\n"
    "    gl_FragColor = vec4 (1.0, 0.0, 1.0, 0.5);\n"
    "}";
    
    GLint result = GL_FALSE;
    
    // Compile Vertex Shader
    std::cout << "Compiling vertex shader" << std::endl;
    char const * vertexSourcePointer = vertexShaderCode.c_str();
    glShaderSource(vertexShaderID, 1, &vertexSourcePointer , NULL);
    print_error();
    glCompileShader(vertexShaderID);
    print_error();
    
    // Check Vertex Shader
    glGetShaderiv(vertexShaderID, GL_COMPILE_STATUS, &result);
    print_error();
    if (result != GL_TRUE)
    {
        std::cout << "Failed to compile vertex shader" << std::endl;
        print_log(vertexShaderID);
        exit(EXIT_FAILURE);
    }
    
    // Compile Fragment Shader
    std::cout << "Compiling fragment shader" << std::endl;
    char const * fragmentSourcePointer = fragmentShaderCode.c_str();
    glShaderSource(fragmentShaderID, 1, &fragmentSourcePointer , NULL);
    print_error();
    glCompileShader(fragmentShaderID);
    print_error();
    
    // Check Fragment Shader
    glGetShaderiv(fragmentShaderID, GL_COMPILE_STATUS, &result);
    print_error();
    if (result != GL_TRUE)
    {
        std::cout << "Failed to compile fragment shader" << std::endl;
        print_log(fragmentShaderID);
        exit(EXIT_FAILURE);
    }
    
    // Link the program
    std::cout << "Linking program" << std::endl;
    GLuint programID = glCreateProgram();
    glAttachShader(programID, vertexShaderID);
    print_error();
    glAttachShader(programID, fragmentShaderID);
    print_error();
    glLinkProgram(programID);
    print_error();
    
    // Check the program
    glGetProgramiv(programID, GL_LINK_STATUS, &result);
    print_error();
    if (result != GL_TRUE)
    {
        std::cout << "Failed to link shader program" << std::endl;
        exit(EXIT_FAILURE);
    }
    
    glDeleteShader(vertexShaderID);
    print_error();
    glDeleteShader(fragmentShaderID);
    print_error();
    
    return programID;
}