//
//  RenderSystem.cpp
//  CrossPlatform
//
//  Created by Jonas Matser on 27-07-14.
//
//

#include "RenderSystem.h"

#include "../components/Camera.h"

RenderSystem::RenderSystem() {
    GLint viewport[4];
    glGetIntegerv(GL_VIEWPORT, viewport);
    
    width = viewport[2];
    height = viewport[3];
}

void RenderSystem::configure(entityx::EventManager &event_manager) {
    std::cout << "Configuring RenderSystem" << std::endl;

    event_manager.subscribe<ResizeEvent>(*this);
}

void RenderSystem::update(entityx::EntityManager &es, entityx::EventManager &events, double dt) {
    
    if (resized) {
        entityx::ComponentHandle<Camera> camera;
        
        for (entityx::Entity entity : es.entities_with_components(camera)) {
            camera->mAspectRatio = width/height;
        }
        resized = false;
    }
    
    

}

void RenderSystem::receive(const ResizeEvent &resizeEvent) {
    resized = true;
    width = resizeEvent.width;
    height = resizeEvent.height;
}