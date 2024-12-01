#include "FontFNT.h"
#include "TextureManager.h"
#include <fstream>

enum class FntBlockType : uint8_t {
	Info         = 1,
	Common       = 2,
	Pages        = 3,
	Chars        = 4,
	KerningPairs = 5,
	Unknown      = 6
};

#pragma pack(push, 1)
struct FntBlock {
	FntBlockType type;
	int32_t size;
};

/* file format: https://www.angelcode.com/products/bmfont/doc/file_format.html */

struct FntInfo {
	short fontSize;
	unsigned char bitField;
	unsigned char charSet;
	unsigned short stretchH;
	unsigned char aa;
	unsigned char paddingUp;
	unsigned char paddingRight;
	unsigned char paddingDown;
	unsigned char paddingLeft;
	unsigned char spacingHoriz;
	unsigned char spasingVert;
	unsigned char outline;
};

struct FntCommon {
	unsigned short lineHeight;
	unsigned short base;
	unsigned short scaleW;
	unsigned short scaleH;
	unsigned short pages;
	unsigned char bitField;
	unsigned char alphaChnl;
	unsigned char redChnl;
	unsigned char greenChnl;
	unsigned char blueChnl;
};

struct FntChars {
	unsigned int id;        // The character id
	unsigned short x;       // The left position of the character image in the texture
	unsigned short y;       // The top position of the character image in the texture
	unsigned short width;   // The width of the character image in the texture
	unsigned short height;  // The height of the character image in the texture
	short xoffset;          // How much the current position should be offset when copying the image from the texture to the screen
	short yoffset;          // How much the current position should be offset when copying the image from the texture to the screen
	short xadvance;         // How much the current position should be advanced after drawing the character.
	unsigned char page;     // The texture page where the character image is found
	unsigned char chnl;     // The texture channel where the character image is found (1 = blue, 2 = green, 4 = red, 8 = alpha, 15 = all channels)
};

struct FntKerningPairs {
	unsigned int first;
	unsigned int second;
	short amount;
};

#pragma pack(pop)

static std::string getStringFromFile(std::string_view filename)
{
	std::string fullPath = filename.data();

	std::string buffer;

	std::ifstream fs(fullPath, std::ios::binary);
	if (fs) {
		fs.seekg(0, std::ifstream::end);
		size_t fileSize = static_cast<size_t>(fs.tellg());
		fs.seekg(0, std::ifstream::beg);

		buffer.resize(fileSize);
		fs.read(buffer.data(), fileSize);

		fs.close();
	}

	return buffer;
}

BMFontConfiguration* BMFontConfiguration::create(std::string_view fntFile)
{
	BMFontConfiguration* result = new BMFontConfiguration();
	if (result->initWithFNTFile(fntFile)) {
		return result;
	}
	delete result;
	return nullptr;
}

static BMFontConfiguration* FNTConfigLoadFile(std::string_view fntFile)
{
	BMFontConfiguration* result = nullptr;
	result = BMFontConfiguration::create(fntFile);
	return result;
}

BMFontConfiguration::BMFontConfiguration()
	: m_charactorSet(nullptr)
	, m_commonHeight(0)
	, m_fontSize(0)
	, m_padding()
{
}

BMFontConfiguration::~BMFontConfiguration()
{
	delete m_charactorSet;
	m_charactorSet = nullptr;
}

bool BMFontConfiguration::initWithFNTFile(std::string_view fntFile)
{
	m_charactorSet = parseConfigFile(fntFile);

	if (m_charactorSet == nullptr) {
		return false;
	}

	return true;
}

std::set<unsigned int>* BMFontConfiguration::parseConfigFile(std::string_view controlFile)
{
	std::string data = getStringFromFile(controlFile);
	if (data.empty()) {
		return nullptr;
	}
	if (data.size() >= (sizeof("BMP") - 1) && memcmp("BMF", data.c_str(), sizeof("BMP") - 1) == 0) {
		return parseBinaryConfigFile(reinterpret_cast<unsigned char*>(&data.front()), static_cast<uint32_t>(data.size()), controlFile);
	}

	printf("Currently unsupported bitmap font.\n");
	return nullptr;
}

