#pragma once

class Image {
public:
	int m_width;
	int m_height;
	unsigned char* m_pData;

	virtual ~Image();
	virtual bool load(const char* _fileName) = 0;
};