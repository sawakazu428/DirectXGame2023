#include "PlayerBullet.h"

void PlayerBullet::Initialize(Model* model, const Vector3& pos,const Vector3& velocity) 
{
	assert(model);

	model_ = model;
	// テクスチャ読み込み
	textureHandle_ = TextureManager::Load("white1x1.png");


	world_.Initialize();
	//　引数で受け取った初期座標をセット
	world_.translation_ = pos;

	// 引数で受け取った速度をメンバ変数に代入
	velocity_ = velocity;
}

void PlayerBullet::Update() 
{

	world_.translation_.x += velocity_.x;
	world_.translation_.y += velocity_.y;
	world_.translation_.z += velocity_.z;

	if (--deathTimer_ <= 0)
	{
		isDead_ = true;
	}

	world_.UpdateMatrix();
}

void PlayerBullet::Draw(const ViewProjection& viewProjection) 
{
	model_->Draw(world_, viewProjection, textureHandle_);

}
