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
	: m_dead(false)
	, m_falling(false)
	, m_state(NOKONOKO_STATE_RUN)
	, m_counter(0)
	, m_speed(-NOKONOKO_RUN_SPEED, 0)
	, m_pStateMachine(nullptr)
	, m_topPoint(0.0f, 0.0f)
	, m_rightPoint(0.0f, 0.0f)
	, m_leftPoint(0.0f, 0.0f)
	, m_bottomPoints{ {0.0f, 0.0f} }
	,Sprite(NOKONOKO_RUN_SIZE, vec2(_x, _y))
{
	m_pStateMachine = new StateMachine<Nokonoko>(this);
	m_pStateMachine->setCurrentState(NokonokoStateRun::instance());

	m_animationController.setAnimation(ANIMATION_NOKONOKO_RUN);
}

Nokonoko::Nokonoko(const Nokonoko& _nokonoko)
	: m_dead(false)
	, m_falling(false)
	, m_state(NOKONOKO_STATE_RUN)
	, m_counter(0)
	, m_speed(-NOKONOKO_RUN_SPEED, 0)
	, m_pStateMachine(nullptr)
	, m_topPoint(0.0f, 0.0f)
	, m_rightPoint(0.0f, 0.0f)
	, m_leftPoint(0.0f, 0.0f)
	, m_bottomPoints{ {0.0f, 0.0f} }
{
	m_position = _nokonoko.m_position;
	m_size = _nokonoko.m_size;

	m_pStateMachine = new StateMachine<Nokonoko>(this);
	m_pStateMachine->setCurrentState(NokonokoStateRun::instance());

	m_animationController.setAnimation(ANIMATION_NOKONOKO_RUN);
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
	if ((m_dead)
		|| (g_courseManager.getScroll() > m_position.x + m_size.x + (PART_SIZE * 4))
		|| (g_courseManager.getScroll() + SCREEN_WIDTH + (PART_SIZE * 4) < m_position.x)) {
		return;
	}

	// アニメーションを更新
	m_animationController.update();

	// ステートマシンを更新
	m_pStateMachine->update();

	// スピードの影響を与える
	m_position += m_speed;

	// 当たり判定用のポイントを設定
	m_topPoint = m_position + vec2(8, 0);
	m_leftPoint = m_position + vec2(0, m_size.y - PART_SIZE);
	m_rightPoint = m_position + vec2(m_size.x, m_size.y - PART_SIZE);
	m_bottomPoints[0] = m_position + m_size;
	m_bottomPoints[1] = m_position + vec2(0, m_size.y);

	if (g_player.intersect(m_topPoint)) {
		m_pStateMachine->changeState(NokonokoStateShell::instance());
		g_player.jump();
	}

	//if (g_player.intersect(m_leftPoint)
	//	|| g_player.intersect(m_rightPoint)) {
	//	g_player.kill();
	//}

	// コースとの当たり判定を取る
	if (g_courseManager.intersect(m_leftPoint)) {
		trun();
		m_flip = RECT_FLIP_HORIZONTAL;

		if (m_state == NOKONOKO_STATE_SPIN) {
			g_pSound->play(SOUND_SE_BUMP);
		}
	}

	if (g_courseManager.intersect(m_rightPoint)) {
		trun();
		m_flip = RECT_FLIP_NONE;

		if (m_state == NOKONOKO_STATE_SPIN) {
			g_pSound->play(SOUND_SE_BUMP);
		}
	}

	for (int i = 0; i < NOKONOKO_BOTTOM_COUNT; i++) {
		if (g_courseManager.intersect(m_bottomPoints[i])) {
			vec2 bottom = ((ivec2)m_bottomPoints[i] / PART_SIZE) * PART_SIZE;
			m_position.y = bottom.y - m_size.y;
			m_speed.y = 0;
			m_falling = false;
			break;
		}
	}
}

void Nokonoko::draw()
{
	g_textureManager.setTexture((TEXTURE)g_animations[m_animationController.m_animation].m_keys[m_animationController.m_time]);
	Rect::draw();
	g_textureManager.unbindTexture();

	if (Game::m_debugInfo) {
		Rect::drawWire();

		glPushClientAttrib(GL_CLIENT_ALL_ATTRIB_BITS);// GLbitfield mask
		glPushAttrib(GL_ALL_ATTRIB_BITS);// GLbitfield mask
		glDisable(GL_TEXTURE_2D);// GLenum cap
		glColor3ub(0xff, 0x00, 0x00);
		glEnableClientState(GL_VERTEX_ARRAY);// GLenum array
		
		glBegin(GL_POINTS);
		glVertex2fv((GLfloat*)&m_topPoint);
		glVertex2fv((GLfloat*)& m_rightPoint);
		glVertex2fv((GLfloat*)&m_leftPoint);
		glEnd();

		glColor3ub(0x00, 0xff, 0x00);
		glVertexPointer(
			2,						// GLint size
			GL_FLOAT,				// GLenum type
			0,						// GLsizei stride
			&m_bottomPoints);	// const GLvoid * pointer
		glDrawArrays(
			GL_POINTS,				// GLenum mode
			0,						// GLint first
			NOKONOKO_BOTTOM_COUNT);	// GLsizei count

		glPopAttrib();
		glPopClientAttrib();
	}
}
