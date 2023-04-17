#include "Player.h"
#include "cassert"


void Player::Initialize(Model* model, uint32_t textureHandle) 
{
	assert(model);

	model_ = model; 
	textureHandle_ = textureHandle;
	worldTransform_.Initialize();
}

void Player::Update() 
{
	// 行列を定数バッファに転送
	worldTransform_.TransferMatrix();
}

void Player::Draw(ViewProjection& view) 
{
	model_->Draw(worldTransform_, view, textureHandle_);
}
