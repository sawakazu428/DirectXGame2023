#include "RailCamera.h"

void RailCamera::Initialize(Vector3 trans, Vector3 rot) {
	// ワールドトランスフォームの初期設定
	railWorldTransform_.translation_ = trans;
	railWorldTransform_.rotation_ = rot;
	railWorldTransform_.Initialize();
	// ビュープロジェクションの初期化
	railViewProjection_.Initialize();
}

void RailCamera::Update() 
{
	Vector3 move = {0.0f,0.0f,0.0f};
	Vector3 rot = {0.0f, 0.000f, 0.0f};

	/*railWorldTransform_.translation_.x += move.x;
	railWorldTransform_.translation_.y += move.y;
	railWorldTransform_.translation_.z += move.z;

	railWorldTransform_.rotation_.x += rot.x;
	railWorldTransform_.rotation_.y += rot.y;
	railWorldTransform_.rotation_.z += rot.z;*/

	railWorldTransform_.UpdateMatrix();

	railViewProjection_.matView = Inverse(railWorldTransform_.matWorld_);
	railViewProjection_.TransferMatrix();

	// カメラの座標を画面表示する処理
	ImGui::Begin("Camera");
	ImGui::DragFloat3("railCameraTranslate", &railWorldTransform_.translation_.x, 0.01f);	
	ImGui::DragFloat3("railCameraRotate", &railWorldTransform_.rotation_.x, 0.01f);
	ImGui::End();

}
