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

	velocityEnemy_ = {0, 0, 0.0f};


}

void Enemy::Update() 
{

	switch (phase_) {
	case Phase::Approach:
	default:
	
		ApproachUpdate(-0.2f);
		break;
	case Phase::Leave:
	
		LeaveUpdate(-0.2f);
		break;
	}

	worldTransformEnemy_.translation_.x += velocityEnemy_.x;
	worldTransformEnemy_.translation_.y += velocityEnemy_.y;
	worldTransformEnemy_.translation_.z += velocityEnemy_.z;

	worldTransformEnemy_.UpdateMatrix();

}

void Enemy::Draw(const ViewProjection& view) {
	modelEnemy_->Draw(worldTransformEnemy_, view, textureHandleEnemy_);
}


void Enemy::ApproachUpdate(const float kEnemySpeed) {
// 移動(ベクトルを加算)
	worldTransformEnemy_.translation_.z += kEnemySpeed;

	// 規定の位置に到達したら離脱
	if (worldTransformEnemy_.translation_.z < 0.0f) {
		phase_ = Phase::Leave;
	}
}

void Enemy::LeaveUpdate(const float kEnemySpeed) 
{
	// 移動(ベクトル加算)
	worldTransformEnemy_.translation_.x += kEnemySpeed;
	worldTransformEnemy_.translation_.y += kEnemySpeed;
}
