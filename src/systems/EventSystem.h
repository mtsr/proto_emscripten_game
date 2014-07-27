//
//  EventSystem.h
//  CrossPlatform
//
//  Created by Jonas Matser on 27-07-14.
//
//

#ifndef __CrossPlatform__EventSystem__
#define __CrossPlatform__EventSystem__

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

class EventSystem : public entityx::System<EventSystem> {
public:
    void configure(entityx::EventManager &event_manager);
    void update(entityx::EntityManager &es, entityx::EventManager &events, double dt) override;
    
    void handleKeyboardEvent(entityx::EventManager &events, SDL_KeyboardEvent &event);
};


#endif /* defined(__CrossPlatform__EventSystem__) */
