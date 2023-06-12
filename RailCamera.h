#pragma once
#include "WorldTransform.h"
#include "ViewProjection.h"
#include <ImGuiManager.h>
class RailCamera 
{
public:

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(Vector3 trans, Vector3 rot);

	/// <summary>
	/// 更新処理
	/// </summary>
	void Update();

	ViewProjection& GetViewProjection() { return railViewProjection_; }
	WorldTransform& GetWorldTransform() { return railWorldTransform_; }

private:
	// ワールド変換データ
	WorldTransform railWorldTransform_;

	// ビュープロジェクション
	ViewProjection railViewProjection_;

};
