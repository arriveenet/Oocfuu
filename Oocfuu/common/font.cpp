#include "font.h"
#include "App.h"
#include "world/CourseManager.h"
#include "FontFNT.h"

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <Windows.h>

#include <vector>
#include <gl/glut.h>
#include <glm/glm.hpp>


using namespace glm;

// Font texture
static vec2 textureSize;

// Font Internal variables
static GLint lastMatrixMode;
static ivec2 screenSize;
static ivec2 size;
static float scale;
static vec2 position;
static vec2 origin;
static COLORREF color;
static bool background;
static Font* pBitmapFont = nullptr;

static std::u32string convertUtf8ToUtf32(const std::string& utf8String)
{
	std::u32string result;

	for (int i = 0; i < utf8String.size(); i++) {
		char p = utf8String.at(i);
		char32_t unicode = 0;

		int numBytes = 0;
		if ((p & 0x80) == 0x00) {
			numBytes = 1;
		}
		else if ((p & 0xE0) == 0xC0) {
			numBytes = 2;
		}
		else if ((p & 0xF0) == 0xE0) {
			numBytes = 3;
		}
		else if ((p & 0xF8) == 0xF0) {
			numBytes = 4;
		}

		switch (numBytes) {
		case 1:
			unicode = p;
			result.push_back(unicode);
			break;
		case 2:
			unicode = (p & 0x1F) << 6;
			unicode |= (utf8String[i + 1] & 0x3F);
			result.push_back(unicode);
			break;
		case 3:
			unicode = (p & 0x0F) << 12;
			unicode |= (utf8String[i + 1] & 0x3F) << 6;
			unicode |= (utf8String[i + 2] & 0x3F);
			result.push_back(unicode);
			break;
		case 4:
			unicode = (p & 0x07) << 18;
			unicode |= (utf8String[i + 1] & 0x3F) << 12;
			unicode |= (utf8String[i + 2] & 0x3F) << 6;
			unicode |= (utf8String[i + 3] & 0x3F) << 6;
			result.push_back(unicode);
			break;
		default:
			break;
		}
	}

	return result;
}

int fontInit()
{
	screenSize = { SCREEN_WIDTH, SCREEN_HEIGHT };
	textureSize = { 128, 128 };
	scale = 1.0f;
	size = { 8,8 };
	color = RGB(0xFF, 0xFF, 0xFF);
	background = false;

	pBitmapFont = FontFNT::create("resource\\textures\\font\\font.fnt");
	if (pBitmapFont == nullptr)
		return 1;

	return 0;
}

void fontRelease()
{
	delete pBitmapFont;
	pBitmapFont = nullptr;
}

void fontBegin()
{
	glGetIntegerv(
		GL_MATRIX_MODE,		// GLenum pname
		&lastMatrixMode);	// GLint *params

	glPushAttrib(GL_ALL_ATTRIB_BITS);	//GLbitfield mask

	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();

	gluOrtho2D(
		0, screenSize.x,
		screenSize.y, 0);

	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();

	glDisable(GL_DEPTH_TEST);	//GLenum cap
	glDisable(GL_LIGHTING);		//GLenum cap

	// Enable texture
	glEnable(GL_TEXTURE_2D);	// GLenum cap
	glEnable(GL_BLEND);			// GLenum cap
	glEnableClientState(GL_VERTEX_ARRAY);// GLenum array
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);// GLenum array
	glBlendFunc(
		GL_SRC_ALPHA,			//GLenum sfactor
		GL_ONE_MINUS_SRC_ALPHA);//GLenum dfactor
	g_textureManager.setTexture(TEXTURE_FONT);
}

void fontEnd()
{
	g_textureManager.unbindTexture();
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glDisableClientState(GL_VERTEX_ARRAY);
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();

	glMatrixMode(GL_PROJECTION);
	glPopMatrix();

	glMatrixMode(lastMatrixMode);

	glPopAttrib();
}

void fontPosition(float _x, float _y)
{
	origin = position = { _x, _y };
}

void fontScale(float _scale)
{
	scale = _scale;
}

void fontColor(unsigned char _red, unsigned char _green, unsigned char _blue)
{
	color = RGB(_red, _green, _blue);
}

void fontBackgroundColor(bool _flag)
{
	background = _flag;
}


