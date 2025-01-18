#pragma once
#include <string>
#include <map>

template<class Base>
class CreatorBase {
public:
	virtual ~CreatorBase() {}
	virtual Base* create() = 0;
};

template<class Product, class Base>
class Creator :public CreatorBase<Base> {
public:
	Base* create() override { return new Product; }
};

template<class Base>
class Factory {
public:
	Base* create(std::string_view type);
	bool registerEntity(std::string_view type, CreatorBase<Base>* pCreator);

private:
	std::map<std::string, CreatorBase<Base>*> m_creatorMap;
};

template<class Base>
inline Base* Factory<Base>::create(std::string_view type)
{
	auto iter = m_creatorMap.find(type.data());
	if (iter == m_creatorMap.end()) {
		return nullptr;
	}

	CreatorBase<Base>* pCreator = (*iter).second;
	return pCreator->create();
}

template<class Base>
inline bool Factory<Base>::registerEntity(std::string_view type, CreatorBase<Base>* pCreator)
{
	auto iter = m_creatorMap.find(type.data());
	if (iter != m_creatorMap.end()) {
		delete pCreator;
		return false;
	}
	m_creatorMap[type.data()] = pCreator;
	return true;
}
