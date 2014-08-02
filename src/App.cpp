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

#include "systems/WindowSystem.h"
#include "systems/EventSystem.h"
#include "systems/RenderSystem.h"

#include "components/Transform.h"
#include "components/Camera.h"
#include "components/Sprite.h"

void App::init() {
    if (SDL_Init(0) != 0) {
        std::cout << "SDL could not initialize! SDL_Error:" << SDL_GetError() << std::endl;
        exit(1);
    }
    
    start_ms = SDL_GetTicks();
    
    systems.add<WindowSystem>();
    systems.add<EventSystem>();
    systems.add<RenderSystem>();
    systems.configure();
}

#ifdef EMSCRIPTEN
static App* app;

static void static_update() {
    app->update();
}
#endif

void App::run() {
    entityx::Entity first = entities.create();
    first.assign<Sprite>();
    first.assign<Transform>();
    first.component<Transform>()->scale(glm::vec3(10.f, 10.f, 1.f));
    
    camera = entities.create();
    camera.assign<Camera>();
    camera.component<Camera>()->mPosition = glm::vec3(0.f, 0.f, -10.f);
    camera.component<Camera>()->mUp = glm::vec3(0.f, 1.f, 0.f);
    camera.component<Camera>()->mLookAt = glm::vec3(0.f, 0.f, 0.f);
    
#ifdef EMSCRIPTEN
    app = this;
    emscripten_set_main_loop(static_update, 0, 1);
#else
    while (!done) {
        update();
    }
#endif
}

void App::update() {
    int current_ms = SDL_GetTicks();
    int delta_ms = current_ms - previous_ms;
    previous_ms = current_ms;
        
    accumulator_ms += delta_ms;
    int count = 0;
    while (accumulator_ms > FIXED_TIME_STEP_MS && ++count <= MAX_LOOPS) {
        accumulator_ms -= FIXED_TIME_STEP_MS;
        systems.update<EventSystem>(FIXED_TIME_STEP_MS / 1000.f);

        //        systems.update<InputSystem>(FIXED_TIME_STEP / 1000.f);
        //        systems.update<PhysicsSystem>(FIXED_TIME_STEP / 1000.f);
    }
    //    systems.update<InterpolationSystem>(accumulator_ms / 1000.f);
    systems.update<RenderSystem>(delta_ms / 1000.f);
    systems.update<WindowSystem>(delta_ms / 1000.f);
}

void App::receive(const QuitEvent &quitEvent) {
#ifdef EMSCRIPTEN
    emscripten_cancel_main_loop();
#else
    done = true;
#endif
}