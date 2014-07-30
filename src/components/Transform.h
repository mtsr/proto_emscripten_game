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

struct Transform : public entityx::Component<Transform> {
    glm::mat4 transform;
};

#endif /* defined(__CrossPlatform__Transform__) */
