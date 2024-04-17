#include "Transform.h"
#include "ImguiManager.h"
#include "JsonManager.h"
#include "CameraManager.h"
#include "Object3D.h"
#include "Sprite.h"

using namespace IFE;
using namespace std;

void IFE::Transform::Initialize()
{
	transformBuffer_ = make_unique<ConstBuffer<ConstBufferDataTransform>>();
	constMapTransform_ = transformBuffer_->GetCBMapObject();
	camera_ = CameraManager::sActivCamera_;
	UpdateMatrix();
}

void IFE::Transform::Update()
{
}

void IFE::Transform::Draw()
{
	UpdateMatrix();
	if (parent_)
	{
		matWorld_ *= parent_->matWorld_;//�e�̍s����|���Z����
	}
	camera_ = CameraManager::sActivCamera_;
	constMapTransform_->world = matWorld_;
	if (!camera_)
	{
		camera_ = CameraManager::sActivCamera_;
		if (!camera_)
		{
			transformBuffer_->SetConstBuffView(0);
			return;
		}
	}
	View* v = camera_->GetView();
	Projection* p = camera_->GetProjection();
	constMapTransform_->viewPro = v->Get() * p->Get();
	constMapTransform_->cameraPos = v->eye_;
	transformBuffer_->SetConstBuffView(0);
}

void IFE::Transform::UpdateMatrix()
{
	//////�g�k//////
	//�X�P�[�����O�{�����s��ɐݒ肷��
	matScale_ =
	{ scale_.x,0,0,0,
	0,scale_.y,0,0,
	0,0,scale_.z,0,
	0,0,0,1 };

	//////��]//////

	Float3 eulerRadians = ConvertToRadians(eulerAngleDegrees_);
	rotation_ = EulerToQuaternion(eulerRadians);
	matRot_ = RotateMatrix(rotation_);

	//////���s�ړ�//////
	//�ړ��ʂ��s��ɐݒ肷��
	matTrans_ =
	{ 1,0,0,0,
	0,1,0,0,
	0,0,1,0,
	position_.x,position_.y,position_.z,1 };


	//�P�ʍs�����
	matWorld_ = MatrixIdentity();
	//�X�P�[�����O�s����|����
	matWorld_ *= matScale_;
	//�����p��]�s����|����
	matWorld_ *= matRot_;
	//���s�ړ��s����|����
	matWorld_ *= matTrans_;

	if (parent_ == nullptr && objectPtr_->parent_ != nullptr)
	{
		parent_ = objectPtr_->parent_->GetComponent<Transform>();
	}
	Matrix ls = matScale_ * matRot_;
	lossyScale_ = { ls.m[0][0],ls.m[1][1],ls.m[2][2] };
}

void IFE::Transform::Copy(Component* component)
{
	Transform* t = dynamic_cast<Transform*>(component);
	if (t == nullptr)return;
	eulerFlag_ = t->eulerFlag_;
	eulerAngleDegrees_ = t->eulerAngleDegrees_;
	billbord_ = t->billbord_;
	scale_ = t->scale_;
	position_ = t->position_;
	rotation_ = t->rotation_;
}

Vector3 IFE::Transform::TransformPoint(const Vector3& p)
{
	UpdateMatrix();
	return Matrix::Transform(p, matWorld_);
}

Vector3 IFE::Transform::InverseTransformPoint(const Vector3& p)
{
	UpdateMatrix();
	return Matrix::Transform(p, MatrixInverse(matWorld_));
}

Float3 IFE::Transform::GetLossyScale()
{
	return lossyScale_;
}

Vector3 IFE::Transform::GetForwardVector()
{
	return MultiplyQuaternionAndVector3(rotation_, Vector3(0, 0, 1)).Normalize();
}

Vector3 IFE::Transform::GetUpVector()
{
	return MultiplyQuaternionAndVector3(rotation_, Vector3(0, 1, 0)).Normalize();
}

Vector3 IFE::Transform::GetRightVector()
{
	return MultiplyQuaternionAndVector3(rotation_, Vector3(1, 0, 0)).Normalize();
}

void IFE::Transform::MovePushBack(Vector3 move)
{
	position_ += move;
}

Vector3 Transform::GetWorldPosition()
{
	UpdateMatrix();
	Matrix rMat = matWorld_;
	if (parent_)
	{
		parent_->UpdateMatrix();
		rMat *= parent_->matWorld_;//�e�̍s����|���Z����
	}
	return Matrix::Transform({ 0,0,0 }, rMat);
}

