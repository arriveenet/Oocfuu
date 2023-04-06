#pragma once

/*
* Special key code
*/
#define KEYBOARD_KEY_F1				0x0080
#define KEYBOARD_KEY_F2				0x0081
#define KEYBOARD_KEY_F3				0x0082
#define KEYBOARD_KEY_F4				0x0083
#define KEYBOARD_KEY_F5				0x0084
#define KEYBOARD_KEY_F6				0x0085
#define KEYBOARD_KEY_F7				0x0086
#define KEYBOARD_KEY_F8				0x0087
#define KEYBOARD_KEY_F9				0x0088
#define KEYBOARD_KEY_F10			0x0089
#define KEYBOARD_KEY_F11			0x008A
#define KEYBOARD_KEY_F12			0x008B
#define KEYBOARD_KEY_LEFT			0x008C
#define KEYBOARD_KEY_UP				0x008D
#define KEYBOARD_KEY_RIGHT			0x008E
#define KEYBOARD_KEY_DOWN			0x008F

#define KEYBOARD_SPECIAL_KEY_BOUND	0x007F

#define KEYBOARD_KEY_MAX			0x00FF

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