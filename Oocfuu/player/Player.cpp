#include "Player.h"
#include "PlayerStateRun.h"
#include "../TextureManager.h"

using namespace glm;

Player g_player;

Player::Player()
{
	m_size = { 16, 16 };
	m_position = { PLAYER_DEFAULT_X, PLAYER_DEFAULT_Y };
	m_pStateContext = new PlayerStateContext();
	m_pStateContext->setStete(new PlayerStateRun);
}

Player::~Player()
{
	delete m_pStateContext;
}

void Player::init()
{

}

void Player::reset()
{
}

void Player::update()
{
	m_pStateContext->update(this);
}

void Player::draw()
{
	g_textureManager.setTexture(TEXTURE_PLAYER_IDLE);
	Rect::draw();
	g_textureManager.unbindTexture();
}
