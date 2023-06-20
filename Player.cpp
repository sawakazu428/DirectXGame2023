﻿#include "Player.h"


Player::~Player() 
{
	for (PlayerBullet* bullet : playerBullets_)
	{
		delete bullet;
	}
}

void Player::Initialize(Model* model, uint32_t textureHandle, Vector3 pos) {
	// NULLポイントチェック
	assert(model);

	// 受け渡し
	model_ = model;
	ReticleModel_ = Model::CreateFromOBJ("Reticle", true);
	textureHandle_ = textureHandle;
	worldTransform_.translation_ = pos;
	// ワールド変換の初期化
	worldTransform_.Initialize();
	// シングルインスタンスを取得する
	input_ = Input::GetInstance();
	worldTransform3Dreticle_.Initialize();
}

void Player::Update() {

	// デスフラグの立った弾を削除
	playerBullets_.remove_if([](PlayerBullet* bullet) 
	{
		if (bullet->IsDead())
		{
			delete bullet;
			return true;
		}
		return false;
	});

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
	
	for (PlayerBullet* bullet : playerBullets_)
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

	// 自機のワールド座標から3Dレティクルのワールド座標を計算
	{
	    // 自機から3Dレティクルへの距離
		const float kDistancePlayerTo3DReticle = 50.0f;
	    // 自機から3Dレティクルのオフセット(Z+向き)
		Vector3 offset = {0.0f, 0.0f, 1.0f};
		// 自機のワールド行列の回転を反映
		offset = TransformNormal(offset, worldTransform_.matWorld_);
		// ベクトルの長さを整える
		offset = Normalize(offset) * kDistancePlayerTo3DReticle;
		// 3Dレティクルの座標を設定
		worldTransform3Dreticle_.translation_ = GetWorldPlayerPosition() + offset;
		worldTransform3Dreticle_.UpdateMatrix();
	}




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
	//worldTransform_.scale_;

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
	ReticleModel_->Draw(worldTransform3Dreticle_, view);
	// 弾の描画
	for (PlayerBullet* bullet : playerBullets_)
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
		// 弾の速度
		const float kBulletSpeed = 1.0f;
		Vector3 velocity(0, 0, kBulletSpeed);

		// 速度ベクトルを自機の向きに合わせて回転させる
		velocity = TransformNormal(velocity, worldTransform_.matWorld_);

		// 弾を生成し、初期化
		PlayerBullet* newBullet = new PlayerBullet();
		newBullet->Initialize(model_, GetWorldPlayerPosition(), velocity);

		// 弾を登録する
		playerBullets_.push_back(newBullet);
		// 自機から照準オブジェクトへのベクトル
		velocity = worldTransform3Dreticle_.translation_ - GetWorldPlayerPosition();
		velocity = Normalize(velocity) * kBulletSpeed;
	}
}

void Player::PlayerOnColision() {}

void Player::SetParent(const WorldTransform* parent) 
{
	worldTransform_.parent_ = parent;
}

Vector3 Player::GetWorldPlayerPosition() {
	Vector3 worldPos;

	worldPos.x = worldTransform_.matWorld_.m[3][0];
	worldPos.y = worldTransform_.matWorld_.m[3][1];
	worldPos.z = worldTransform_.matWorld_.m[3][2];
	return worldPos;
};
