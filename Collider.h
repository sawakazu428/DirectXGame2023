#pragma once
class Collider 
{
private:
	// 衝突半径
	float radius = 30;

public:
	// 半径を取得
	const float GetRedius() { return radius; }
	// 半径を設定
	void SetRadius(float radius);
	// 衝突時に呼ばれる関数
	virtual void OnCollision();
	// ワールド座標を取得
	virtual void GetWorldPosition() = 0;
};