void Transform::SetWorldPosition(const Vector3& worldPos)
{
	if (parent_ == nullptr)
	{
		position_ = worldPos;
	}
	else
	{
		position_ = Matrix::Transform(worldPos, MatrixInverse(parent_->matWorld_));
	}
}

IFE::Transform::~Transform()
{
}

#ifdef NDEBUG
#else
void IFE::Transform::DebugGUI()
{
	ImguiManager* im = ImguiManager::Instance();
	std::function<void(void)> guiFunc = [&]()
	{
		im->DragFloat3GUI(&position_, "position");
		im->DragFloat3GUI(&eulerAngleDegrees_, "rotation", 1.0f);
		if (eulerAngleDegrees_.x >= 360)
		{
			eulerAngleDegrees_.x -= 360;
		}
		if (eulerAngleDegrees_.y >= 360)
		{
			eulerAngleDegrees_.y -= 360;
		}
		if (eulerAngleDegrees_.z >= 360)
		{
			eulerAngleDegrees_.z -= 360;
		}
		if (eulerAngleDegrees_.x <= 0)
		{
			eulerAngleDegrees_.x += 360;
		}
		if (eulerAngleDegrees_.y <= 0)
		{
			eulerAngleDegrees_.y += 360;
		}
		if (eulerAngleDegrees_.z <= 0)
		{
			eulerAngleDegrees_.z += 360;
		}
		rotation_ = EulerToQuaternion(eulerAngleDegrees_);
		im->DragFloat3GUI(&scale_, "scale");
	};
	std::function<void(void)> deleteFunc = [&]()
	{
		componentDeleteFlag_ = true;
	};
	im->ComponentGUI(guiFunc, deleteFunc, componentName_);
	UpdateMatrix();
}
void IFE::Transform::OutputComponent(nlohmann::json& j)
{
	JsonManager* jm = JsonManager::Instance();
	jm->OutputFloat3(j["scale"], scale_);
	jm->OutputFloat3(j["rotation"], eulerAngleDegrees_);
	jm->OutputFloat3(j["position"], position_);
}
#endif
void IFE::Transform::LoadingComponent(nlohmann::json& json)
{
	JsonManager* j = JsonManager::Instance();
	scale_ = j->InputFloat3(json["scale"]);
	eulerAngleDegrees_ = j->InputFloat3(json["rotation"]);
	position_ = j->InputFloat3(json["position"]);
	transformBuffer_ = make_unique<ConstBuffer<ConstBufferDataTransform>>();
	constMapTransform_ = transformBuffer_->GetCBMapObject();
	camera_ = CameraManager::sActivCamera_;
}

#include "WindowsAPI.h"
Matrix Transform2D::matPro_;
void IFE::Transform2D::StaticInitialize()
{
	matPro_ = MatrixOrthoGraphicProjection((float)0, (float)WindowsAPI::Instance()->winWidth_, (float)0, (float)WindowsAPI::Instance()->winHeight_, (float)0, (float)1);
}
void IFE::Transform2D::Initialize()
{
	transformBuffer_ = make_unique<ConstBuffer<ConstBufferMatrix>>();
	constMapTransform_ = transformBuffer_->GetCBMapObject();
	spritePtr_->transform_ = this;
}

void IFE::Transform2D::Update()
{
}

void IFE::Transform2D::Draw()
{
	UpdateMatrix();
	transformBuffer_->SetConstBuffView(1);
}

void IFE::Transform2D::UpdateMatrix()
{
	matWorld_ = MatrixIdentity();
	matWorld_ *= MatrixScaling(scale2D_.x, scale2D_.y, 1);
	float r = ConvertToRadians(rotation2D_);
	matWorld_ *= MatrixRotationZ(r);
	matWorld_ *= MatrixTranslation(position2D_.x, position2D_.y, 0);

	//�萔�o�b�t�@�ւ̃f�[�^�]��
	constMapTransform_->mat = matWorld_ * matPro_;
}

IFE::Transform2D::~Transform2D()
{
}

#ifdef NDEBUG
#else
void IFE::Transform2D::DebugGUI()
{
	ImguiManager* im = ImguiManager::Instance();
	std::function<void(void)> guiFunc = [&]()
	{
		im->DragFloat2GUI(&position2D_, "position");
		im->DragFloatGUI(&rotation2D_, "rotation", 1.0f);
		if (rotation2D_ > 360)
		{
			rotation2D_ -= 360;
		}
		if (rotation2D_ < 0)
		{
			rotation2D_ += 360;
		}
		im->DragFloat2GUI(&scale2D_, "scale");
	};
	std::function<void(void)> deleteFunc = [&]()
	{
		componentDeleteFlag_ = true;
	};
	im->ComponentGUI(guiFunc, deleteFunc, componentName_);
}

