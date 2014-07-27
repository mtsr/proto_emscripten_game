//
//  RenderSystem.cpp
//  CrossPlatform
//
//  Created by Jonas Matser on 27-07-14.
//
//

#include "RenderSystem.h"

void RenderSystem::configure(entityx::EventManager &event_manager) {
    event_manager.subscribe<ResizeEvent>(*this);
}

void RenderSystem::update(entityx::EntityManager &es, entityx::EventManager &events, double dt) {
}

void RenderSystem::receive(const ResizeEvent &resizeEvent) {
    
}