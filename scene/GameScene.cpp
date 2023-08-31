#include "GameScene.h"
#include "TextureManager.h"
#include "AxisIndicator.h"
#include "fstream"
GameScene::GameScene() {}

GameScene::~GameScene() 
{
	delete debugCamera_;
	for (Enemy* enemy : enemy_)
	{
		delete enemy;
	}
	delete player_;
	delete model_;
	delete skydome_;
	delete modelSkydome_;
	delete railCamera_;
	for (EnemyBullet* bullet : enemyBullets_) {
		delete bullet;
	}
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
	Vector3 playerPosition(0.0f, -5.0f, 10.0f);
	// 自キャラの初期化
	player_->Initialize(model_, textureHandle_, playerPosition);

	AddEnemy({0.0f, 5.0f, 30.0f});
	
	LoadEnemyPopData();

	// 3Dモデルの作成
	modelSkydome_ = Model::CreateFromOBJ("skydome", true);
	// 天球の作成
	skydome_ = new Skydome();
	// 天球の初期化
	skydome_->Initialize(modelSkydome_);

	// レールカメラの作成
	railCamera_ = new RailCamera();
	// レールカメラの初期化
	railCamera_->Initialize({0, 0, 0}, {0, 0, 0});
	player_->SetParent(&railCamera_->GetWorldTransform());

	TextureManager::Load("target.png");

	debugCamera_ = new DebugCamera(WinApp::kWindowWidth, WinApp::kWindowHeight);

	// 軸方向表示の表示を有効にする
	AxisIndicator::GetInstance()->SetVisible(true);
	// 軸方向表示が参照するビュープロジェクションを指定する(アドレス渡し)
	AxisIndicator::GetInstance()->SetTargetViewProjection(&viewProjection_);

}

void GameScene::Update() {

	player_->Update(viewProjection_);

	debugCamera_->Update();

	skydome_->Update();

	railCamera_->Update();	

	
	  // デスフラグの立った弾を削除
	enemyBullets_.remove_if([](EnemyBullet* bullet) {
		if (bullet->IsDead()) {
			delete bullet;
			return true;
		}
		return false;
	});
	for (EnemyBullet* bullet : enemyBullets_) {
		bullet->Update();
	}

	enemy_.remove_if([](Enemy* enemy)
	{
		if (enemy->GetIsDead()) 
		{
			delete enemy;
			return true;
		}
		return false;
	});
	for (Enemy* enemy : enemy_) 
	{
		enemy->Update();		
	}

	UpdateEnemyPopCommands();
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
	} else if(!isDebugCameraActive_){
		viewProjection_.matView = railCamera_->GetViewProjection().matView;
		viewProjection_.matProjection = railCamera_->GetViewProjection().matProjection;
		viewProjection_.TransferMatrix();

	}

	
	
	CheckAllColisions();
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

	for (Enemy* enemy : enemy_) {
		// ポインタがnullでない(有効である)時だけ行う
		enemy->Draw(viewProjection_);
	}

	// 弾の描画
	for (EnemyBullet* bullet : enemyBullets_) {
		bullet->Draw(viewProjection_);
	}

	skydome_->Draw(viewProjection_);
	
	// 3Dオブジェクト描画後処理
	Model::PostDraw();
#pragma endregion

#pragma region 前景スプライト描画
	// 前景スプライト描画前処理
	Sprite::PreDraw(commandList);


	player_->DrawUI();
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
	const std::list<EnemyBullet*>& enemyBullets = enemyBullets_;

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
	for (Enemy* enemy : enemy_) {
		// 敵キャラのワールド座標
		posA = enemy->GetWorldEnemyPosition();

		// 敵キャラと自弾全ての当たり判定
		for (PlayerBullet* bullets : playerBullets) {
			// 自弾の座標
			posB = bullets->GetWorldPlayerBulletPosition();

			float distance = (posB.x - posA.x) * (posB.x - posA.x) +
			                 (posB.y - posA.y) * (posB.y - posA.y) +
			                 (posB.z - posA.z) * (posB.z - posA.z);

			float length = (enemy->enemyRadius + bullets->playerBulletRadius) *
			               (enemy->enemyRadius + bullets->playerBulletRadius);
			// 球と球の交差判定
			if (distance <= length) {
				// 敵キャラの衝突時コールバックを呼び出す
				enemy->EnemyOnColision();
				// 自弾の衝突時コールバックを呼び出す
				bullets->PlayerBulletOnColision();
			}
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

void GameScene::AddEnemyBullet(EnemyBullet* enemyBullet) 
{
	// 弾を登録する
	enemyBullets_.push_back(enemyBullet);
}

void GameScene::AddEnemy(Vector3 pos) 
{
	// 敵の作成
	Enemy* obj = new Enemy();
	// 敵の初期化
	obj->Initialize(model_, pos);
	// 敵キャラに自キャラのアドレスを渡す
	obj->SetPlayer(player_);
	// 敵キャラにゲームシーンを渡す
	obj->SetGameScene(this);

	enemy_.push_back(obj);
}

void GameScene::LoadEnemyPopData() 
{
	// ファイルを開く
	std::ifstream file;
	file.open("./Resources/enemyPop.csv");
	assert(file.is_open());

	// ファイルの内容を文字列ストリームにコピー
	enemyPopCommands << file.rdbuf();

	// ファイルを閉じる
	file.close();
}

void GameScene::UpdateEnemyPopCommands() 
{ 
	// 待機処理
	if (isWaitTime_)
	{
		waitTimer_--;
		if (waitTimer_ <= 0)
		{
			// 待機完了
			isWaitTime_ = false;
		}
		return;
	}

	// 1行分の文字列を入れる変数;
	std::string line;

	// コマンド実行ループ
	while (getline(enemyPopCommands,line))
	{
		std::istringstream line_stream(line);

		std::string word;
		// ,区切りで行の先頭文字列を取得
		getline(line_stream, word, ',');

		// "//"から始まる行はコメント
		if (word.find("//") == 0)
		{
			// コメント行を飛ばす(今回 コマンドx.y.z の記述を飛ばす)
			continue;
		}

		// POPコマンド
		if (word.find("POP") == 0)
		{
			// x座標
			getline(line_stream, word, ',');
			float x = (float)std::atof(word.c_str());

			// y座標
			getline(line_stream, word, ',');
			float y = (float)std::atof(word.c_str());

			// z座標
			getline(line_stream, word, ',');
			float z = (float)std::atof(word.c_str());

			// 敵を発生させる
			AddEnemy(Vector3(x,y,z));
		}
		// WAITコマンド
		else if (word.find("WAIT") == 0)
		{
			getline(line_stream, word, ',');

			// 待ち時間
			int32_t waitTime = atoi(word.c_str());

			// 待機開始
			isWaitTime_ = true;
			waitTimer_ = waitTime;

			//コマンドループを抜ける
			break;			
		}
	}
}
