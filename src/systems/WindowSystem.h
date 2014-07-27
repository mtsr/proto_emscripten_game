//
//  WindowSystem.h
//  CrossPlatform
//
//  Created by Jonas Matser on 27-07-14.
//
//

#ifndef __CrossPlatform__WindowSystem__
#define __CrossPlatform__WindowSystem__

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

class WindowSystem : public entityx::System<WindowSystem> {
    SDL_Window* window;
    SDL_GLContext context;
    
    double accumulator = 0;
    int frame_count = 0;
    
    void updateFPSCounter(SDL_Window* window, double delta);

public:
    WindowSystem();
    ~WindowSystem();
    
    void configure(entityx::EventManager &event_manager);
    void update(entityx::EntityManager &es, entityx::EventManager &events, double dt) override;
};

#endif /* defined(__CrossPlatform__WindowSystem
System__) */
