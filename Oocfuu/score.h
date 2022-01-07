#pragma once
#include "audio.h"

enum Key {
	A,
	A_SHARP,
	B,
	C,
	C_SHARP,
	D,
	D_SHARP,
	E,
	F,
	F_SHARP,
	G,
	G_SHARP,
	REST,
	SCALE_MAX
};

const float octave[] = { 27.5, 55, 110, 220, 440, 880, 1760, 3520, 7040, 0 };

const char scale[][3] = { "A", "A#", "B", "C", "C#", "D", "D#", "E", "F", "F#", "G", "G#" ,"*" };

typedef struct {
	Key key;
	int oct;
	unsigned int length;
	float decay;
	float freq;
}SCORE;