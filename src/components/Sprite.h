//
//  Sprite.h
//  CrossPlatform
//
//  Created by Jonas Matser on 30-07-14.
//
//

#ifndef __CrossPlatform__Sprite__
#define __CrossPlatform__Sprite__

#ifdef EMSCRIPTEN
#include <SDL/SDL.h>
#include <SDL/SDL_opengles2.h>
#else
// Because of conflict with glew
//#define NO_SDL_GLEXT
//#include <GL/glew.h>
#include <SDL/SDL.h>
#include <SDL/SDL_opengl.h>
#endif

static bool initialized;
static GLuint vertexBufferObject, shaderProgram, attribute_position;

class Sprite : public entityx::Component<Sprite> {
public:
    
    Sprite() {
        init();
    }
    
    static void print_log(GLuint object)
    {
        GLint log_length = 0;
        if (glIsShader(object))
            glGetShaderiv(object, GL_INFO_LOG_LENGTH, &log_length);
        else if (glIsProgram(object))
            glGetProgramiv(object, GL_INFO_LOG_LENGTH, &log_length);
        else {
            fprintf(stderr, "printlog: Not a shader or a program\n");
            return;
        }
        
        char* log = (char*)malloc(log_length);
        
        if (glIsShader(object))
            glGetShaderInfoLog(object, log_length, NULL, log);
        else if (glIsProgram(object))
            glGetProgramInfoLog(object, log_length, NULL, log);
        
        fprintf(stderr, "%s", log);
        free(log);
    }
    
    static GLuint loadShaders(const char * vertex_file_path, const char * fragment_file_path)
    {
        // Create the shaders
        GLuint vertexShaderID = glCreateShader(GL_VERTEX_SHADER);
        GLuint fragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);
        
        // Read the Vertex Shader code from the file
        std::string vertexShaderCode = "#version 100\n"
        "attribute vec3 vp;\n"
        "\n"
        "uniform mat4 modelview;\n"
        "uniform mat4 projection;\n"
        "\n"
        "void main () {\n"
        "    gl_Position = projection * modelview * vec4(vp, 1.0);\n"
        "}";
        
        // Read the Fragment Shader code from the file
        std::string fragmentShaderCode = "#version 100\n"
        "\n"
        "void main () {\n"
        "    gl_FragColor = vec4 (1.0, 0.0, 1.0, 1.0);\n"
        "}";
        
        GLint result = GL_FALSE;
        GLint infoLogLength;
        GLchar* strInfoLog;
        
        // Compile Vertex Shader
        std::cout << "Compiling shader: " <<  vertex_file_path << std::endl;
        char const * vertexSourcePointer = vertexShaderCode.c_str();
        glShaderSource(vertexShaderID, 1, &vertexSourcePointer , NULL);
        glCompileShader(vertexShaderID);
        
        // Check Vertex Shader
        glGetShaderiv(vertexShaderID, GL_COMPILE_STATUS, &result);
        if (result != GL_TRUE)
        {
            std::cout << "Failed to compile vertex shader" << std::endl;
            print_log(vertexShaderID);
            exit(EXIT_FAILURE);
        }
        
        glGetShaderiv(vertexShaderID, GL_INFO_LOG_LENGTH, &infoLogLength);
        if (infoLogLength > 0)
        {
            strInfoLog = new GLchar[infoLogLength + 1];
            glGetShaderInfoLog(vertexShaderID, infoLogLength, NULL, strInfoLog);
            std::cout << strInfoLog << std::endl;
            delete strInfoLog;
        }
        
        // Compile Fragment Shader
        std::cout << "Compiling shader: " << fragment_file_path << std::endl;
        char const * fragmentSourcePointer = fragmentShaderCode.c_str();
        glShaderSource(fragmentShaderID, 1, &fragmentSourcePointer , NULL);
        glCompileShader(fragmentShaderID);
        
        // Check Fragment Shader
        glGetShaderiv(fragmentShaderID, GL_COMPILE_STATUS, &result);
        if (result != GL_TRUE)
        {
            std::cout << "Failed to compile fragment shader" << std::endl;
            print_log(fragmentShaderID);
            exit(EXIT_FAILURE);
        }
        
        glGetShaderiv(fragmentShaderID, GL_INFO_LOG_LENGTH, &infoLogLength);
        if (infoLogLength > 0)
        {
            strInfoLog = new GLchar[infoLogLength + 1];
            glGetShaderInfoLog(fragmentShaderID, infoLogLength, NULL, strInfoLog);
            std::cout << strInfoLog << std::endl;
            delete strInfoLog;
        }
        
        // Link the program
        std::cout << "Linking program" << std::endl;
        GLuint programID = glCreateProgram();
        glAttachShader(programID, vertexShaderID);
        glAttachShader(programID, fragmentShaderID);
        glLinkProgram(programID);
        
        // Check the program
        glGetProgramiv(programID, GL_LINK_STATUS, &result);
        if (result != GL_TRUE)
        {
            std::cout << "Failed to link shader program" << std::endl;
            exit(EXIT_FAILURE);
        }
        
        glGetProgramiv(programID, GL_INFO_LOG_LENGTH, &infoLogLength);
        if (infoLogLength > 0)
        {
            strInfoLog = new GLchar[infoLogLength + 1];
            glGetProgramInfoLog(fragmentShaderID, infoLogLength, NULL, strInfoLog);
            std::cout << strInfoLog << std::endl;
            delete strInfoLog;
        }
        
        glDeleteShader(vertexShaderID);
        glDeleteShader(fragmentShaderID);
        
        return programID;
    }

    static void init() {
        if (!initialized)
        {
            std::vector<GLfloat> points = {
                -0.5f,  0.5f,  0.0f,
                0.5f, -0.5f,  0.0f,
                -0.5f, -0.5f,  0.0f,
                0.5f,  0.5f,  0.0f,
                0.5f, 0.5f,  0.0f,
                0.5f, -0.5f,  0.0f
            };
            
            glGenBuffers(1, &vertexBufferObject);
            glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObject);
            glBufferData(GL_ARRAY_BUFFER, points.size() * sizeof (GLfloat), &points[0], GL_STATIC_DRAW);
            
            shaderProgram = loadShaders("resources/shaders/simple.vert", "resources/shaders/simple.frag");
            
            const char* attribute_name = "vp";
            attribute_position = glGetAttribLocation(shaderProgram, attribute_name);
            if (attribute_position == -1) {
                std::cout << "Could not bind attribute " << attribute_name << std::endl;
                exit(1);
            }
        }
    }
    
    void draw() {
        glUseProgram(shaderProgram);
        glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObject);
        glEnableVertexAttribArray(attribute_position);
        glVertexAttribPointer(
            attribute_position, // attribute
            3,                 // number of elements per vertex, here (x,y,z)
            GL_FLOAT,          // the type of each element
            GL_FALSE,          // take our values as-is
            0,                 // no extra data between each position
            0                  // offset of first element
        );
        glDrawArrays(GL_TRIANGLES, 0, 6);
        glDisableVertexAttribArray(attribute_position);
    }
    
    // TODO call
    void destroy() {
        glDeleteProgram(shaderProgram);
        glDeleteBuffers(1, &vertexBufferObject);
    }
};


#endif /* defined(__CrossPlatform__Sprite__) */
