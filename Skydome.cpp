#include "Skydome.h"

void Skydome::Initialize(Model* model)
{ 
	// NULLポインタチェック
	assert(model);

	model_ = model;

	worldTransform_.Initialize();
	worldTransform_.translation_ = {0, 0, 0};
	worldTransform_.scale_ = {10, 10, 10};
}

void Skydome::Update()
{
	worldTransform_.UpdateMatrix();
}

void Skydome::Draw(ViewProjection& viewprojection) 
{
	model_->Draw(worldTransform_, viewprojection);
}
