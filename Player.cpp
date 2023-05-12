#include "Player.h"
#include "cassert"


Player::~Player() 
{
	for (PlayerBullet* bullet : bullets_)
	{
		delete bullet;
	}
}

void Player::Initialize(Model* model, uint32_t textureHandle) {
	// NULLポイントチェック
	assert(model);

	// 受け渡し
	model_ = model;
	textureHandle_ = textureHandle;
	// ワールド変換の初期化
	worldTransform_.Initialize();
	// シングルインスタンスを取得する
	input_ = Input::GetInstance();
}

void Player::Update() {
	// キャラクターの移動ベクトル
	Vector3 move = {0, 0, 0};

	// キャラクター旋回処理
	// 回転速さ[ラジアン/frame]
	const float kRotSpeed = 0.02f;

	// 旋回、押した方向で移動ベクトルを変更
	if (input_->PushKey(DIK_A)) {
		worldTransform_.rotation_.y -= kRotSpeed;
	} else if (input_->PushKey(DIK_D))
	{
		worldTransform_.rotation_.y += kRotSpeed;
	}


	// キャラクター移動処理
	// キャラクターの移動速さ
	const float kCharacterSpeed = 0.2f;


	// 押した方向で移動ベクトルを変更(左右)
	if (input_->PushKey(DIK_LEFT)) 
	{
		move.x -= kCharacterSpeed;

	} else if (input_->PushKey(DIK_RIGHT))
	{
		move.x += kCharacterSpeed;
	}

	// 押した方向で移動ベクトル変更(上下)
	if (input_->PushKey(DIK_UP)) {
		move.y += kCharacterSpeed;
	} else if (input_->PushKey(DIK_DOWN)) {
		move.y -= kCharacterSpeed;
	}

	
	// キャラクター攻撃処理
	Attack();

	// 弾更新
	
	for (PlayerBullet* bullet : bullets_)
	{
		bullet->Update();
	}

	//if (bullet_) // if(bullet != nullptr)と同じ効果になる
	//{
	//	bullet_->Update();
	//}

	// 範囲制限
	const float kMoveLimitX = 20.0f;
	const float kMoveLimitY = 20.0f;

	// 範囲を超えない処理
	worldTransform_.translation_.x = max(worldTransform_.translation_.x, -kMoveLimitX);
	worldTransform_.translation_.x = min(worldTransform_.translation_.x, +kMoveLimitX);
	worldTransform_.translation_.y = max(worldTransform_.translation_.y, -kMoveLimitY);
	worldTransform_.translation_.y = min(worldTransform_.translation_.y, +kMoveLimitY);

	// 座標移動(ベクトルの加算)
	worldTransform_.translation_.x += move.x; 
	worldTransform_.translation_.y += move.y; 
	worldTransform_.translation_.z += move.z; 

	worldTransform_.UpdateMatrix();






	#ifdef DEBUG
	// キャラクターの座標を画面表示する処理
	ImGui::Begin("Player");
	float sliderValue[3] = {
	    worldTransform_.translation_.x, worldTransform_.translation_.y,
	    worldTransform_.translation_.z};
	ImGui::SliderFloat3("position", sliderValue, -20.0f, 20.0f);
	worldTransform_.translation_ = {sliderValue[0], sliderValue[1], sliderValue[2]};
	ImGui::End();
#endif // DEBUG


	// スケーリング行列の作成？
	worldTransform_.scale_;

	// X.Y.Z軸周りの回転行列の作成
	//worldTransform_.rotation_.y;
	//worldTransform_.rotation_.z;
	//worldTransform_.rotation_.x = MakeRotateMatrixX(theta_);

	//// 回転行列の合成(Z回転＊X回転＊Y回転)
	//worldTransform_.rotation_ = worldTransform_.rotation_.z * worldTransform_.rotation_.y;

	//// 平行移動行列の作成？
	//worldTransform_.translation_ = MakeTranslateMatrix(move);

	// スケーリング行列 ＊回転行列＊平行移動行列;

}

void Player::Draw(ViewProjection& view) 
{
	model_->Draw(worldTransform_, view, textureHandle_);

	// 弾の描画
	for (PlayerBullet* bullet : bullets_)
	{
		bullet->Draw(view);
	}


	//if (bullet_) // if(bullet != nullptr)と同じ効果になる
	//{
	//	bullet_->Draw(view);
	//}
}

void Player::Attack() 
{
	if (input_->TriggerKey(DIK_SPACE))
	{

		// 弾を生成し、初期化
		PlayerBullet* newBullet = new PlayerBullet();
		newBullet->Initialize(model_, worldTransform_.translation_);

		// 弾を登録する
		bullets_.push_back(newBullet);
	}
}
