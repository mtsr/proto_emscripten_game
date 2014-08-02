//
//  Transform.h
//  CrossPlatform
//
//  Created by Jonas Matser on 30-07-14.
//
//

#ifndef __CrossPlatform__Transform__
#define __CrossPlatform__Transform__

#include <glm/glm.hpp>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>

struct Transform : public entityx::Component<Transform> {
    glm::mat4 transform;
    
    Transform() : transform(glm::mat4(1.f)) {};
    Transform(glm::mat4 transform) : transform(transform) {};
    
    void scale(glm::vec3 scale) {
        transform = glm::scale(transform, scale);
    }

    void rotate(GLfloat angle, glm::vec3 axis) {
        transform = glm::rotate(angle, axis) * transform;
    }
};

#endif /* defined(__CrossPlatform__Transform__) */
