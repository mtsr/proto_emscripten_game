//
//  Sprite.h
//  CrossPlatform
//
//  Created by Jonas Matser on 30-07-14.
//
//

#ifndef __CrossPlatform__Sprite__
#define __CrossPlatform__Sprite__

#ifdef EMSCRIPTEN
#include <SDL/SDL.h>
#include <SDL/SDL_opengles2.h>
#else
// Because of conflict with glew
//#define NO_SDL_GLEXT
//#include <GL/glew.h>
#include <SDL/SDL.h>
#include <SDL/SDL_opengl.h>
#endif

#include <entityx/entityx.h>

struct Sprite : public entityx::Component<Sprite> {
    static bool initialized;
    static GLuint vertexBufferObject, shaderProgram, attribute_position, uniform_modelview, uniform_projection;
};

#endif /* defined(__CrossPlatform__Sprite__) */
