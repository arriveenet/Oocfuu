#pragma once
#include <vector>
#include <glm/glm.hpp>

class Game;
class Component;

class Actor {
public:
	Actor();
	virtual ~Actor();

	virtual void update();
	virtual void update(float deltaTime);

	virtual void draw();

	virtual void setPosition(float x, float y);

	virtual void setPosition(const glm::vec2& position);

	virtual glm::vec2 getPosition() const;

	virtual void setSize(float x, float y);

	virtual void setSize(const glm::vec2& size);

	virtual glm::vec2 getSize() const;

	void addChild(Actor* pActor);

	void removeChild(Actor* pActor);

	void addComponent(Component* pComponent);

	void removeComponent(Component* pComponent);

protected:
	void updateComponent();

protected:
	glm::vec2 m_position;
	glm::vec2 m_size;
	glm::mat4 m_transformMatrix;
	bool m_transformDirty;

	Game* m_pGame;

	std::vector<Actor*> m_children;
	std::vector<Component*> m_components;
};

