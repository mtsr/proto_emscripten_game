//
//  ResizeEvent.h
//  CrossPlatform
//
//  Created by Jonas Matser on 27-07-14.
//
//

#ifndef CrossPlatform_ResizeEvent_h
#define CrossPlatform_ResizeEvent_h

#include <entityx/entityx.h>

struct ResizeEvent : public entityx::Event<ResizeEvent> {
    ResizeEvent(GLint width, GLint height) : width(width), height(height) {}
    
    GLint width, height;
};

#endif
