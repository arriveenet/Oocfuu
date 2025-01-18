#include "Actor.h"
#include "Game.h"
#include "Component.h"

Actor::Actor()
	: m_position(0.0f, 0.0f)
	, m_size(0.0f, 0.0f)
	, m_taransform(1.0f)
	, m_transformDirty(true)
	, m_pGame(Game::getInstance())
{
}

Actor::~Actor()
{
}

void Actor::update()
{
}

void Actor::update(float deltaTime)
{
}

void Actor::draw()
{
}

void Actor::setPosition(float x, float y)
{
	setPosition(glm::vec2(x, y));
}

void Actor::setPosition(const glm::vec2& position)
{
	if (m_position != position) {
		m_position = position;
		m_transformDirty = true;
	}
}

glm::vec2 Actor::getPosition() const
{
	return m_position;
}

void Actor::setSize(float x, float y)
{
	setSize(glm::vec2(x, y));
}

void Actor::setSize(const glm::vec2& size)
{
	if (m_size != size) {
		m_size = size;
		m_transformDirty = true;
	}
}

glm::vec2 Actor::getSize() const
{
	return m_size;
}

void Actor::addChild(Actor* pActor)
{
	assert(pActor != nullptr);
	m_children.emplace_back(pActor);
}

void Actor::removeChild(Actor* pActor)
{
	auto iter = std::find(m_children.begin(), m_children.end(), pActor);
	if (iter != m_children.end()) {
		delete pActor;
		m_children.erase(iter);
	}
}

void Actor::addComponent(Component* pComponent)
{
	m_componets.emplace_back(pComponent);
}

void Actor::removeComponent(Component* pComponent)
{
	auto iter = std::find(m_componets.begin(), m_componets.end(), pComponent);
	if (iter != m_componets.end()) {
		m_componets.erase(iter);
	}
}

void Actor::updateComponent()
{
	for (auto& component : m_componets) {
		component->update();
	}
}
