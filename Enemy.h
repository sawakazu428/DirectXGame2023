#pragma once
#include "WorldTransform.h"
#include "MathUtility.h"
#include "Model.h"
#include "EnemyBullet.h"
#include<list>

class Enemy
{
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
private:
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
enum class Phase 
{
	Approach, // 接近する
	Leave,    // 離脱する
};
	Phase phase_ = Phase::Approach;

// 弾
std::list<EnemyBullet*> enemyBullets_;
};

