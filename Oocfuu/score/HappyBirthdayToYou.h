#pragma once

#include "score.h"

//#define HBTY_QUARTER_NOTE	32
#define HBTY_NAME_COUNT	96

static SCORE pulse0_score_happy_birthday[] = {

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

static SCORE pulse1_score_happy_birthday[] = {

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
	{G,3,32}, {G,2,64},
};

static SCORE triangle_score_happy_birthday[] = {
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

static constexpr size_t PULSE0_COUNT_HAPPY_BIRTHDAY = sizeof(pulse0_score_happy_birthday) / sizeof(pulse0_score_happy_birthday[0]);
static constexpr size_t PULSE1_COUNT_HAPPY_BIRTHDAY = sizeof(pulse1_score_happy_birthday) / sizeof(pulse1_score_happy_birthday[0]);
static constexpr size_t TRIANGLE_COUNT_HAPPY_BIRTHDAY = sizeof(triangle_score_happy_birthday) / sizeof(triangle_score_happy_birthday[0]);
