#include <iostream>

// Because of conflict with glew
#define NO_SDL_GLEXT
#include <GL/glew.h>
#include <SDL/SDL.h>
//#include <SDL/SDL_opengl.h>

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

void print_log(GLuint object)
{
   GLint log_length = 0;
   if (glIsShader(object))
       glGetShaderiv(object, GL_INFO_LOG_LENGTH, &log_length);
   else if (glIsProgram(object))
       glGetProgramiv(object, GL_INFO_LOG_LENGTH, &log_length);
   else {
       std::cout << "printlog: Not a shader or a program" << std::endl;
       return;
   }
   
   char* log = (char*)malloc(log_length);
   
   if (glIsShader(object))
       glGetShaderInfoLog(object, log_length, NULL, log);
   else if (glIsProgram(object))
       glGetProgramInfoLog(object, log_length, NULL, log);
   
   std::cout << log << std::endl;
   free(log);
}

GLuint loadShaders()
{
   // Create the shaders
   GLuint vertexShaderID = glCreateShader(GL_VERTEX_SHADER);
   print_error();
   GLuint fragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);
   print_error();
   
   std::string vertexShaderCode = "#version 100\n"
   "precision mediump float;\n"
   "attribute vec3 vertexposition;\n"
   "\n"
   "\n"
   "void main () {\n"
   "    gl_Position = vec4(vertexposition, 1.0);\n"
   "}";
   
   std::string fragmentShaderCode = "#version 100\n"
   "precision mediump float;\n"
   "\n"
   "void main () {\n"
   "    gl_FragColor = vec4(1.0, 1.0, 1.0, 1.0);\n"
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
   print_error();
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
       print_log(programID);
       exit(EXIT_FAILURE);
   }
   
   glDeleteShader(vertexShaderID);
   print_error();
   glDeleteShader(fragmentShaderID);
   print_error();
   
   return programID;
}

int main() {
   SDL_Window* window;
   SDL_GLContext context;
   
   if (SDL_Init(0) != 0) {
   	std::cout << "SDL could not initialize! SDL_Error:" << SDL_GetError() << std::endl;
   	exit(1);
   }
   
   if (SDL_VideoInit(NULL) != 0) {
   	std::cout << "SDL could not initialize video! SDL_Error:" << SDL_GetError() << std::endl;
   	exit(1);
   }
   
   SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
   SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
   SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

   SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
   SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
   
   window = SDL_CreateWindow(
       "Crossplatform",
       100, 100,
       800, 600, SDL_WINDOW_OPENGL
   );
   
   if ( window == NULL ) {
   	std::cout << "Window could not be created! SDL_Error:" << SDL_GetError() << std::endl;
   	exit(1);
   }
   
   context = SDL_GL_CreateContext(window);
   
   if ( context == NULL ) {
   	std::cout << "Context could not be created! SDL_Error:" << SDL_GetError() << std::endl;
   	exit(1);
   }
   
   glewExperimental=true;
   GLenum status = glewInit();
   // Throw away GL_INVALID_ENUM here
   glGetError();
   if (status != GLEW_OK)
   {
       std::cerr << "GLEW Error: " << glewGetErrorString(status) << "\n";
       print_error();
       exit(1);
   }
   
   // get version info
   const GLubyte* renderer = glGetString(GL_RENDERER); // get renderer string
   print_error();
   const GLubyte* version = glGetString(GL_VERSION); // version as a string
   print_error();
   std::cout << "Renderer: " << renderer << std::endl;
   std::cout << "OpenGL version supported: " << version << std::endl;
   
   glEnable(GL_DEPTH_TEST); // enable depth-testing
   print_error();
   glDepthFunc(GL_LESS); // depth-testing interprets a smaller value as "closer"
   print_error();
//    glEnable (GL_CULL_FACE); // cull face
//    glCullFace (GL_BACK); // cull back face
//    glFrontFace (GL_CW); // GL_CCW for counter clock-wise
   
   GLfloat points[] = {
                   -10.f,  10.f,  0.0f,
                    10.f, -10.f,  0.0f,
                   -10.f, -10.f,  0.0f
   };
   
   GLuint vertexBufferObject;
   glGenBuffers(1, &vertexBufferObject);
   print_error();
   glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObject);
   print_error();
   glBufferData(GL_ARRAY_BUFFER, sizeof(points), &points[0], GL_STATIC_DRAW);
   print_error();
   
   GLuint vertexArrayObject;
   glGenVertexArrays(1, &vertexArrayObject);
   print_error();
   glBindVertexArray(vertexArrayObject);
   print_error();

   GLuint shaderProgram = loadShaders();
   
   const char* attribute_name = "vertexposition";
   GLuint attribute_position = glGetAttribLocation(shaderProgram, attribute_name);
   print_error();
   if (attribute_position == -1) {
       std::cout << "Could not bind attribute " << attribute_name << std::endl;
       exit(1);
   }
   
   glEnableVertexAttribArray(attribute_position);
   print_error();
   glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObject);
   print_error();
   glVertexAttribPointer(
                         attribute_position, // attribute
                         3,                 // number of elements per vertex, here (x,y,z)
                         GL_FLOAT,          // the type of each element
                         GL_FALSE,          // take our values as-is
                         0,                 // no extra data between each position
                         0                  // offset of first element
                         );
   print_error();

   glBindBuffer(GL_ARRAY_BUFFER, 0);
   print_error();
   
   while (true) {
       glClearColor(.0f, .0f, .0f, .0f);
       print_error();
       glClear(
           GL_COLOR_BUFFER_BIT
           | GL_DEPTH_BUFFER_BIT
       );
       print_error();
       
       glUseProgram(shaderProgram);
       print_error();
       glDrawArrays(GL_TRIANGLES, 0, 3);
       print_error();
       glDisableVertexAttribArray(attribute_position);
       print_error();
       
       SDL_GL_SwapWindow(window);
   }
}