void IFE::Transform2D::OutputComponent(nlohmann::json& j)
{
	JsonManager* jm = JsonManager::Instance();
	jm->OutputFloat2(j["scale"], scale2D_);
	j["rotation"] = rotation2D_;
	jm->OutputFloat2(j["position"], position2D_);
}
#endif

void IFE::Transform2D::LoadingComponent(nlohmann::json& json)
{
	JsonManager* j = JsonManager::Instance();
	scale2D_ = j->InputFloat2(json["scale"]);
	rotation2D_ = json["rotation"];
	position2D_ = j->InputFloat2(json["position"]);
	transformBuffer_ = make_unique<ConstBuffer<ConstBufferMatrix>>();
	constMapTransform_ = transformBuffer_->GetCBMapObject();
}

void IFE::TransformParticle::Initialize()
{
	if (particlePtr_)return;
	transformBuffer_ = make_unique<ConstBuffer<ConstBufferBillboard>>();
	constMapTransform_ = transformBuffer_->GetCBMapObject();
	camera_ = CameraManager::sActivCamera_;
}

void IFE::TransformParticle::Update()
{
}

void IFE::TransformParticle::Draw()
{
	if (particlePtr_)return;
	UpdateMatrix();
	camera_ = CameraManager::sActivCamera_;
	View* v = camera_->GetView();
	Projection* p = camera_->GetProjection();
	constMapTransform_->mat = matWorld_ * v->Get() * p->Get();
	constMapTransform_->matBillboard = View::sMatBillBoard_;
	transformBuffer_->SetConstBuffView(1);
}

void IFE::TransformParticle::UpdateMatrix()
{
	//////�g�k//////
	//�X�P�[�����O�{�����s��ɐݒ肷��
	matScale_ =
	{ scale_.x,0,0,0,
	0,scale_.y,0,0,
	0,0,scale_.z,0,
	0,0,0,1 };

	//////��]//////

	Float3 eulerRadians = ConvertToRadians(eulerAngleDegrees_);
	rotation_ = EulerToQuaternion(eulerRadians);
	matRot_ = RotateMatrix(rotation_);

	//////���s�ړ�//////
	//�ړ��ʂ��s��ɐݒ肷��
	matTrans_ =
	{ 1,0,0,0,
	0,1,0,0,
	0,0,1,0,
	position_.x,position_.y,position_.z,1 };


	//�P�ʍs�����
	matWorld_ = MatrixIdentity();
	//�X�P�[�����O�s����|����
	matWorld_ *= matScale_;
	//�����p��]�s����|����
	matWorld_ *= matRot_;
	//���s�ړ��s����|����
	matWorld_ *= matTrans_;

	//if (parent == nullptr && objectPtr->parent != nullptr)
	//{
	//	parent = objectPtr->parent->GetComponent<TransformParticle>();
	//}
	if (parent_ != nullptr)
	{
		parent_->UpdateMatrix();
		matWorld_ *= parent_->matWorld_;//�e�̍s����|���Z����
		matScale_ *= parent_->matScale_;//�e�̃X�P�[�����O�s����|���Z����
		matRot_ *= parent_->matRot_;//�e�̉�]�s����|���Z����
		matTrans_ *= parent_->matTrans_;//�e�̕��s�ړ��s����|���Z����
	}
	Matrix ls = matScale_ * matRot_;
	lossyScale_ = { ls.m[0][0],ls.m[1][1],ls.m[2][2] };
}

Vector3 IFE::TransformParticle::TransformPoint(const Vector3& p)
{
	UpdateMatrix();
	return Matrix::Transform(p, matWorld_);
}

Vector3 IFE::TransformParticle::InverseTransformPoint(const Vector3& p)
{
	UpdateMatrix();
	return Matrix::Transform(p, MatrixInverse(matWorld_));
}

Float3 IFE::TransformParticle::GetLossyScale()
{
	return lossyScale_;
}

ConstBufferBillboard IFE::TransformParticle::GetMatrix()
{
	UpdateMatrix();
	camera_ = CameraManager::sActivCamera_;
	View* v = camera_->GetView();
	Projection* p = camera_->GetProjection();
	ConstBufferBillboard cb;
	cb.mat = matWorld_ * v->Get() * p->Get();
	cb.matBillboard = View::sMatBillBoard_;
	return cb;
}

