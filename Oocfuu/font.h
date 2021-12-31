#pragma once

#include "glm/glm.hpp"
using namespace glm;

#define FONT_DEFAULT_WIDTH		105.f
#define FONT_DEFAULT_HEIGHT		119.05f

void fontInit(float _width, float _height);
void fontScreenSize(float _width, float _height);
void fontBegin();
void fontEnd();
void fontPosition(float _x, float _y);
void fontPosition(vec2 const& _position);
void fontSize(float _width, float _height);
void fontSize(float _size);
void fontSize(vec2 const& _size);
void fontWeight(float _weight);
float fontGetWidth(int _character);
float fontGetLength(const unsigned char* _string);
vec2 fontGetSize();
float fontGetWeightMin();
float fontGetWeightMax();
float fontGetWeight();

void fontDraw(const char* _format, ...);