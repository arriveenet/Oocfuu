#pragma once

#include "Rect.h"

int texFromBMP(const char* _fileName, unsigned char* _colorKey = nullptr, Rect* _pBound = nullptr);
int texFromBMP(const char* _fileName,
	unsigned char _colorKeyR,
	unsigned char _colorKeyG,
	unsigned char _colorKeyB,
	Rect* _pBound = nullptr);