Vector3 IFE::TransformParticle::GetWorldPosition()
{
	UpdateMatrix();
	return Matrix::Transform({ 0,0,0 }, matWorld_);
}

void IFE::TransformParticle::SetWorldPosition(const Vector3& worldPos)
{
	if (parent_ == nullptr)
	{
		position_ = worldPos;
	}
	else
	{
		position_ = Matrix::Transform(worldPos, MatrixInverse(parent_->matWorld_));
	}
}

IFE::TransformParticle::~TransformParticle()
{
}

#ifdef NDEBUG
#else
void IFE::TransformParticle::DebugGUI()
{
	ImguiManager* im = ImguiManager::Instance();
	std::function<void(void)> guiFunc = [&]()
	{
		im->DragFloat3GUI(&position_, "position");
		im->DragFloat3GUI(&eulerAngleDegrees_, "rotation", 1.0f);
		if (eulerAngleDegrees_.x >= 360)
		{
			eulerAngleDegrees_.x -= 360;
		}
		if (eulerAngleDegrees_.y >= 360)
		{
			eulerAngleDegrees_.y -= 360;
		}
		if (eulerAngleDegrees_.z >= 360)
		{
			eulerAngleDegrees_.z -= 360;
		}
		if (eulerAngleDegrees_.x <= 0)
		{
			eulerAngleDegrees_.x += 360;
		}
		if (eulerAngleDegrees_.y <= 0)
		{
			eulerAngleDegrees_.y += 360;
		}
		if (eulerAngleDegrees_.z <= 0)
		{
			eulerAngleDegrees_.z += 360;
		}
		rotation_ = EulerToQuaternion(eulerAngleDegrees_);
		im->DragFloat3GUI(&scale_, "scale");
	};
	std::function<void(void)> deleteFunc = [&]()
	{
		componentDeleteFlag_ = true;
	};
	im->ComponentGUI(guiFunc, deleteFunc, componentName_);
}

void IFE::TransformParticle::OutputComponent(nlohmann::json& json)
{
	JsonManager* jm = JsonManager::Instance();
	jm->OutputFloat3(json["scale"], scale_);
	jm->OutputFloat3(json["rotation"], eulerAngleDegrees_);
	jm->OutputFloat3(json["position"], position_);
}
#endif

void IFE::TransformParticle::LoadingComponent(nlohmann::json& json)
{
	JsonManager* j = JsonManager::Instance();
	scale_ = j->InputFloat3(json["scale"]);
	eulerAngleDegrees_ = j->InputFloat3(json["rotation"]);
	position_ = j->InputFloat3(json["position"]);
	transformBuffer_ = make_unique<ConstBuffer<ConstBufferBillboard>>();
	constMapTransform_ = transformBuffer_->GetCBMapObject();
	camera_ = CameraManager::sActivCamera_;
}

void IFE::TransformCamera::Update()
{
	UpdateMatrix();
}

void IFE::TransformCamera::UpdateMatrix()
{
	if (eyeTargetUpFlag_)
	{
		auto v = cameraPtr_->GetView();
		v->eye_ = eye_;
		v->target_ = target_;
		v->up_ = up_;
		v->Update();
	}
	else
	{
		//////��]//////
		Float3 eulerRadians = ConvertToRadians(eulerAngleDegrees_);
		rotation_ = EulerToQuaternion(eulerRadians);
		matRot_ = RotateMatrix(rotation_);

		//////���s�ړ�//////
		//�ړ��ʂ��s��ɐݒ肷��
		matTrans_ =
		{ 1,0,0,0,
		0,1,0,0,
		0,0,1,0,
		position_.x,position_.y,position_.z,1 };


		//�P�ʍs�����
		matWorld_ = MatrixIdentity();
		//�����p��]�s����|����
		matWorld_ *= matRot_;
		//���s�ړ��s����|����
		matWorld_ *= matTrans_;

		if (parent_ != nullptr)
		{
			parent_->UpdateMatrix();
			matWorld_ *= parent_->matWorld_;//�e�̍s����|���Z����
			matRot_ *= parent_->matRot_;//�e�̉�]�s����|���Z����
			matTrans_ *= parent_->matTrans_;//�e�̕��s�ړ��s����|���Z����
		}
		cameraPtr_->GetView()->SetMatrixView(matWorld_);
		cameraPtr_->GetView()->eye_ = position_;
	}
	cameraPtr_->GetProjection()->Inisialize(45, projectionSize_.x, projectionSize_.y);
}

