#pragma once

#define KEYBOARD_KEY_MAX	256
struct Keyboard {
	static bool m_pressed[KEYBOARD_KEY_MAX];
	static bool m_lastPressed[KEYBOARD_KEY_MAX];
	static bool m_changed[KEYBOARD_KEY_MAX];
	static bool m_nowPressed[KEYBOARD_KEY_MAX];
	static bool m_released[KEYBOARD_KEY_MAX];
	static bool m_nowReleased[KEYBOARD_KEY_MAX];

	static int init();
	static void begin();
	static void end();
};