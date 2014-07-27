#include <stdlib.h>
#include <iostream>

#ifdef EMSCRIPTEN
#include <emscripten.h>
#else
#endif

#ifdef EMSCRIPTEN
#include <SDL/SDL.h>
#include <SDL/SDL_opengles2.h>
#else
// Because of conflict with glew
#define NO_SDL_GLEXT
#include <GL/glew.h>

#include <SDL/SDL.h>
#endif

#include <entityx/entityx.h>
#include <glm/glm.hpp>

SDL_Window* window;

int start_time = 0;

#ifndef EMSCRIPTEN
bool done;
#endif

void updateFPSCounter(SDL_Window* window) {
    static int previous_ms = SDL_GetTicks();
    static int frame_count;
    int current_ms = SDL_GetTicks();
    int elapsed_ms = current_ms - previous_ms;
    if (elapsed_ms > 250) {
        previous_ms = current_ms;
        double fps = (double)frame_count / elapsed_ms * 1000;
        char tmp[128];
        sprintf(tmp, "opengl @ fps: %.2lf", fps);
        SDL_SetWindowTitle(window, tmp);
        frame_count = 0;
    }
    frame_count++;
}

void update() {
    updateFPSCounter(window);
    
    SDL_Event windowEvent;
    
    if (SDL_PollEvent(&windowEvent))
    {
        if (windowEvent.type == SDL_QUIT) {
#ifdef EMSCRIPTEN
            emscripten_cancel_main_loop();
#else
            done = true;
#endif
        }
    }
    
    SDL_GL_SwapWindow(window);
}

int main(int argc, char *argv[]) {
    if (SDL_Init(0) != 0) {
    	std::cout << "SDL could not initialize! SDL_Error:" << SDL_GetError() << std::endl;
    	return 1;
    }

	int n = SDL_GetNumVideoDrivers();
    if (n == 0) {
        std::cout << "No built-in video drivers" << std::endl;
        return 1;
    } else {
        std::cout << "Built-in video drivers:" << std::endl;
        for (int i = 0; i < n; ++i) {
            if (i > 0) {
                std::cout << ",";
            }
            std::cout << " " << SDL_GetVideoDriver(i);
        }
        std::cout << std::endl;
    }

    if (SDL_VideoInit(NULL) != 0) {
    	std::cout << "SDL could not initialize video! SDL_Error:" << SDL_GetError() << std::endl;
    	return 1;
    }

    start_time = SDL_GetTicks();
    
#ifdef EMSCRIPTEN
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_ES);
#else
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
#endif
    
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
    
    window = SDL_CreateWindow(
    	"Crossplatform",
#ifdef EMSCRIPTEN
    SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,    
#else
    	100, 100,
#endif
    	800, 600, SDL_WINDOW_OPENGL
	);

    if ( window == NULL ) {
    	std::cout << "Window could not be created! SDL_Error:" << SDL_GetError() << std::endl;
    	return 1;
    }

    SDL_GLContext context = SDL_GL_CreateContext(window);

    if ( context == NULL ) {
    	std::cout << "Context could not be created! SDL_Error:" << SDL_GetError() << std::endl;
    	return 1;
    }
    
    // get version info
    const GLubyte* renderer = glGetString(GL_RENDERER); // get renderer string
    const GLubyte* version = glGetString(GL_VERSION); // version as a string
    std::cout << "Renderer: " << renderer << std::endl;
    std::cout << "OpenGL version supported " << version << std::endl;
    
#ifdef EMSCRIPTEN
    emscripten_set_main_loop(update, 0, 1);
#else
    done = false;
    while (!done) {
        update();
    }
#endif
    
    SDL_GL_DeleteContext(context);
    SDL_Quit();
    
    return 0;
}