void fontDraw(const char* format, ...)
{
	va_list ap;
	char str[256];
	char* p;

	va_start(ap, format);
	vsprintf_s(str, format, ap);
	va_end(ap);

	int x = (int)position.x, y = (int)position.y;
	p = str;
	std::vector<QUAD> quads;

	while (1) {
		if ((*p) == '\n') {
			x = (int)position.x;
			y += size.y;
			p++;
			if (!(*p)) {
				position.y = (float)y;
				break;
			}
			continue;
		}

		int xoffset = (*p % 16) * 8;
		int yoffset = (*p / 16) * 8;
		//printf("xy: %d, %d\n", x, y);
		
		GLfloat tx0 = (float)xoffset / (float)textureSize.x;
		GLfloat tx1 = (float)(xoffset + size.x) / (float)textureSize.x;
		GLfloat ty0 = (float)yoffset / (float)textureSize.y;
		GLfloat ty1 = (float)(yoffset + size.y) / (float)textureSize.y;

		QUAD quad = {
			{
				{{x, y}, {tx0, ty0}},
				{{x , y + size.y}, {tx0, ty1}},
				{{x + size.x, y + size.y}, {tx1, ty1}},
				{{x + size.x, y}, {tx1, ty0}}
			}
		};
		quads.push_back(quad);

		x += size.x;
		p++;

		if (!(*p)) {
			fontPosition(origin.x, (float)y);
			break;
		}
	}

	glVertexPointer(3, GL_FLOAT, sizeof(VERTEX), &quads[0].vertices[0].position);
	glTexCoordPointer(2, GL_FLOAT, sizeof(VERTEX), &quads[0].vertices[0].texCoord);

	// Draw background color
	if (background) {
		glDisable(GL_TEXTURE_2D);
		glColor4ub(0x40, 0x40, 0x40, 0x80);
		glDrawArrays(GL_QUADS, 0, GLsizei(quads.size() * 4));
		glEnable(GL_TEXTURE_2D);
	}

	glColor4ub(GetRValue(color), GetGValue(color), GetBValue(color), 0xff);
	glDrawArrays(GL_QUADS, 0, GLsizei(quads.size() * 4));
}

void fontBitmapFontDraw(const char* format, ...)
{
	va_list ap;
	char str[512];

	va_start(ap, format);
	vsprintf_s(str, format, ap);
	va_end(ap);

	std::u32string utf32Text = convertUtf8ToUtf32(str);
	float x = position.x, y = position.y;
	float lineWidth = 0.0f;
	float lineHeight = pBitmapFont->getLineHeight();
	ivec2 textureSize = g_textureManager.getSize(pBitmapFont->getTexture());
	std::vector<QUAD> quads;

	for (int i = 0; i < utf32Text.size(); i++) {
		const char32_t p = utf32Text.at(i);

		if (p == '\n') {
			x = 0;
			y -= lineHeight;
			continue;
		}

		auto& pChar = pBitmapFont->m_characterDefinition[p];

		float tx0 = static_cast<float>(pChar.x) / textureSize.x;
		float ty0 = static_cast<float>(pChar.y) / textureSize.y;
		float tx1 = static_cast<float>((pChar.x) + pChar.width) / textureSize.x;
		float ty1 = static_cast<float>((pChar.y) + pChar.height) / textureSize.y;

		QUAD quad = { };
		quad.vertices[0].position = { x + pChar.xoffset, y + pChar.yoffset };
		quad.vertices[0].texCoord = { tx0, ty0 };

		quad.vertices[1].position = { x + pChar.xoffset, y + pChar.yoffset + pChar.height};
		quad.vertices[1].texCoord = { tx0, ty1 };

		quad.vertices[2].position = { x + pChar.xoffset + pChar.width, y + pChar.yoffset + pChar.height };
		quad.vertices[2].texCoord = { tx1, ty1 };

		quad.vertices[3].position = { x + pChar.xoffset + pChar.width, y + pChar.yoffset };
		quad.vertices[3].texCoord = { tx1, ty0 };

		quads.emplace_back(quad);
		x += pChar.xadvance;
	}

	g_textureManager.setTexture(pBitmapFont->getTexture());
	glVertexPointer(3, GL_FLOAT, sizeof(VERTEX), &quads[0].vertices[0].position);
	glTexCoordPointer(2, GL_FLOAT, sizeof(VERTEX), &quads[0].vertices[0].texCoord);

	glDrawArrays(GL_QUADS, 0, GLsizei(quads.size() * 4));
	g_textureManager.unbindTexture();
}

Font::Font()
	: m_texture(TEXTURE_FONT_BMP)
	, m_lineHeight(0.0f)
{
}

Font::~Font()
{
}

void Font::addCharacterDefinition(char32_t utf32char, const FontCharacterDefinition& defintition)
{
	m_characterDefinition[utf32char] = defintition;
}

bool Font::getCharacterDefinition(char32_t utf32char, FontCharacterDefinition& defintition)
{
	auto iter = m_characterDefinition.find(utf32char);
	if (iter != m_characterDefinition.end()) {
		defintition = (*iter).second;
		return true;
	}
	else {
		return false;
	}
}

void Font::setLineHeight(float lineHeight)
{
	m_lineHeight = lineHeight;
}

std::string_view Font::getFontName() const
{
	return m_fontName;
}

TEXTURE Font::getTexture()
{
	return m_texture;
}
