//
//  RenderSystem.h
//  CrossPlatform
//
//  Created by Jonas Matser on 27-07-14.
//
//

#ifndef __CrossPlatform__RenderSystem__
#define __CrossPlatform__RenderSystem__

#include "../common.h"

#include "../events/ResizeEvent.h"

#include "../components/Camera.h"
#include "../components/Transform.h"
#include "../components/Sprite.h"

class RenderSystem : public entityx::System<RenderSystem>, public entityx::Receiver<RenderSystem> {
    
    GLint mWidth, mHeight;
    bool resized = true;
public:
    RenderSystem();
    ~RenderSystem() {};
    
    void configure(entityx::EventManager &event_manager);
    void update(entityx::EntityManager &es, entityx::EventManager &events, double dt) override;
    
    void receive(const ResizeEvent &resizeEvent);
    
    void draw(entityx::ComponentHandle<Sprite> sprite, entityx::ComponentHandle<Transform> transform, glm::mat4 view, glm::mat4 projection);
    
protected:
    void print_log(GLuint object);
    GLuint loadShaders();
    GLuint loadTexture(const std::string filename);
};

#endif /* defined(__CrossPlatform__RenderSystem__) */
