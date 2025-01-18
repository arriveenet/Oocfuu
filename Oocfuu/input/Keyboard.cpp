#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <GL/glut.h>

#include "Keyboard.h"

bool Keyboard::m_pressed[KEYBOARD_KEY_MAX];
bool Keyboard::m_lastPressed[KEYBOARD_KEY_MAX];
bool Keyboard::m_changed[KEYBOARD_KEY_MAX];
bool Keyboard::m_nowPressed[KEYBOARD_KEY_MAX];
bool Keyboard::m_released[KEYBOARD_KEY_MAX];
bool Keyboard::m_nowReleased[KEYBOARD_KEY_MAX];

void keyboard(unsigned char key, int x, int y) {
	//printf("keybord:\'%c\' (%#x)\n", key, key);
	Keyboard::m_pressed[key] = true;
}

void keyboardUp(unsigned char key, int x, int y) {
	//printf("keybordUp:\'%c\' (%#x)\n", key, key);
	Keyboard::m_pressed[key] = false;
}

void special(int key, int x, int y)
{
	int keyCode = 0;
	switch (key) {
	case GLUT_KEY_LEFT:
		keyCode = KEYBOARD_KEY_LEFT;
		break;
	case GLUT_KEY_UP:
		keyCode = KEYBOARD_KEY_UP;
		break;
	case GLUT_KEY_RIGHT:
		keyCode = KEYBOARD_KEY_RIGHT;
		break;
	case GLUT_KEY_DOWN:
		keyCode = KEYBOARD_KEY_DOWN;
		break;
	default:
		keyCode = KEYBOARD_SPECIAL_KEY_BOUND + key;
		break;
	}
	Keyboard::m_pressed[keyCode] = true;
}

void specialUp(int key, int x, int y)
{
	int keyCode = 0;
	switch (key) {
	case GLUT_KEY_LEFT:
		keyCode = KEYBOARD_KEY_LEFT;
		break;
	case GLUT_KEY_UP:
		keyCode = KEYBOARD_KEY_UP;
		break;
	case GLUT_KEY_RIGHT:
		keyCode = KEYBOARD_KEY_RIGHT;
		break;
	case GLUT_KEY_DOWN:
		keyCode = KEYBOARD_KEY_DOWN;
		break;
	default:
		keyCode = KEYBOARD_SPECIAL_KEY_BOUND + key;
		break;
	}
	Keyboard::m_pressed[keyCode] = false;
}

int Keyboard::init() {
	glutKeyboardFunc(keyboard);		// void (GLUTCALLBACK *func)(unsigned char key, int x, int y)
	glutKeyboardUpFunc(keyboardUp);	// void (GLUTCALLBACK *func)(unsigned char key, int x, int y))
	glutSpecialFunc(special);		// void (* callback)( int, int, int )
	glutSpecialUpFunc(specialUp);	// void (* callback)( int, int, int )

	glutIgnoreKeyRepeat(GL_TRUE); //int ignore

	return 0;
}

void Keyboard::begin() {
	for (int i = 0; i < KEYBOARD_KEY_MAX; i++) {
		m_changed[i] = (m_pressed[i] != m_lastPressed[i]);
		m_nowPressed[i] = (m_changed[i] && m_pressed[i]);
		m_released[i] = !m_pressed[i];
		m_nowReleased[i] = (m_changed[i] && m_released[i]);
	}
}

void Keyboard::end() {
	memcpy(m_lastPressed, m_pressed, sizeof m_pressed);
}