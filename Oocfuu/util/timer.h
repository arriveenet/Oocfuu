#pragma once
#include <stdint.h>

/**
 * @brief タイマー構造体
 */
struct Timer {
	uint64_t frequency;
	uint64_t offset;
};

// タイマーを初期化
bool timerInit(void);

// プログラム開始からの時間を取得
double getTime(void);

// パフォーマンスカウンターの現在の値を取得
uint64_t getTimerValue(void);

// パフォーマンス カウンターの頻度を取得
uint64_t getTimerFrequency(void);