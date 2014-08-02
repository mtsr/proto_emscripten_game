//
//  Camera.h
//  CrossPlatform
//
//  Created by Jonas Matser on 30-07-14.
//
//

#ifndef __CrossPlatform__Camera__
#define __CrossPlatform__Camera__

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

struct Camera : public entityx::Component<Camera> {

    GLfloat mLeft = 0.f, mTop = 0.f, mWidth, mHeight, mNearClip = 1.f, mFarClip = 100.f;
    glm::vec3 mLookAt, mPosition, mUp;
    
    glm::mat4 getProjection() {
        return glm::ortho(0.f, mWidth, mHeight, 0.f, mNearClip, mFarClip);
//        return glm::perspective(45.f, mWidth/mHeight, 1.f, 100.f);
    }
    
    glm::mat4 getView() {
        return glm::lookAt(mLookAt, mPosition, mUp);
    }

    void setUp(glm::vec3 up) { mUp = up; };
    void setLookAt(glm::vec3 target) { mLookAt = target; };
    void setClip(GLfloat near, GLfloat far) { mNearClip = near, mFarClip = far; };    
};

#endif /* defined(__CrossPlatform__Camera__) */
