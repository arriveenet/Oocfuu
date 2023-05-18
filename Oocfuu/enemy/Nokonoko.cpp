#include "Nokonoko.h"

#include "Game.h"
#include "TextureManager.h"
#include "world/Part.h"
#include "world/Course.h"
#include "player/Player.h"
#include "animation/Animation.h"
#include "sound/Sound.h"

using namespace glm;
using std::vector;

Nokonoko::Nokonoko()
	: Nokonoko(0.0f, 0.0f)
{
}

Nokonoko::Nokonoko(glm::vec2 _position)
	: Nokonoko(_position.x, _position.y)
{
}

Nokonoko::Nokonoko(float _x, float _y)
	: m_state(NOKONOKO_STATE_RUN)
	, m_counter(0)
	, m_pStateMachine(nullptr)
	, m_topPoint(0.0f, 0.0f)
	, Enemy(NOKONOKO_RUN_SIZE, vec2(_x, _y))
{
	m_pStateMachine = new StateMachine<Nokonoko>(this);
	m_pStateMachine->setCurrentState(NokonokoStateRun::instance());

	m_animationController.setAnimation(ANIMATION_NOKONOKO_RUN);
}

Nokonoko::Nokonoko(const Nokonoko& _nokonoko)
	: Nokonoko(_nokonoko.m_position)
{
}

Nokonoko::~Nokonoko()
{
	if (m_pStateMachine) {
		delete m_pStateMachine;
		m_pStateMachine = nullptr;
	}
}

void Nokonoko::update()
{
	// アニメーションを更新
	m_animationController.update();

	// ステートマシンを更新
	m_pStateMachine->update();

	// 当たり判定用のポイントを設定
	m_topPoint = m_position + vec2(8, 0);

	if (g_player.intersect(m_topPoint)) {
		m_pStateMachine->changeState(NokonokoStateShell::instance());
		g_player.jump();
	}

	//if (g_player.intersect(m_leftPoint)
	//	|| g_player.intersect(m_rightPoint)) {
	//	g_player.kill();
	//}
}

void Nokonoko::draw()
{
	g_textureManager.setTexture((TEXTURE)g_animations[m_animationController.m_animation].m_keys[m_animationController.m_time]);
	Rect::draw();
	g_textureManager.unbindTexture();

	if (Game::m_debugInfo) {
		glPushClientAttrib(GL_CLIENT_ALL_ATTRIB_BITS);// GLbitfield mask
		glPushAttrib(GL_ALL_ATTRIB_BITS);// GLbitfield mask
		glDisable(GL_TEXTURE_2D);// GLenum cap
		glColor3ub(0xff, 0x00, 0x00);
		glEnableClientState(GL_VERTEX_ARRAY);// GLenum array
		
		glBegin(GL_POINTS);
		glVertex2fv((GLfloat*)&m_topPoint);
		glEnd();

		glPopAttrib();
		glPopClientAttrib();
	}

	Enemy::draw();
}

void Nokonoko::kill()
{
	m_pStateMachine->changeState(NokonokoStateDie::instance());
}
