//
//  RenderSystem.cpp
//  CrossPlatform
//
//  Created by Jonas Matser on 27-07-14.
//
//

#include "RenderSystem.h"

#include <glm/gtc/type_ptr.hpp>

#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

RenderSystem::RenderSystem() {
    GLint viewport[4];
    glGetIntegerv(GL_VIEWPORT, viewport);
    
    mWidth = viewport[2];
    mHeight = viewport[3];
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

#include <glm/gtx/string_cast.hpp>
std::ostream &operator<< (std::ostream &out, const glm::vec3 &vec) {
    out << glm::to_string(vec);
    return out;
}

std::ostream &operator<< (std::ostream &out, const glm::vec4 &vec) {
    out << glm::to_string(vec);
    return out;
}

std::ostream &operator<< (std::ostream &out, const glm::mat4 &mat) {
    out << glm::to_string(mat);
    return out;
}

void RenderSystem::update(entityx::EntityManager &es, entityx::EventManager &events, double dt) {
    entityx::ComponentHandle<Camera> camera;
    for (entityx::Entity entity : es.entities_with_components(camera)) {
        if (resized) {
            camera->setWindow(-mWidth * .5f, mWidth * .5f, -mHeight * .5f, mHeight * .5f);
            resized = false;
        }
        glClearColor(.0f, .0f, .0f, .0f);
        glClear(
            GL_COLOR_BUFFER_BIT
          | GL_DEPTH_BUFFER_BIT
        );
        
        glm::mat4 projection = camera->getProjection();
        glm::mat4 view = camera->getView();
        
        entityx::ComponentHandle<Sprite> sprite;
        entityx::ComponentHandle<Transform> transform;
        for (entityx::Entity entity : es.entities_with_components(sprite, transform)) {
            draw(sprite, transform, view, projection);
        }
    }
}

void RenderSystem::draw(entityx::ComponentHandle<Sprite> sprite, entityx::ComponentHandle<Transform> transform, glm::mat4 view, glm::mat4 projection) {
    
    if (!Sprite::initialized) {
        Sprite::shaderProgram = loadShaders();
        
        const char* attribute_name = "vertexposition";
        Sprite::attribute_position = glGetAttribLocation(Sprite::shaderProgram, attribute_name);
        print_error();
        if (Sprite::attribute_position == -1) {
            std::cout << "Could not bind attribute " << attribute_name << std::endl;
            exit(1);
        }

        attribute_name = "texturecoordinates";
        Sprite::attribute_texture_coordinates = glGetAttribLocation(Sprite::shaderProgram, attribute_name);
        print_error();
        if (Sprite::attribute_position == -1) {
            std::cout << "Could not bind attribute " << attribute_name << std::endl;
            exit(1);
        }
        
        const char* uniform_name;
        uniform_name = "modelview";
        Sprite::uniform_modelview = glGetUniformLocation(Sprite::shaderProgram, uniform_name);
        print_error();
        if (Sprite::uniform_modelview == -1) {
            std::cout << "Could not bind uniform " << uniform_name << std::endl;
            exit(1);
        }
        
        uniform_name = "projection";
        Sprite::uniform_projection = glGetUniformLocation(Sprite::shaderProgram, uniform_name);
        print_error();
        if (Sprite::uniform_projection == -1) {
            std::cout << "Could not bind uniform " << uniform_name << std::endl;
            exit(1);
        }
        
        uniform_name = "mytexture";
        Sprite::uniform_texture = glGetUniformLocation(Sprite::shaderProgram, uniform_name);
        print_error();
        if (Sprite::uniform_texture == -1) {
            std::cout << "Could not bind uniform " << uniform_name << std::endl;
            exit(1);
        }
        
#ifndef EMSCRIPTEN
        glGenVertexArrays(1, &(Sprite::vertexArrayObject));
        print_error();
        glBindVertexArray(Sprite::vertexArrayObject);
        print_error();
#endif
        
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
        glBufferData(GL_ARRAY_BUFFER, sizeof(points), &points[0], GL_STATIC_DRAW);
        print_error();

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        print_error();
        
        GLfloat texcoords[] = {
            0.f, 1.f,
            1.f, 0.f,
            0.f, 0.f,
            0.f, 1.f,
            1.f, 1.f,
            0.f, 1.f
        };

        glGenBuffers(1, &(sprite->vertexBufferObject_texture_coordinates));
        print_error();
        glBindBuffer(GL_ARRAY_BUFFER, sprite->vertexBufferObject_texture_coordinates);
        print_error();
        glBufferData(GL_ARRAY_BUFFER, sizeof(texcoords), texcoords, GL_STATIC_DRAW);
        print_error();

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        print_error();
        
        GLushort indices[] = {
            0, 1, 2,
            3, 4, 5
        };
        
        glGenBuffers(1, &(Sprite::indexBufferObject));
        print_error();
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, Sprite::indexBufferObject);
        print_error();
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), &indices[0], GL_STATIC_DRAW);
        print_error();
        
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        print_error();
        
        Sprite::initialized = true;
        
        //                // TODO call
        //                glDeleteProgram(shaderProgram);
        //                glDeleteBuffers(1, &vertexBufferObject);
    }
    
    if (sprite->textureFilename != "" && !sprite->textureLoaded) {
        sprite->texture = loadTexture(sprite->textureFilename);
    }
    
    glm::mat4 modelview = view * transform->transform;

    glUseProgram(Sprite::shaderProgram);
    print_error();
    glBindBuffer(GL_ARRAY_BUFFER, Sprite::vertexBufferObject);
    print_error();
    glVertexAttribPointer(Sprite::attribute_texture_coordinates, 2, GL_FLOAT, GL_FALSE, 0, 0);
    print_error();
    
    glBindBuffer(GL_ARRAY_BUFFER, Sprite::vertexBufferObject_texture_coordinates);
    print_error();
    glVertexAttribPointer(Sprite::attribute_position, 3, GL_FLOAT, GL_FALSE, 0, 0);
    print_error();

    glActiveTexture(GL_TEXTURE0);
    print_error();
    glBindTexture(GL_TEXTURE_2D, sprite->texture);
    print_error();
    glUniform1i(Sprite::uniform_texture, sprite->texture);
    print_error();

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, Sprite::indexBufferObject);
    print_error();
    glUniformMatrix4fv(Sprite::uniform_modelview, 1, GL_FALSE, glm::value_ptr(modelview));
    print_error();
    glUniformMatrix4fv(Sprite::uniform_projection, 1, GL_FALSE, glm::value_ptr(projection));
    print_error();
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, 0);
    print_error();
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    print_error();

}

