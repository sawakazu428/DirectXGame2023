#pragma once
#include "iostream"
#include "functional"
class TimedCall {
public:
	// コンストラクタ
	TimedCall(std::function<void(void)> f, uint32_t time);
	// 更新
	void Update();
	// 完了ならtrueを返す
	bool IsFinished() { return isComp; }

private:
	// コールバック
	std::function<void(void)> f;
	// 残り時間
	uint32_t time_;

	// 完了フラグ
	bool isComp = false;

};  
