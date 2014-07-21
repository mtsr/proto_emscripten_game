#include <stdlib.h>
#include <iostream>

#ifdef EMSCRIPTEN
#include <emscripten.h>
#else
#endif

#include <SDL/SDL.h>
#include <SDL/SDL_opengles2.h>

SDL_Window* window;
#ifndef EMSCRIPTEN
bool done;
#endif

void update() {
    SDL_Event windowEvent;
    
    if (SDL_PollEvent(&windowEvent))
    {
        if (windowEvent.type == SDL_QUIT) done = true;
    }
    
    SDL_GL_SwapWindow(window);
}

int main(int argc, char *argv[]) {
    SDL_Init(SDL_INIT_VIDEO);
    
    window = SDL_CreateWindow("OpenGL", 100, 100, 800, 600, SDL_WINDOW_OPENGL);
    
    SDL_GLContext context = SDL_GL_CreateContext(window);
    
#ifdef EMSCRIPTEN
    emscripten_set_mainloop(update, 0, 1);
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