void RenderSystem::receive(const ResizeEvent &resizeEvent) {
    resized = true;
    mWidth = resizeEvent.width;
    mHeight = resizeEvent.height;
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
    "attribute vec2 texturecoordinates;\n"
    "\n"
    "varying vec2 f_texturecoordinates;\n"
    "\n"
    "uniform mat4 modelview;\n"
    "uniform mat4 projection;\n"
    "\n"
    "void main () {\n"
    "    gl_Position = projection * modelview * vec4(vertexposition, 1.0);\n"
//    "    gl_Position = vec4(vertexposition, 1.0);\n"
    "f_texturecoordinates = texturecoordinates;\n"
    "}";
    
    // Read the Fragment Shader code from the file
    std::string fragmentShaderCode = "#version 100\n"
    "precision mediump float;\n"
    "varying vec2 f_texturecoordinates;\n"
    "\n"
    "uniform sampler2D mytexture;\n"
    "\n"
    "void main () {\n"
    "    gl_FragColor = texture2D(mytexture, vec2(f_texturecoordinates.s, f_texturecoordinates.t));\n"
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

GLuint RenderSystem::loadTexture(const std::string filename) {
    int width, height, numComponents;
    unsigned char *data = stbi_load(filename.c_str(), &width, &height, &numComponents, 4);
    if (data == 0) {
        std::cout << stbi_failure_reason() << std::endl;
        exit(1);
    }
    // stbi_image_free(data);
    // glDeleteTextures(1, &texture_object_id);
    
    GLuint texture_object_id;
    glGenTextures(1, &texture_object_id);
    print_error();
    assert(texture_object_id != 0);
    
    glBindTexture(GL_TEXTURE_2D, texture_object_id);
    print_error();
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    print_error();
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    print_error();
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    print_error();
    glGenerateMipmap(GL_TEXTURE_2D);
    print_error();
    
    glBindTexture(GL_TEXTURE_2D, 0);
    print_error();
    return texture_object_id;
}