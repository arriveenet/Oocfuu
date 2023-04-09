#pragma once
#include <string>
#include <glm/glm.hpp>

typedef struct tagMESSAGE{
	std::string string;
	glm::vec2 position;
	int startCount;
} MESSAGE;

// メッセージ制御クラス
class MessageController {
public:
	// コンストラクタ
	MessageController();

	// デストラクタ
	virtual ~MessageController();

	// 更新
	void update();

	// 描画
	void draw();

	// メッセージをスタート
	void start() { m_update = true; m_count = 0; };

	// メッセージをリセット
	void reset() { m_update = false; m_count = 0; };

	// メッセージを設定する
	void setMessage(MESSAGE* _messages, int _size);

private:
	unsigned int m_count;	// カウンター
	int m_messageSize;		// メッセージサイズ
	MESSAGE* m_messages;	// メッセージリスト
	bool m_update;			// 更新フラグ
};

