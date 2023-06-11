#include "CourseLoader.h"


CourseLoader::CourseLoader()
{
	// COM‚Ì‰Šú‰»ˆ—
	HRESULT result = CoInitialize(NULL);
}

CourseLoader::~CourseLoader()
{
	// COM‚ÌI—¹ˆ—
	//CoUninitialize();
}

CourseLoader* CourseLoader::create()
{
	static CourseLoader instance;
	return &instance;
}

bool CourseLoader::initialize(const char* pFileName)
{
	MSXML2::IXMLDOMNodeList* nodeList = nullptr;

	m_pDocument.CreateInstance("Msxml2.DOMDocument.6.0");

	_variant_t xmlSource(pFileName);
	VARIANT_BOOL isSuccessful((bool)TRUE);
	
	HRESULT hResult = m_pDocument->load(xmlSource, &isSuccessful);

	BSTR tagName = ::SysAllocString(L"catalog");

	hResult = m_pDocument->getElementsByTagName(tagName, &nodeList);

	long a;
	nodeList->get_length(&a);


	::SysFreeString(tagName);

	return true;
}
