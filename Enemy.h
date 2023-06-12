#pragma once
#include "WorldTransform.h"
#include "MathUtility.h"
#include "Model.h"
#include "EnemyBullet.h"
#include<list>

// 自機クラスの前方宣言
class Player;

class Enemy {
public:
	// 発射間隔
	static const int kFireInterval = 60;
	~Enemy();

	void Initialize(Model* model, const Vector3& pos);

	void ApproachInitialze();

	void Update();

	void Draw(const ViewProjection& view);

	void ApproachUpdate(const float kEnemySpeed);

	void LeaveUpdate(const float kEnemySpeed);

	/// <summary>
	/// 弾発射
	/// </summary>
	void Fire();

	// 衝突を検出したら呼び出されるコールバック関数
	void EnemyOnColision();

	void SetPlayer(Player* player) { player_ = player; }

	// ワールド座標を取得
	Vector3 GetWorldEnemyPosition();

	// 弾リストを取得
	const std::list<EnemyBullet*>& GetEnemyBullets() { return enemyBullets_; }

	const float GetEnemyRadius() { return enemyRadius; }
	const float enemyRadius = 1.0f;

private:
	// 自キャラ
	Player* player_ = nullptr;
	// ワールドトランスフォーム
	WorldTransform worldTransformEnemy_;
	// 3Dモデル
	Model* modelEnemy_;
	// テクスチャハンドル
	uint32_t textureHandleEnemy_;

	// 速度
	Vector3 velocityEnemy_;

	// 発射タイマー
	int32_t shotTimer_ = 0;

	// 行動フェーズ
	enum class Phase {
		Approach, // 接近する
		Leave,    // 離脱する
	};
	Phase phase_ = Phase::Approach;

	// 弾
	std::list<EnemyBullet*> enemyBullets_;
};