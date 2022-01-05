#pragma once

#include "glm/glm.hpp"

#define FONT_DEFAULT_WIDTH		8
#define FONT_DEFAULT_HEIGHT		8

void fontInit(float _width, float _height);
void fontScreenSize(float _width, float _height);
void fontBegin();
void fontEnd();
void fontPosition(float _x, float _y);
void fontPosition(glm::vec2 const& _position);

void fontDraw(const char* _format, ...);