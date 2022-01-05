#include "Sprite.h"

#include <Windows.h>
#include <stdio.h>
#include <glut.h>

Sprite g_sprite;

int Sprite::loadBMPFile(const char* _fileName, unsigned char* _colorKey)
{
	FILE* pFile;
	fopen_s(&pFile, _fileName, "rb");
	if (pFile == nullptr) {
		printf("%s open failed!\n", _fileName);
		return 1;
	}

	printf("%s opened!\n", _fileName);

	BITMAPFILEHEADER bf;
	fread(&bf, sizeof BITMAPFILEHEADER, 1, pFile);
	//printf("%d\n", bf.bfSize);

	BITMAPINFOHEADER bi;
	fread(&bi, sizeof BITMAPINFOHEADER, 1, pFile);
	//printf("biWidth:%d biHeight:%d\n", bi.biWidth, bi.biHeight);
	//printf("biBitCount:%d\n", bi.biBitCount);

	if (bi.biWidth != 128 && bi.biHeight != 128) {
		printf("File size must be 128 pixels.");
		return 1;
	}

	int padding = (4 - bi.biWidth * (bi.biBitCount / 8) % 4) % 4;
	typedef struct {
		unsigned char r, g, b, a;
	}RGBA;
	RGBA* pixels = (RGBA*)malloc(sizeof RGBA * bi.biWidth * bi.biHeight);

	for (int y = 0; y < bi.biHeight; y++) {
		for (int x = 0; x < bi.biWidth; x++) {
			RGBA* pPixel = &pixels[y * bi.biWidth + x];
			fread(pPixel, 3, 1, pFile);

			pPixel->a = ((_colorKey != nullptr)
				&& (pPixel->b == _colorKey[0])
				&& (pPixel->g == _colorKey[1])
				&& (pPixel->r == _colorKey[2]))
				? 0x00
				: 0xff;
		}
		fseek(pFile, padding, SEEK_CUR);
	}

	for (int y = 0; y < bi.biHeight; y++)
		for (int x = 0; x < bi.biWidth; x++) {
			RGBA* pPixel = &pixels[y * bi.biWidth + x];
			unsigned char temp = pPixel->r;
			pPixel->r = pPixel->b;
			pPixel->b = temp;
		}

	for (int y = 0; y < bi.biHeight / 2; y++)
		for (int x = 0; x < bi.biWidth; x++) {
			RGBA* pPixel0 = &pixels[y * bi.biWidth + x];
			RGBA* pPixel1 = &pixels[(bi.biHeight - 1 - y) * bi.biWidth + x];
			RGBA temp = *pPixel0;
			*pPixel0 = *pPixel1;
			*pPixel1 = temp;
		}

	for (int i = 0; i < SPRITE_SIZE; i++) {
		RGBA sprit[64] = { 0 };
		int t = 0;
		int sx = 8 * i, sy = 8 * (i * 8 / bi.biWidth) - (i * 8 / bi.biWidth);
		//printf("%d\n", sy);
		for (int y = sy; y < sy + 8; y++) {
			for (int x = sx; x < sx + 8; x++) {
				int p = bi.biWidth * y + x;
				sprit[t] = pixels[p];
				//printf("%d\n", _width * y + x);
				t++;

			}
		}

		glGenTextures(
			1,			// GLsizei n
			&m_textures[i]);// GLuint *textures
		glBindTexture(
			GL_TEXTURE_2D,	// GLenum target
			m_textures[i]);		// GLuint texture
		bindTexture(8, 8, sprit);
	}

	free(pixels);
	fclose(pFile);
	return 0;
}

int Sprite::loadBMPFile(const char* _fileName,
	unsigned char _colorKeyR,
	unsigned char _colorKeyG,
	unsigned char _colorKeyB)
{
	unsigned char colorKey[] = {
	_colorKeyR,
	_colorKeyG,
	_colorKeyB,
	};
	return loadBMPFile(_fileName, colorKey);
}

void Sprite::bindTexture(int _width, int _heihgt, void* _pixels)
{
	glTexImage2D(
		GL_TEXTURE_2D,				// GLenum target
		0,							// GLint level
		GL_RGBA,					// GLint internalformat
		_width, _heihgt,	// GLsizei width, GLsizei height
		0,							// GLint border
		GL_RGBA,					// GLenum format
		GL_UNSIGNED_BYTE,			// GLenum type
		_pixels);					// const GLvoid* pixels
	glTexParameteri(
		GL_TEXTURE_2D,			// GLenum target
		GL_TEXTURE_MAG_FILTER,	// GLenum pname
		GL_NEAREST);			// GLint param
	glTexParameteri(
		GL_TEXTURE_2D,			// GLenum target
		GL_TEXTURE_MIN_FILTER,	// GLenum pname
		GL_NEAREST);			// GLint param

	glTexParameteri(
		GL_TEXTURE_2D,		// GLenum target
		GL_TEXTURE_WRAP_S,	// GLenum pname
		GL_CLAMP);			// GLint param

	glTexParameteri(
		GL_TEXTURE_2D,		// GLenum target
		GL_TEXTURE_WRAP_T,	// GLenum pname
		GL_CLAMP);			// GLint param
}