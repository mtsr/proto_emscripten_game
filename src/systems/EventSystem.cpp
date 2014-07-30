//
//  EventSystem.cpp
//  CrossPlatform
//
//  Created by Jonas Matser on 27-07-14.
//
//

#include "EventSystem.h"
#include "../events/QuitEvent.h"

void EventSystem::configure(entityx::EventManager &event_manager) {
    std::cout << "Configuring EventSystem" << std::endl;
}

void EventSystem::update(entityx::EntityManager &es, entityx::EventManager &events, double dt) {
    SDL_Event event;
    
    if (SDL_PollEvent(&event))
    {
        switch(event.type) {
            case SDL_QUIT:
                events.emit<QuitEvent>();
                break;
            case SDL_KEYDOWN:
                handleKeyboardEvent(events, event.key);
                break;
        }
    }
}

void EventSystem::handleKeyboardEvent(entityx::EventManager &events, SDL_KeyboardEvent &event) {
    if (event.keysym.sym == SDLK_ESCAPE) {
        events.emit<QuitEvent>();
    }
}