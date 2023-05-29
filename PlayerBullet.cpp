#include "PlayerBullet.h"

void PlayerBullet::Initialize(Model* model, const Vector3& pos,const Vector3& velocity) 
{
	assert(model);

	model_ = model;
	// テクスチャ読み込み
	textureHandle_ = TextureManager::Load("white1x1.png");


	worldPlayerBullet_.Initialize();
	//　引数で受け取った初期座標をセット
	worldPlayerBullet_.translation_ = pos;

	// 引数で受け取った速度をメンバ変数に代入
	velocity_ = velocity;
}

void PlayerBullet::Update() 
{

	worldPlayerBullet_.translation_.x += velocity_.x;
	worldPlayerBullet_.translation_.y += velocity_.y;
	worldPlayerBullet_.translation_.z += velocity_.z;

	if (--deathTimer_ <= 0)
	{
		isPlayerBulletDead_ = true;
	}

	worldPlayerBullet_.UpdateMatrix();
}

void PlayerBullet::Draw(const ViewProjection& viewProjection) 
{
	model_->Draw(worldPlayerBullet_, viewProjection, textureHandle_);

}

void PlayerBullet::PlayerBulletOnColision()
{
	isPlayerBulletDead_ = true;
}

Vector3 PlayerBullet::GetWorldPlayerBulletPosition() {
	Vector3 worldPos;

	worldPos.x = worldPlayerBullet_.translation_.x;
	worldPos.y = worldPlayerBullet_.translation_.y;
	worldPos.z = worldPlayerBullet_.translation_.z;
	return worldPos;
}
