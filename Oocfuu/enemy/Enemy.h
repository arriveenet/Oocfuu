#pragma once

enum ENEMYTYPE {
	ENEMYTYPE_KURIBO,
	ENEMYTYPE_MAX
};

typedef struct tagENEMY {
	unsigned char id;	// �G�̎��
	unsigned short x;	// �G��X���W
	unsigned short y;	// �G��Y���W
} ENEMY;