void IFE::TransformCamera::Copy(Component* component)
{
	TransformCamera* t = dynamic_cast<TransformCamera*>(component);
	if (t == nullptr)return;
	eulerFlag_ = t->eulerFlag_;
	eulerAngleDegrees_ = t->eulerAngleDegrees_;
	billbord_ = t->billbord_;
	position_ = t->position_;
	rotation_ = t->rotation_;
}

Vector3 IFE::TransformCamera::TransformPoint(const Vector3& p)
{
	UpdateMatrix();
	return Matrix::Transform(p, matWorld_);
}

Vector3 IFE::TransformCamera::InverseTransformPoint(const Vector3& p)
{
	UpdateMatrix();
	return Matrix::Transform(p, MatrixInverse(matWorld_));
}

Vector3 IFE::TransformCamera::GetForwardVector()
{
	return MultiplyQuaternionAndVector3(rotation_, Vector3(0, 0, 1)).Normalize();
}

Vector3 IFE::TransformCamera::GetUpVector()
{
	return MultiplyQuaternionAndVector3(rotation_, Vector3(0, 1, 0)).Normalize();
}

Vector3 IFE::TransformCamera::GetRightVector()
{
	return MultiplyQuaternionAndVector3(rotation_, Vector3(1, 0, 0)).Normalize();
}

void IFE::TransformCamera::RotateAround(const Float3& point, const Float3& axis, float angle)
{
	Quaternion rotation = MakeAxisAngle(axis, angle);
	Vector3 point2 = position_ - point;
	point2 = MultiplyQuaternionAndVector3(rotation, point2);
	position_ = (Vector3)point + point2;
	eulerAngleDegrees_ = QuaternionToEuler(rotation);
}

void IFE::TransformCamera::MovePushBack(Vector3 move)
{
	position_ += move;
}

Vector3 TransformCamera::GetWorldPosition()
{
	UpdateMatrix();
	return Matrix::Transform({ 0,0,0 }, matWorld_);
}

void TransformCamera::SetWorldPosition(const Vector3& worldPos)
{
	if (parent_ == nullptr)
	{
		position_ = worldPos;
	}
	else
	{
		position_ = Matrix::Transform(worldPos, MatrixInverse(parent_->matWorld_));
	}
}

IFE::TransformCamera::~TransformCamera()
{
}

#ifdef NDEBUG
#else
void IFE::TransformCamera::DebugGUI()
{
	ImguiManager* im = ImguiManager::Instance();
	std::function<void(void)> guiFunc = [&]()
	{
		im->DragFloat3GUI(&position_, "position");
		im->DragFloat3GUI(&eulerAngleDegrees_, "rotation", 1.0f);
		if (eulerAngleDegrees_.x >= 360)
		{
			eulerAngleDegrees_.x -= 360;
		}
		if (eulerAngleDegrees_.y >= 360)
		{
			eulerAngleDegrees_.y -= 360;
		}
		if (eulerAngleDegrees_.z >= 360)
		{
			eulerAngleDegrees_.z -= 360;
		}
		if (eulerAngleDegrees_.x <= 0)
		{
			eulerAngleDegrees_.x += 360;
		}
		if (eulerAngleDegrees_.y <= 0)
		{
			eulerAngleDegrees_.y += 360;
		}
		if (eulerAngleDegrees_.z <= 0)
		{
			eulerAngleDegrees_.z += 360;
		}
		rotation_ = EulerToQuaternion(eulerAngleDegrees_);
		im->DragFloat3GUI(&eye_, "eye");
		im->DragFloat3GUI(&target_, "target");
		im->DragFloat2GUI(&projectionSize_, "projectionSize");
	};
	std::function<void(void)> deleteFunc = [&]()
	{
		componentDeleteFlag_ = true;
	};
	im->ComponentGUI(guiFunc, deleteFunc, componentName_);
}
void IFE::TransformCamera::OutputComponent(nlohmann::json& j)
{
	JsonManager* jm = JsonManager::Instance();
	jm->OutputFloat3(j["rotation"], eulerAngleDegrees_);
	jm->OutputFloat3(j["position"], position_);
	jm->OutputFloat2(j["projectionSize"], projectionSize_);
}
#endif
void IFE::TransformCamera::LoadingComponent(nlohmann::json& json)
{
	JsonManager* j = JsonManager::Instance();
	eulerAngleDegrees_ = j->InputFloat3(json["rotation"]);
	position_ = j->InputFloat3(json["position"]);
	projectionSize_ = j->InputFloat2(json["projectionSize"]);
}
