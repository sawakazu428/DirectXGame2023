#include "Enemy.h"
#include "Player.h"
#include <cmath>
#include "GameScene.h"

Enemy::~Enemy()
{
	
}

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

	ApproachInitialze();

}

void Enemy::ApproachInitialze() 
{
	// 発射タイマーを初期化
	shotTimer_ = kFireInterval;
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

void Enemy::Draw(const ViewProjection& view)
{
	if (isEnemyDead_ == false)
	{
		modelEnemy_->Draw(worldTransformEnemy_, view, textureHandleEnemy_);
	}	

}


void Enemy::ApproachUpdate(const float kEnemySpeed) {
// 移動(ベクトルを加算)
	worldTransformEnemy_.translation_.z += kEnemySpeed;

	// 規定の位置に到達したら離脱
	if (worldTransformEnemy_.translation_.z < 30.0f) {
		phase_ = Phase::Leave;
	}

	shotTimer_--;
	// 指定時間に達した
	if (shotTimer_ <= 0) {
		// 弾を発射した
		Fire();
		// 発射タイマーを初期化
		shotTimer_ = kFireInterval;
	}
}

void Enemy::LeaveUpdate(const float kEnemySpeed) 
{
	if (leaveTimer_-- <= 0)
	{
		isEnemyLeave_ = true;
	}
	// 移動(ベクトル加算)
	worldTransformEnemy_.translation_.x += kEnemySpeed;
	worldTransformEnemy_.translation_.y += kEnemySpeed;
}

void Enemy::Fire() 
{
	assert(player_);
	
	// 弾の速度
	const float kBulletSpeed = 1.0f;
	//Vector3 velocity(0, 0, kBulletSpeed);
	float length;

	// 自キャラのワールド座標を取得する
	player_->GetWorldPlayerPosition();
	// 敵キャラのワールド座標を取得する
	GetWorldEnemyPosition();
	// 敵キャラ->自キャラの差分ベクトルを求める
	Vector3 vector = {
	    player_->GetWorldPlayerPosition().x - GetWorldEnemyPosition().x,
	    player_->GetWorldPlayerPosition().y - GetWorldEnemyPosition().y,
	    player_->GetWorldPlayerPosition().z - GetWorldEnemyPosition().z};

	// ベクトルの正規化
	length = sqrt(vector.x * vector.x +	vector.y * vector.y + vector.z * vector.z);
	// ベクトルの長さを、早さに合わせる。
	Vector3 dir = {vector.x / length, vector.y / length, vector.z / length};
	Vector3 velocity = {dir.x * kBulletSpeed, dir.y * kBulletSpeed, dir.z * kBulletSpeed};


	// 弾を生成し、初期化
	EnemyBullet* newEnemyBullet = new EnemyBullet();
	newEnemyBullet->Initialize(modelEnemy_, worldTransformEnemy_.translation_, velocity);

	gameScene_->AddEnemyBullet(newEnemyBullet);
}

void Enemy::EnemyOnColision() 
{
	isEnemyDead_ = true;
	Score_ += 100;
}

Vector3 Enemy::GetWorldEnemyPosition() 
{
	Vector3 worldPos;

	worldPos.x = worldTransformEnemy_.translation_.x;
	worldPos.y = worldTransformEnemy_.translation_.y;
	worldPos.z = worldTransformEnemy_.translation_.z;
	return worldPos;
	    
};