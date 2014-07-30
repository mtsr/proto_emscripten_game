//
//  RenderSystem.h
//  CrossPlatform
//
//  Created by Jonas Matser on 27-07-14.
//
//

#ifndef __CrossPlatform__RenderSystem__
#define __CrossPlatform__RenderSystem__

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

class RenderSystem : public entityx::System<RenderSystem>, public entityx::Receiver<RenderSystem> {
    
    GLint width, height;
    bool resized = true;
public:
    RenderSystem();
    ~RenderSystem() {};
    
    void configure(entityx::EventManager &event_manager);
    void update(entityx::EntityManager &es, entityx::EventManager &events, double dt) override;
    
    void receive(const ResizeEvent &resizeEvent);
};

#endif /* defined(__CrossPlatform__RenderSystem__) */