std::set<unsigned int>* BMFontConfiguration::parseBinaryConfigFile(unsigned char* pData, uint32_t size, std::string_view controlFile)
{
	std::set<unsigned int>* validCharsString = new std::set<unsigned int>();

	uint32_t remains = size;

	pData += 4;
	remains -= 4;

	while (remains > 0) {
		FntBlockType blockId = static_cast<FntBlockType>(pData[0]);
		pData += 1;
		remains -= 1;
		uint32_t blockSize = 0;
		memcpy(&blockSize, pData, 4);

		pData += 4;
		remains -= 4;

		if (blockId == FntBlockType::Info) {
			FntInfo info = { 0 };
			memcpy(&info, pData, sizeof(FntInfo));
			m_fontSize = info.fontSize;
			m_padding.top = info.paddingUp;
			m_padding.right = info.paddingRight;
			m_padding.bottom = info.paddingDown;
			m_padding.left = info.paddingLeft;

			const char* fontName = (const char*)pData + sizeof(FntInfo);
			m_fontName = fontName;
		}
		else if (blockId == FntBlockType::Common) {
			FntCommon common = { 0 };
			memcpy(&common, pData, sizeof(FntInfo));
			m_commonHeight = common.lineHeight;
		}
		else if (blockId == FntBlockType::Pages) {
			const char* value = (const char*)pData;
			assert(strlen(value) < blockSize);

			m_atlasName = value;
		}
		else if (blockId == FntBlockType::Chars) {
			FntChars fntChar = { 0 };

			const uint32_t count = blockSize / 20;
			for (uint32_t i = 0; i < count; i++) {
				memcpy(&fntChar, pData + (i * sizeof(FntChars)), sizeof(FntChars));
				BMFontDef& fontDef = m_fontDefDictionary[fntChar.id];
				fontDef.charID = fntChar.id;
				fontDef.rect.m_position.x = fntChar.x;
				fontDef.rect.m_position.y = fntChar.y;
				fontDef.rect.m_size.x = fntChar.width;
				fontDef.rect.m_size.y = fntChar.height;
				fontDef.xOffset = fntChar.xoffset;
				fontDef.yOffset = fntChar.yoffset;
				fontDef.xAdvance = fntChar.xadvance;

				validCharsString->insert(fontDef.charID);
			}
		}
		else if (blockId == FntBlockType::KerningPairs) {

		}
		else {

		}

		pData += blockSize;
		remains -= blockSize;
	}

	return validCharsString;
}

FontFNT* FontFNT::create(std::string_view fntFilePath)
{
	BMFontConfiguration* config = FNTConfigLoadFile(fntFilePath);
	if (config == nullptr) {
		return nullptr;
	}

	FontFNT* font = new FontFNT(config);
	if (font->init()) {
		return font;
	}
	delete font;
	return nullptr;
}

FontFNT::FontFNT(BMFontConfiguration* config)
	: m_pConfiguration(config)
{
}

FontFNT::~FontFNT()
{
	delete m_pConfiguration;
	m_pConfiguration = nullptr;
}

bool FontFNT::init()
{
	if (m_pConfiguration == nullptr) {
		return false;
	}

	m_fontName = m_pConfiguration->m_fontName;

	setLineHeight(static_cast<float>(m_pConfiguration->m_commonHeight));

	for (auto&& e : m_pConfiguration->m_fontDefDictionary) {
		BMFontConfiguration::BMFontDef& fontDef = e.second;

		FontCharacterDefinition definition = { 0 };

		definition.x = fontDef.rect.m_position.x;
		definition.y = fontDef.rect.m_position.y;
		definition.width = fontDef.rect.m_size.x;
		definition.height = fontDef.rect.m_size.y;
		definition.xoffset = fontDef.xOffset;
		definition.yoffset = fontDef.yOffset;
		definition.xadvance = fontDef.xAdvance;

		addCharacterDefinition(fontDef.charID, definition);
	}

	m_texture = TEXTURE_FONT_BMP;

	return true;
}
