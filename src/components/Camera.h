//
//  Camera.h
//  CrossPlatform
//
//  Created by Jonas Matser on 30-07-14.
//
//

#ifndef __CrossPlatform__Camera__
#define __CrossPlatform__Camera__

struct Camera : public entityx::Component<Camera> {
    GLfloat mAspectRatio;
};

#endif /* defined(__CrossPlatform__Camera__) */
