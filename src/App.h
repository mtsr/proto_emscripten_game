//
//  App.h
//  CrossPlatform
//
//  Created by Jonas Matser on 27-07-14.
//
//

#ifndef __CrossPlatform__App__
#define __CrossPlatform__App__

#include <iostream>

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

#include "events/QuitEvent.h"

class App : public entityx::Receiver<App> {
    entityx::EventManager events;
    entityx::EntityManager entities;
    entityx::SystemManager systems;
    
    int start_ms = 0;
    int previous_ms = 0;
    int accumulator_ms = 0;
    
    const int FIXED_TIME_STEP_MS = 10;
    const int MAX_LOOPS = 10;
    
public:
#ifndef EMSCRIPTEN
    bool done = false;
#endif
    
    App() : events(), entities(events), systems(entities, events) {
        events.subscribe<QuitEvent>(*this);
    }
    
    ~App() {
        SDL_Quit();
    }
    
    void update();
    void init();
    
    void receive(const QuitEvent &quitEvent);
};

#endif /* defined(__CrossPlatform__App__) */
