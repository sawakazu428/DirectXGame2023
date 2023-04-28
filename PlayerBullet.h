#pragma once
#include "Model.h"
#include"Vector3.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
#include "cassert"
class PlayerBullet 
{
public:
	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="model">モデル</param>
	/// <param name="position">初期座標</param>
	void Initialize(Model* model ,const Vector3& pos);

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	/// <param name="viewProjection">ビュープロジェクション</param>
	void Draw(const ViewProjection& view);

	private:
	// ワールドトランスフォーム
	WorldTransform world_;
	// 3Dモデル
	Model* model_;
	// テクスチャハンドル
	uint32_t textureHandle_ ;
};
