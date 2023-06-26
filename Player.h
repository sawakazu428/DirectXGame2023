#pragma once
#include "Model.h"
#include "WorldTransform.h"
#include "Input.h"
#include "PlayerBullet.h"
#include<list>
#include "MathUtility.h"
#include "Matrix4x4.h"
#include "Sprite.h"




/// <summary>
/// 自キャラ
/// </summary>
class Player {
public:

	/// <summary>
	/// デストラクタ
	/// </summary>
	~Player();

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="model">モデル</param>
	/// <param name="textureHandle">テクスチャハンドル</param>
	void Initialize(Model* model, uint32_t textureHandle,Vector3 pos);

	/// <summary>
	/// 更新
	/// </summary>
	void Update(ViewProjection& view);

	/// <summary>
	/// 描画
	/// </summary>
	
	/// <summary>
	/// 
	/// </summary>
	/// <param name="ViewProjection">ビュープロジェクション(参照渡し)</param>
	void Draw(ViewProjection& view);

	void DrawUI();

	void Attack();

	// 衝突を検出したら呼び出されるコールバック関数
	void PlayerOnColision();

	void SetParent(const WorldTransform* parent);

	// ワールド座標を取得
	Vector3 GetWorldPlayerPosition();

	// 弾リストを取得
	const std::list<PlayerBullet*>& GetPlayerBullets() { return playerBullets_; }

	const float GetPlayerRadius() { return playerRadius; }
	const float playerRadius = 1.0f;

private:
	
	
	// ワールド変換データ
	WorldTransform worldTransform_;

	// 3Dレティクル用ワールドトランスフォーム
	WorldTransform worldTransform3Dreticle_;

	ViewProjection viewProjection_;
	// モデル
	Model* model_ = nullptr;
	Model* ReticleModel_ = nullptr;


	// テクスチャハンドル
	uint32_t textureHandle_ = 0u;
	// キーボード入力
	Input* input_ = nullptr;

	// 弾
	//PlayerBullet* bullet_ = nullptr;
	std::list<PlayerBullet*> playerBullets_;

	Sprite* sprite2DReticle_ = nullptr;

};
