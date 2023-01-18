#pragma once
#ifndef _FONT_H_
#define _FONT_H_

int fontInit();
void fontRelease();

void fontBegin();
void fontEnd();
void fontPosition(float _x, float _y);
void fontScale(float _scale);
void fontColor(unsigned char _red, unsigned char _green, unsigned char _blue);
void fontBackgroundColor(bool _flag);

void fontDraw(const char* format, ...);

#endif // !_FONT_H_