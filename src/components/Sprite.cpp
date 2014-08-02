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
GLuint Sprite::shaderProgram = -1;
GLuint Sprite::attribute_position = -1;
GLuint Sprite::uniform_modelview = -1;
GLuint Sprite::uniform_projection = -1;