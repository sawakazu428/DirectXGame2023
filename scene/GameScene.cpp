#include "GameScene.h"
#include "TextureManager.h"
#include "AxisIndicator.h"
#include <cassert>

GameScene::GameScene() {}

GameScene::~GameScene() 
{
	delete debugCamera_;
	delete enemy_;
	delete player_;
	delete model_;
	delete skydome_;
	delete modelSkydome_;
}


void GameScene::Initialize() {

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();

		// ファイル名を	指定してテクスチャを読み込む
	textureHandle_ = TextureManager::Load("sample.png");
	// 3Dモデルの作成
	model_ = Model::Create();
	// ワールドトランスフォームの初期化
	worldTransform_.Initialize();
	// ビュープロジェクションの初期化
	viewProjection_.Initialize();
	// 自キャラの作成
	player_ = new Player();
	// 自キャラの初期化
	player_->Initialize(model_, textureHandle_);

	//敵の作成
	enemy_ = new Enemy();
	//敵の初期化
	enemy_->Initialize(model_, {5,0,50});
	// 敵キャラに自キャラのアドレスを渡す
	enemy_->SetPlayer(player_);

	// 3Dモデルの作成
	modelSkydome_ = Model::CreateFromOBJ("skydome", true);
	// 天球の作成
	skydome_ = new Skydome();
	// 天球の初期化
	skydome_->Initialize(modelSkydome_);

	debugCamera_ = new DebugCamera(WinApp::kWindowWidth, WinApp::kWindowHeight);

	// 軸方向表示の表示を有効にする
	AxisIndicator::GetInstance()->SetVisible(true);
	// 軸方向表示が参照するビュープロジェクションを指定する(アドレス渡し)
	AxisIndicator::GetInstance()->SetTargetViewProjection(&viewProjection_);

}

void GameScene::Update() {

	player_->Update();

	debugCamera_->Update();

	skydome_->Update();

	CheckAllColisions();
	// ポインタがnullでない(有効である)時だけ行う
	if (enemy_) // if(Enemy_ != nullptr)と同じ効果になる
	{
		enemy_->Update();
	}
#ifdef _DEBUG
	if (input_->TriggerKey(DIK_V)) {
		isDebugCameraActive_ = true;
	}

#endif // DEBUG

	// カメラの処理
	if (isDebugCameraActive_) {
		debugCamera_->Update();
		viewProjection_.matView = debugCamera_->GetViewProjection().matView;
		viewProjection_.matProjection = debugCamera_->GetViewProjection().matProjection;
		viewProjection_.TransferMatrix();
	} else {
		viewProjection_.UpdateMatrix();
	}
}

void GameScene::Draw() {

	// コマンドリストの取得
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();

#pragma region 背景スプライト描画
	// 背景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに背景スプライトの描画処理を追加できる
	/// </summary>

	// スプライト描画後処理
	Sprite::PostDraw();
	// 深度バッファクリア
	dxCommon_->ClearDepthBuffer();
#pragma endregion

#pragma region 3Dオブジェクト描画
	// 3Dオブジェクト描画前処理
	Model::PreDraw(commandList);

	/// <summary>
	/// ここに3Dオブジェクトの描画処理を追加できる
	/// </summary>
	player_->Draw(viewProjection_);

	// ポインタがnullでない(有効である)時だけ行う
	if (enemy_) // if(Enemy_ != nullptr)と同じ効果になる
	{
		enemy_->Draw(viewProjection_);
	}

	skydome_->Draw(viewProjection_);
	
	// 3Dオブジェクト描画後処理
	Model::PostDraw();
#pragma endregion

#pragma region 前景スプライト描画
	// 前景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに前景スプライトの描画処理を追加できる
	/// </summary>

	// スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion
}

void GameScene::CheckAllColisions() 
{ 
	// 判定対象AとBの座標
	Vector3 posA, posB;

	// 自弾リストの取得
	const std::list<PlayerBullet*>& playerBullets = player_->GetPlayerBullets();
	// 敵弾リストの取得
	const std::list<EnemyBullet*>& enemyBullets = enemy_->GetEnemyBullets();

	#pragma region // 自キャラと敵弾の当たり判定
	// 自キャラのワールド座標
	posA = player_->GetWorldPlayerPosition();

	// 自キャラと敵弾全ての当たり判定
	for (EnemyBullet* bullets : enemyBullets)
	{
		// 敵弾の座標
		posB = bullets->GetWorldEnemyBulletPosition();

		float distance = (posB.x - posA.x) * (posB.x - posA.x) +
		               (posB.y - posA.y) * (posB.y - posA.y) +
		               (posB.z - posA.z) * (posB.z - posA.z);

		float length = (player_->playerRadius + bullets->enemyBulletRadius) *
		               (player_->playerRadius + bullets->enemyBulletRadius);
		// 球と球の交差判定
		if (distance <= length)
		{
			// 自キャラの衝突時コールバックを呼び出す
			player_->PlayerOnColision();
			// 敵弾の衝突時コールバックを呼び出す
			bullets->EnemyBulletOnColision();
		}
	}	
	#pragma endregion

	#pragma region // 自弾と敵キャラの当たり判定
	// 敵キャラのワールド座標
	posA = enemy_->GetWorldEnemyPosition();

	// 敵キャラと自弾全ての当たり判定
	for (PlayerBullet* bullets : playerBullets) {
		// 自弾の座標
		posB = bullets->GetWorldPlayerBulletPosition();

		float distance = (posB.x - posA.x) * (posB.x - posA.x) +
		                 (posB.y - posA.y) * (posB.y - posA.y) +
		                 (posB.z - posA.z) * (posB.z - posA.z);

		float length = (enemy_->enemyRadius + bullets->playerBulletRadius) *
		               (enemy_->enemyRadius + bullets->playerBulletRadius);
		// 球と球の交差判定
		if (distance <= length) {
			// 敵キャラの衝突時コールバックを呼び出す
			enemy_->EnemyOnColision();
			// 自弾の衝突時コールバックを呼び出す
			bullets->PlayerBulletOnColision();
		}
	}	
    #pragma endregion

	#pragma region // 自弾と敵弾の当たり判定
	// 自キャラと敵弾全ての当たり判定
	for (EnemyBullet* enemybullets : enemyBullets) 
	{
		for (PlayerBullet* playerbullets : playerBullets)
		{
			// 自弾の座標
			posA = playerbullets->GetWorldPlayerBulletPosition();
			// 敵弾の座標
			posB = enemybullets->GetWorldEnemyBulletPosition();

			float distance = (posB.x - posA.x) * (posB.x - posA.x) +
			                 (posB.y - posA.y) * (posB.y - posA.y) +
			                 (posB.z - posA.z) * (posB.z - posA.z);

			float length = (playerbullets->playerBulletRadius + enemybullets->enemyBulletRadius) *
			               (playerbullets->playerBulletRadius + enemybullets->enemyBulletRadius);
			// 球と球の交差判定
			if (distance <= length) {
				// 自キャラの衝突時コールバックを呼び出す
				playerbullets->PlayerBulletOnColision();
				// 敵弾の衝突時コールバックを呼び出す
				enemybullets->EnemyBulletOnColision();
			}
		}
	}	
    #pragma endregion
}
