//
//  App.cpp
//  CrossPlatform
//
//  Created by Jonas Matser on 27-07-14.
//
//

#include "App.h"
#include <stdlib.h>
#include <iostream>

#ifdef EMSCRIPTEN
#include <emscripten.h>
#else
#endif

#include <glm/glm.hpp>

#include "systems/WindowSystem.h"

void App::init() {
    if (SDL_Init(0) != 0) {
        std::cout << "SDL could not initialize! SDL_Error:" << SDL_GetError() << std::endl;
        exit(1);
    }
    
    start_ms = SDL_GetTicks();
    
    systems.add<WindowSystem>();
    systems.configure();
}

void App::update() {
    int current_ms = SDL_GetTicks();
    int delta_ms = current_ms - previous_ms;
    previous_ms = current_ms;
    
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
    
    accumulator_ms += delta_ms;
    int count = 0;
    while (accumulator_ms > FIXED_TIME_STEP_MS && ++count <= MAX_LOOPS) {
        accumulator_ms -= FIXED_TIME_STEP_MS;
        //        systems.update<InputSystem>(FIXED_TIME_STEP / 1000.f);
        //        systems.update<PhysicsSystem>(FIXED_TIME_STEP / 1000.f);
    }
    //    systems.update<InterpolationSystem>(accumulator_ms / 1000.f);
    systems.update<WindowSystem>(delta_ms / 1000.f);
}
