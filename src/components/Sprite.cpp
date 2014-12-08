//
//  Sprite.cpp
//  CrossPlatform
//
//  Created by Jonas Matser on 01-08-14.
//
//

#include "Sprite.h"

bool Sprite::initialized = false;
GLuint Sprite::vertexBufferObject = -1;
GLuint Sprite::vertexBufferObject_texture_coordinates = -1;
GLuint Sprite::indexBufferObject = -1;
GLuint Sprite::shaderProgram = -1;
GLuint Sprite::attribute_position = -1;
GLuint Sprite::attribute_texture_coordinates = -1;
GLuint Sprite::uniform_modelview = -1;
GLuint Sprite::uniform_projection = -1;
GLuint Sprite::uniform_texture = -1;

#ifndef Emscripten
GLuint Sprite::vertexArrayObject = -1;
#endif