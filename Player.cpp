#include "Player.h"
#include "cassert"


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

	// キャラクターの移動速さ
	const float kCharacterSpeed = 0.2f;

	// 押した方向で移動ベクトルを変更(左右)
	if (input_->PushKey(DIK_LEFT)) {
		move.x -= kCharacterSpeed;
	} else if (input_->PushKey(DIK_RIGHT)) {
		move.x += kCharacterSpeed;
	}

	// 押した方向で移動ベクトル変更(上下)
	if (input_->PushKey(DIK_UP)) {
		move.y -= kCharacterSpeed;
	} else if (input_->PushKey(DIK_DOWN)) {
		move.y += kCharacterSpeed;
	}

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

	// 全て掛け合わせたアフィン変換行列をワールド行列に代入する // 行列更新
	worldTransform_.matWorld_ = MakeAffineMatrix(
	    worldTransform_.scale_, worldTransform_.rotation_, worldTransform_.translation_);

	// 行列を定数バッファに転送
	worldTransform_.TransferMatrix();

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
}
