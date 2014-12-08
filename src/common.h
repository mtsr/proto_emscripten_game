//
//  common.h
//  CrossPlatform
//
//  Created by Jonas Matser on 07-08-14.
//
//

#ifndef CrossPlatform_common_h
#define CrossPlatform_common_h

#ifdef EMSCRIPTEN
#include <emscripten.h>
#include <SDL/SDL.h>
#include <SDL/SDL_opengles2.h>
#else
// Because of conflict with glew
#define NO_SDL_GLEXT
#include <GL/glew.h>
#include <SDL/SDL.h>
//#include <SDL/SDL_opengl.h>
#endif

#include <entityx/entityx.h>

#include <glm/glm.hpp>

#endif
