#pragma once
#include "Model.h"
#include "Vector3.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
#include "cassert"

class EnemyBullet 
{
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

	// デスフラグのゲッター
	bool IsDead() const { return isEnemyBulletDead_; }

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
