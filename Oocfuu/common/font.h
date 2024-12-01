#pragma once
#ifndef _FONT_H_
#define _FONT_H_
#include <unordered_map>
#include <string>
#include "TextureManager.h"

class Font;

int fontInit();
void fontRelease();

void fontBegin();
void fontEnd();
void fontPosition(float _x, float _y);
void fontScale(float _scale);
void fontColor(unsigned char _red, unsigned char _green, unsigned char _blue);
void fontBackgroundColor(bool _flag);

void fontDraw(const char* format, ...);
void fontBitmapFontDraw(const char* format, ...);

struct FontCharacterDefinition {
	float x;
	float y;
	float width;
	float height;
	float xoffset;
	float yoffset;
	float xadvance;
};

class Font {
public:
	Font();
	virtual ~Font();

	void addCharacterDefinition(char32_t utf32char, const FontCharacterDefinition& defintition);
	bool getCharacterDefinition(char32_t utf32char, FontCharacterDefinition& defintition);

	float getLineHeight() const { return m_lineHeight; }
	void setLineHeight(float lineHeight);

	std::string_view getFontName() const;
	TEXTURE getTexture();
protected:
	std::string m_fontName;
	TEXTURE m_texture;
	float m_lineHeight;
	std::unordered_map<char32_t, FontCharacterDefinition> m_characterDefinition;

	friend void fontBitmapFontDraw(const char* format, ...);
};

#endif // !_FONT_H_