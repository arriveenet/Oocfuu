#pragma once
#ifndef _FONT_H_
#define _FONT_H_

#include <gl/glut.h>

int fontInit();
void fontRelease();

void fontBegin();
void fontEnd();
void fontPosition(float _x, float _y);
void fontScale(float _scale);

void fontDraw(const char* format, ...);

#endif // !_FONT_H_