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
//#define NO_SDL_GLEXT
//#include <GL/glew.h>
#include <SDL/SDL.h>
#include <SDL/SDL_opengl.h>
#endif

#include <entityx/entityx.h>

#include "../events/ResizeEvent.h"

class WindowSystem : public entityx::System<WindowSystem>, public entityx::Receiver<WindowSystem> {
    SDL_Window* window;
    SDL_GLContext context;
    
    double accumulator = 0;
    int frame_count = 0;
    
    void updateFPSCounter(SDL_Window* window, double delta);

public:
    WindowSystem();
    ~WindowSystem() {
        if (context != NULL) {
            SDL_GL_DeleteContext(context);
        }
    }
    
    void configure(entityx::EventManager &event_manager);
    void update(entityx::EntityManager &es, entityx::EventManager &events, double dt) override;
    
    void receive(const ResizeEvent &resizeEvent);
};

#endif /* defined(__CrossPlatform__WindowSystem
System__) */
