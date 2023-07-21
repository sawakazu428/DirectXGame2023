#pragma once
#include "Model.h"
#include "Vector3.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
#include "cassert"
#include "Collider.h"

class EnemyBullet : public Collider {
public:
	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="model">モデル</param>
	/// <param name="position">初期座標</param>
	void Initialize(Model* model, const Vector3& pos, const Vector3& velocity);

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	/// <param name="viewProjection">ビュープロジェクション</param>
	void Draw(const ViewProjection& view);

	// 衝突を検出したら呼び出されるコールバック関数
	void EnemyBulletOnColision();

	// ワールド座標を取得
	Vector3 GetWorldEnemyBulletPosition();


	// デスフラグのゲッター
	bool IsDead() const { return isEnemyBulletDead_; }

	const float GetEnemyBulletRadius() { return enemyBulletRadius; }
	const float enemyBulletRadius = 1.0f;

		// 衝突時に呼ばれる関数
	void OnCollision() override;

	// 衝突時に呼ばれる関数
	Vector3 GetWorldPosition() override;


private:
	// ワールドトランスフォーム
	WorldTransform worldEnemyBullet_;
	// 3Dモデル
	Model* modelEnemyBullet_;
	// テクスチャハンドル
	uint32_t textureHandleEnemyBullet_;
	// 速度
	Vector3 velocityEnemyBullet_;

	// 寿命<frm>
	static const int32_t kEnemyBulletLifeTime = 60 * 5;

	// デスタイマー
	int32_t deathEnemyBulletTimer_ = kEnemyBulletLifeTime;
	// デスフラグ
	bool isEnemyBulletDead_ = false;
};
