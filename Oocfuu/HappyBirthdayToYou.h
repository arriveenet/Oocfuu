#pragma once

#include "score.h"

//#define HBTY_QUARTER_NOTE	32
#define HBTY_NAME_COUNT	96

SCORE hbtyPulse0[] = {

	{C,5,24}, {C,5,8},
	{B,4,32}, {G,4,32}, {A,4,32},
	{G,4,64}, {D,4,24}, {D,4,8},
	{E,4,32}, {D,4,32}, {G,4,32},
	{F_SHARP,4,64}, {D,4,24}, {D,4,8},
	{E,4,32}, {D,4,32}, {A,4,32},
	{G,4,64}, {D,4,24}, {D,4,8},
	{D,5,32}, {B,4,32}, {G,4,32},
	{F_SHARP,4,32}, {E,4,HBTY_NAME_COUNT},{C,5,24}, {C,5,8},
	{B,4,32}, {G,4,32}, {A,4,32},
	{G,4,512},
	{REST,9,64},
};

SCORE hbtyPulse1[] = {

	{REST,9,32},
	{G,3,32}, {B,3,32}, {F_SHARP,3,32},
	{G,3,32}, {G,2,32}, {REST,3,32},
	{G,3,32}, {B,3,32}, {B,3,32},
	{D,3,32}, {A,3,32}, {F_SHARP,3,32},
	{D,3,32}, {F_SHARP,3,32}, {F_SHARP,3,32},
	{G,3,32}, {G,2,32}, {REST,3,32},
	{G,2,32}, {A,2,32}, {B,2,32},
	{C,3,32}, {E,3,HBTY_NAME_COUNT}, {F_SHARP,3,32},
	{G,3,32}, {B,3,32}, {D,3,32},
	{G,3,32}, {G,2,32},
};

SCORE hbtyTri[] = {
	{REST,9,32},
	{REST,9,32}, {D,4,32}, {C,4,32},
	{B,3,32}, {REST,9,64},
	{REST,9,96},
	{REST,9,32}, {C,4,32}, {REST,9,32},
	{REST,9,32}, {A,3,32}, {A,3,32},
	{REST,9,96},
	{REST,9,96},
	{REST,9,32}, {G,3,HBTY_NAME_COUNT}, {REST,9,32},
	{REST,9,32}, {D,4,32}, {REST,9,32},
	{REST,9,96},
};

#define HBTY_PULSE0_COUNT sizeof(hbtyPulse0) / sizeof(hbtyPulse0[0])
#define HBTY_PULSE1_COUNT sizeof(hbtyPulse1) / sizeof(hbtyPulse1[0])
#define HBTY_TRI_COUNT sizeof(hbtyTri) / sizeof(hbtyTri[0])

extern SCORE hbtyPulse0[];
extern SCORE hbtyPulse1[];
extern SCORE hbtyTri[];