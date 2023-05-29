#include "EnemyBullet.h"
void EnemyBullet::Initialize(Model* model, const Vector3& pos, const Vector3& velocity) {
	assert(model);

	modelEnemyBullet_ = model;
	// テクスチャ読み込み
	textureHandleEnemyBullet_ = TextureManager::Load("white1x1.png");

	worldEnemyBullet_.Initialize();
	// 　引数で受け取った初期座標をセット
	worldEnemyBullet_.translation_ = pos;

	// 引数で受け取った速度をメンバ変数に代入
	velocityEnemyBullet_ = velocity;
}

void EnemyBullet::Update() {

	worldEnemyBullet_.translation_.x += velocityEnemyBullet_.x;
	worldEnemyBullet_.translation_.y += velocityEnemyBullet_.y;
	worldEnemyBullet_.translation_.z += velocityEnemyBullet_.z;

	if (--deathEnemyBulletTimer_ <= 0) {
		isEnemyBulletDead_ = true;
	}

	worldEnemyBullet_.UpdateMatrix();
}

void EnemyBullet::Draw(const ViewProjection& viewProjection) 
{
	modelEnemyBullet_->Draw(worldEnemyBullet_, viewProjection, textureHandleEnemyBullet_);
}

void EnemyBullet::EnemyBulletOnColision() 
{
	isEnemyBulletDead_ = true; 
}

Vector3 EnemyBullet::GetWorldEnemyBulletPosition() {
	Vector3 worldPos;

	worldPos.x = worldEnemyBullet_.translation_.x;
	worldPos.y = worldEnemyBullet_.translation_.y;
	worldPos.z = worldEnemyBullet_.translation_.z;
	return worldPos;
};