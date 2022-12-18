#pragma once

enum ENEMYTYPE {
	ENEMYTYPE_KURIBO,
	ENEMYTYPE_MAX
};

typedef struct tagENEMY {
	unsigned char id;	// 敵の種類
	unsigned short x;	// 敵のX座標
	unsigned short y;	// 敵のY座標
} ENEMY;