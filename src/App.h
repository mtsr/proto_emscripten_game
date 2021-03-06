//
//  App.h
//  CrossPlatform
//
//  Created by Jonas Matser on 27-07-14.
//
//

#ifndef __CrossPlatform__App__
#define __CrossPlatform__App__

#include "common.h"

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
    
    entityx::Entity camera;
    
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
    
    void init();
    void run();
    void update();
    
    void receive(const QuitEvent &quitEvent);
};

#endif /* defined(__CrossPlatform__App__) */
