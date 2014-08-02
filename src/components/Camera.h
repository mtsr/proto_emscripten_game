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

class Camera : public entityx::Component<Camera> {
protected:
    GLfloat mLeft = 0.f, mTop = 0.f, mWidth, mHeight, mNearClip = 1.f, mFarClip = 100.f;
    glm::vec3 mLookAt, mPosition, mUp;
    
public:
    glm::mat4 getProjection() {
        return glm::ortho(mLeft, mWidth, mHeight, mTop, mNearClip, mFarClip);
//        return glm::perspective(45.f, mWidth/mHeight, 1.f, 100.f);
    }
    
    glm::mat4 getView() {
        return glm::lookAt(mPosition, mLookAt, mUp);
    }

    void setPosition(glm::vec3 position) {
        std::cout << "Camera.setPosition()" << std::endl;
        mPosition = position;
    };
    
    void setUp(glm::vec3 up) {
        std::cout << "Camera.setUp()" << std::endl;
        mUp = up;
    };
    
    void setLookAt(glm::vec3 target) {
        std::cout << "Camera.setLookAt()" << std::endl;
        mLookAt = target;
    };
    
    void setClip(GLfloat near, GLfloat far) {
        std::cout << "Camera.setClip()" << std::endl;
        mNearClip = near, mFarClip = far;
    };
    
    void setWindow(GLfloat left, GLfloat width, GLfloat height, GLfloat top) {
        std::cout << "Camera.setWindowSize()" << std::endl;
        mLeft = left;
        mWidth = width;
        mHeight = height;
        mTop = top;
    }
};

#endif /* defined(__CrossPlatform__Camera__) */
