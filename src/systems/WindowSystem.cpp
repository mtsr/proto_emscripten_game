//
//  WindowSystem.cpp
//  CrossPlatform
//
//  Created by Jonas Matser on 27-07-14.
//
//

#include "WindowSystem.h"

#include <iostream>

void WindowSystem::configure(entityx::EventManager &event_manager) {
    event_manager.subscribe<ResizeEvent>(*this);

    int n = SDL_GetNumVideoDrivers();
    if (n == 0) {
        std::cout << "No built-in video drivers" << std::endl;
        exit(1);
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
    	exit(1);
    }
    
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
    	exit(1);
    }
    
    context = SDL_GL_CreateContext(window);
    
    if ( context == NULL ) {
    	std::cout << "Context could not be created! SDL_Error:" << SDL_GetError() << std::endl;
    	exit(1);
    }
    
    // get version info
    const GLubyte* renderer = glGetString(GL_RENDERER); // get renderer string
    const GLubyte* version = glGetString(GL_VERSION); // version as a string
    std::cout << "Renderer: " << renderer << std::endl;
    std::cout << "OpenGL version supported " << version << std::endl;}

void WindowSystem::updateFPSCounter(SDL_Window* window, double delta) {
    accumulator += delta;
    if (accumulator > 0.25) {
        double fps = (double)frame_count / accumulator;
        char tmp[128];
        sprintf(tmp, "opengl @ fps: %.2lf", fps);
        SDL_SetWindowTitle(window, tmp);
        frame_count = 0;
        accumulator = 0;
    }
    frame_count++;
}

void WindowSystem::update(entityx::EntityManager &es, entityx::EventManager &events, double dt) {
    updateFPSCounter(window, dt);
    SDL_GL_SwapWindow(window);
}

void WindowSystem::receive(const ResizeEvent &resizeEvent) {
    
}