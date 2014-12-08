//
//  Sprite.h
//  CrossPlatform
//
//  Created by Jonas Matser on 30-07-14.
//
//

#ifndef __CrossPlatform__Sprite__
#define __CrossPlatform__Sprite__

#include "../common.h"

struct Sprite : public entityx::Component<Sprite> {
    static bool initialized;
    static GLuint vertexBufferObject, vertexBufferObject_texture_coordinates, indexBufferObject, shaderProgram, attribute_position, attribute_texture_coordinates, uniform_modelview, uniform_projection, uniform_texture;

#ifndef EMSCRIPTEN
    static GLuint vertexArrayObject;
#endif
    
    std::string textureFilename = "";
    GLuint texture;
    bool textureLoaded = false;
};

#endif /* defined(__CrossPlatform__Sprite__) */
