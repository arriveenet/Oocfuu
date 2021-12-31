#pragma once
class Sprite {
public:
	unsigned int m_textures[256];

	int loadBMPFile(const char* _fileName, unsigned char* _colorKey = nullptr);

private:
	void bindTexture(int _width, int _heihgt, void* _pixels);
};

