#pragma once

enum ENEMYTYPE {
	ENEMYTYPE_KURIBO,
	ENEMYTYPE_MAX
};

typedef struct tagENEMY {
	unsigned char id;	// “G‚Ìí—Ş
	unsigned short x;	// “G‚ÌXÀ•W
	unsigned short y;	// “G‚ÌYÀ•W
} ENEMY;