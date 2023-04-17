#pragma once

#include "Audio.h"
#include "DirectXCommon.h"
#include "Input.h"
#include "Model.h"
#include "SafeDelete.h"
#include "Sprite.h"
#include "ViewProjection.h"
#include "WorldTransform.h"

/// <summary>
/// ゲームシーン
/// </summary>
class GameScene {

public: // メンバ関数
	/// <summary>
	/// コンストクラタ
	/// </summary>
	GameScene();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~GameScene();

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

private: // メンバ変数
	DirectXCommon* dxCommon_ = nullptr;
	Input* input_ = nullptr;
	Audio* audio_ = nullptr;
	uint32_t textureHandle_ = 0;
	//Sprite* sprite_ = nullptr; //2Dモデルの変数宣言

	//Model* model_ = nullptr; //3Dモデルの変数宣言
	//// ワールドトランスフォーム
	//WorldTransform worldTransform_;
	//// ビュープロジェクション
	//ViewProjection viewProjection_;

	//サウンドデータハンドル
	uint32_t soundDataHandle_ = 0; // サウンドデータの変数宣言
	uint32_t
	/// <summary>
	/// ゲームシーン用
	/// </summary>
};
