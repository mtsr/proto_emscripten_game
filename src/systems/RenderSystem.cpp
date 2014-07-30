//
//  RenderSystem.cpp
//  CrossPlatform
//
//  Created by Jonas Matser on 27-07-14.
//
//

#include "RenderSystem.h"

#include "../components/Camera.h"
#include "../components/Transform.h"
#include "../components/Sprite.h"

RenderSystem::RenderSystem() {
    GLint viewport[4];
    glGetIntegerv(GL_VIEWPORT, viewport);
    
    width = viewport[2];
    height = viewport[3];
}

void RenderSystem::configure(entityx::EventManager &event_manager) {
    std::cout << "Configuring RenderSystem" << std::endl;

    event_manager.subscribe<ResizeEvent>(*this);
    
    glEnable(GL_DEPTH_TEST); // enable depth-testing
    glDepthFunc(GL_LESS); // depth-testing interprets a smaller value as "closer"
    glEnable (GL_CULL_FACE); // cull face
    glCullFace (GL_BACK); // cull back face
    glFrontFace (GL_CW); // GL_CCW for counter clock-wise
}

void RenderSystem::update(entityx::EntityManager &es, entityx::EventManager &events, double dt) {
    
    entityx::ComponentHandle<Camera> camera;
    for (entityx::Entity entity : es.entities_with_components(camera)) {
        if (resized) {
            camera->mWidth = width;
            camera->mHeight = height;
            resized = false;
        }
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        glm::mat4 projection = camera->getProjection();
        glm::mat4 view = camera->getView();
        
        entityx::ComponentHandle<Sprite> sprite;
        entityx::ComponentHandle<Transform> transform;
        for (entityx::Entity entity : es.entities_with_components(sprite, transform)) {
            sprite->draw();
        }

    }
}

void RenderSystem::receive(const ResizeEvent &resizeEvent) {
    resized = true;
    width = resizeEvent.width;
    height = resizeEvent.height;
}