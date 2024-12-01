#pragma once
#include <set>
#include "Rect.h"
#include "common/Font.h"

class BMFontConfiguration {
public:
	struct BMFontDef {
		unsigned int charID;
		Rect rect;
		short xOffset;
		short yOffset;
		short xAdvance;

		BMFontDef() : charID(0), xOffset(0), yOffset(0), xAdvance(0) {}
	};

	struct BMFontPadding {
		int left;
		int top;
		int right;
		int bottom;
	};

	static BMFontConfiguration* create(std::string_view fntFile);

	BMFontConfiguration();
	virtual ~BMFontConfiguration();

	bool initWithFNTFile(std::string_view fntFile);

protected:
	virtual std::set<unsigned int>* parseConfigFile(std::string_view controlFile);
	virtual std::set<unsigned int>* parseBinaryConfigFile(unsigned char* pData, uint32_t size, std::string_view controlFile);

public:
	std::unordered_map<int, BMFontDef> m_fontDefDictionary;
	int m_commonHeight;
	BMFontPadding m_padding;
	std::set<unsigned int>* m_charactorSet;
	int m_fontSize;
	std::string m_fontName;
	std::string m_atlasName;
};

class FontFNT : public Font {
public:
	static FontFNT* create(std::string_view fntFilePath);

protected:
	FontFNT(BMFontConfiguration* config);
	~FontFNT();

	bool init();

	BMFontConfiguration* m_pConfiguration;
};