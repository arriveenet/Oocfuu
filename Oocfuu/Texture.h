#pragma once

class Texture
{
	int m_width;
	int m_height;
	int m_padding;
	unsigned char* m_bitmapPixels;
	unsigned char* m_pixels;

public:
	Texture();
	~Texture();

	int getWidth() const;
	int getHeight() const;

	const unsigned char* getTexImage() const;
	void deleteTexImage();

	int loadBitmapFile(const char* _fileName, const unsigned char* _colorKey = nullptr);
	int loadBitmapFile(const char* _fileName,
		unsigned char _colorKeyR,
		unsigned char _colorKeyG,
		unsigned char _colorKeyB);
private:
	int openBitmapFile(const char* _fileName);
	int createTexImage(const unsigned char* _colorKey = nullptr);
};
