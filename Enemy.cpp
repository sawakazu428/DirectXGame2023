#include "Enemy.h"

void Enemy::Initialize(Model* model, const Vector3& pos) {

	assert(model);
	// 受け渡し
	modelEnemy_ = model;
	textureHandleEnemy_ = TextureManager::Load("uvChecker.png");
	// ワールド変換の初期化
	worldTransformEnemy_.Initialize();
	// 　引数で受け取った初期座標をセット
	worldTransformEnemy_.translation_ = pos;

	velocityEnemy_ = {0, 0, -0.2f};
}

void Enemy::Update() 
{ 
	worldTransformEnemy_.translation_.x += velocityEnemy_.x;
	worldTransformEnemy_.translation_.y += velocityEnemy_.y;
	worldTransformEnemy_.translation_.z += velocityEnemy_.z;

	worldTransformEnemy_.UpdateMatrix();
}

void Enemy::Draw(const ViewProjection& view) {
	modelEnemy_->Draw(worldTransformEnemy_, view, textureHandleEnemy_);
}
