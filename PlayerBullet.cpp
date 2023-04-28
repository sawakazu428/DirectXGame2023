#include "PlayerBullet.h"

void PlayerBullet::Initialize(Model* model, const Vector3& pos) 
{
	assert(model);

	model_ = model;
	// テクスチャ読み込み
	textureHandle_ = TextureManager::Load("white.png");


	world_.Initialize();
	//　引数で受け取った初期座標をセット
	world_.translation_ = pos;
}

void PlayerBullet::Update() 
{
	world_.UpdateMatrix();
}

void PlayerBullet::Draw(const ViewProjection& viewProjection) 
{
	model_->Draw(world_, viewProjection, textureHandle_);

}
