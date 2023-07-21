#pragma once
#include "Model.h"
#include"Vector3.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
#include "cassert"
class PlayerBullet : public Collider {
public:
	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="model">モデル</param>
	/// <param name="position">初期座標</param>
	void Initialize(Model* model ,const Vector3& pos, const Vector3& velocity);

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
	void PlayerBulletOnColision();

	// ワールド座標を取得
	Vector3 GetWorldPlayerBulletPosition();

	// デスフラグのゲッター
	bool IsDead() const { return isPlayerBulletDead_; }

	const float GetPlayerBulletRadius() { return playerBulletRadius; }
	const float playerBulletRadius = 1.0f;

	// 衝突時に呼ばれる関数
	void OnCollision() override;
	// 衝突時に呼ばれる関数
	void GetWorldPosition() override;


private:
	// ワールドトランスフォーム
	WorldTransform worldPlayerBullet_;
	// 3Dモデル
	Model* model_;
	// テクスチャハンドル
	uint32_t textureHandle_ ;
	// 速度
	Vector3 velocity_;

	// 寿命<frm>
	static const int32_t kLifeTime = 60 * 5;

	// デスタイマー
	int32_t deathTimer_ =  kLifeTime;
	// デスフラグ
	bool isPlayerBulletDead_ = false;

};
