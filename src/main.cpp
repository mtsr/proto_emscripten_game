#include <stdlib.h>
#include <iostream>

#ifdef EMSCRIPTEN
#include <emscripten.h>
#else
#endif

// Because of conflict with glew
#define NO_SDL_GLEXT

#include <GL/glew.h>
#include <SDL/SDL.h>

#include <entityx/entityx.h>
#include <glm/glm.hpp>

SDL_Window* window;

int start_time = 0;

#ifndef EMSCRIPTEN
bool done;
#endif

void update() {
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