#include "timer.h"
#include <Windows.h>

// 唯一のインスタンス
static Timer timer;

/*
 * @breif タイマーを初期化を行う
 * 
 * @param なし
 * 
 * @return 成功：true 失敗：false
 * 
 */
bool timerInit(void)
{
// コンパイラターゲットがWindowsではない場合falseを返す
#ifndef _WIN32
	return false;
#endif // !_WIN32

	// パフォーマンス カウンターの頻度を取得
	if (QueryPerformanceFrequency((LARGE_INTEGER*)&timer.frequency) == FALSE)
		return false;

	// オフセットを取得
	QueryPerformanceCounter((LARGE_INTEGER*)&timer.offset);

	return true;
}

/*
 * @breif プログラム開始からの時間を取得
 *
 * @param なし
 *
 * @return 経過時間
 *
 */
double getTime(void)
{
	return (double)(getTimerValue() - timer.offset) / timer.frequency;
}

/*
 * @breif パフォーマンスカウンターの現在の値を取得
 *
 * @param なし
 *
 * @return パフォーマンスカウンター
 *
 */
uint64_t getTimerValue(void)
{
	uint64_t value = 0;
	QueryPerformanceCounter((LARGE_INTEGER*)&value);
	return value;
}

/*
 * @breif パフォーマンス カウンターの頻度を取得
 *
 * @param なし
 *
 * @return パフォーマンスカウンターの頻度
 *
 */
uint64_t getTimerFrequency(void)
{
	return timer.frequency;
}
