#include "Enemy.h"

Enemy::~Enemy()
{
	for (EnemyBullet* bullet : enemyBullets_)
	{
		delete bullet;
	}
	
	for (TimedCall* timedCall : timedCalls_)
	{
		delete timedCall;
	}
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
	
	// 発射タイマーをセットする
	FireReset();
}

void Enemy::Update() 
{
    // デスフラグの立った弾を削除
	enemyBullets_.remove_if([](EnemyBullet* bullet) {
		if (bullet->IsDead()) {
			delete bullet;
			return true;
		}
		return false;
	});
	

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

	for (EnemyBullet* bullet : enemyBullets_)
	{
		bullet->Update();
	}

	 // 終了したタイマーを削除
	timedCalls_.remove_if([](TimedCall* timedCall)
	{
		if (timedCall->IsFinished()) 
		{
			delete timedCall;
			return true;
		}
		return false;
	});

	// 範囲forでリストの全要素について回す
	for (TimedCall* timedCall : timedCalls_) 
	{
		timedCall->Update();
	}

}

void Enemy::Draw(const ViewProjection& view)
{

	modelEnemy_->Draw(worldTransformEnemy_, view, textureHandleEnemy_);

	// 弾の描画
	for (EnemyBullet* bullet : enemyBullets_) {
		bullet->Draw(view);
	}
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

void Enemy::Fire() 
{
	
		// 弾の速度
		const float kBulletSpeed = -1.0f;
		Vector3 velocity(0, 0, kBulletSpeed);

		// 弾を生成し、初期化
		EnemyBullet* newEnemyBullet = new EnemyBullet();
		newEnemyBullet->Initialize(modelEnemy_, worldTransformEnemy_.translation_, velocity);

		// 弾を登録する
		enemyBullets_.push_back(newEnemyBullet);
	
}

void Enemy::FireReset() 
{
	// 弾を発射する
	Fire(); 

	// 発射タイマーを初期化
	shotTimer_ = kFireInterval;

	// 発射タイマーをセットする
	timedCalls_.push_back(new TimedCall(std::bind(&Enemy::FireReset, this), kFireInterval));

}


