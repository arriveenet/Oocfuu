#pragma once
#include "Image.h"

class PngImage : public Image {
public:
	PngImage();
	virtual ~PngImage();

	bool load(const char* _fileName) override;
};