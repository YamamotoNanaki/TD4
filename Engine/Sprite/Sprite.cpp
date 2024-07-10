#include "Sprite.h"
//#include "SpriteManager.h"
#include <cassert>
#include "TextureManager.h"
#include "Debug.h"
#include "imgui.h"
#include "GraphicsAPI.h"
#include "Transform.h"
#include "ColorBuffer.h"
#include "GraphicsPipelineManager.h"

using namespace IFE;
using namespace Microsoft::WRL;
using namespace std;

GraphicsPipeline* Sprite::gp_ = nullptr;

IFE::Sprite::Sprite()
{
	SetSpritePtr(this);
}

void IFE::Sprite::StaticInitialize()
{
	gp_ = GraphicsPipelineManager::Instance()->CreateBasic2DGraphicsPipeLine();
}

void IFE::Sprite::BufferInitialize()
{
	Vertex2D vertices[4];

	enum { LB, LT, RB, RT };

	float left = (0.0f - anchorpoint_.x) * size_.x;
	float right = (1.0f - anchorpoint_.x) * size_.x;
	float top = (0.0f - anchorpoint_.y) * size_.y;
	float bottom = (1.0f - anchorpoint_.y) * size_.y;

	if (flipX_)
	{
		left = -left;
		right = -right;
	}

	if (flipY_)
	{
		top = -top;
		bottom = -bottom;
	}

	vertices[LB].pos = { left,	bottom,	0.0f };
	vertices[LT].pos = { left,	top,	0.0f };
	vertices[RB].pos = { right,	bottom,	0.0f };
	vertices[RT].pos = { right,	top,	0.0f };
	float tex_left = 0;
	float tex_right = 1;
	float tex_top = 0;
	float tex_bottom = 1;
	vertices[LB].uv = { tex_left,	tex_bottom };
	vertices[LT].uv = { tex_left,	tex_top };
	vertices[RB].uv = { tex_right,	tex_bottom };
	vertices[RT].uv = { tex_right,	tex_top };

	vb_.SetVerticle(vertices, _countof(vertices));
	vb_.Initialize();
}

void IFE::Sprite::SPRITEInitialize(const std::string& texName, const Float2& s, const Float2& a, bool x, bool y)
{
	this->tex_ = TextureManager::Instance()->GetTexture(texName);

	this->anchorpoint_ = a;
	this->size_ = s;
	this->flipX_ = x;
	this->flipY_ = y;

	BufferInitialize();
#ifdef InverseEditorMode
	AddComponent<ColorBuffer>();
	AddComponent<Transform2D>();
#else
	DebugAddComponent<ColorBuffer>();
	DebugAddComponent<Transform2D>();
#endif
	transform_ = GetComponent<Transform2D>();
	ComponentManager::Initialize();
}

void IFE::Sprite::Initialize(const std::string& texName, const Float2& s, const Float2& a, bool x, bool y)
{
	if (tex_ == nullptr)this->tex_ = TextureManager::Instance()->GetTexture(texName);

	this->anchorpoint_ = a;
	this->size_ = s;
	this->flipX_ = x;
	this->flipY_ = y;

	BufferInitialize();
	transform_ = GetComponent<Transform2D>();
	ComponentManager::Initialize();
}

void IFE::Sprite::DrawBefore()
{
	ID3D12GraphicsCommandList* commandList = GraphicsAPI::Instance()->GetCmdList();
	commandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
	gp_->SetDrawBlendMode();
}

void IFE::Sprite::Update()
{
	if (!isActive_)return;
	ComponentManager::Update();
}

void IFE::Sprite::Draw()
{
	if (!drawFlag_)return;
	if (!isActive_)return;

	ComponentManager::Draw();
	ID3D12GraphicsCommandList* commandList = GraphicsAPI::Instance()->GetCmdList();
	//頂点バッファの設定
	commandList->IASetVertexBuffers(0, 1, vb_.GetVBView());
	//描画コマンド
	tex_->SetTexture(2);
	commandList->DrawInstanced((UINT)vb_.GetSize(), 1, 0, 0);
}

void Sprite::SetSize(const Float2& s)
{
	this->size_ = s;
	TransferVertex();
}

void Sprite::SetTextureRect(const Float2& b, const Float2& s)
{
	this->texBase_ = b;
	this->texSize_ = s;

	// 頂点バッファへのデータ転送
	TransferVertex();
}

void IFE::Sprite::SetAnimation(uint16_t sheetsNumX, uint16_t sheetsNumY, uint16_t& nowNum, bool flipX, bool flipY)
{
	int isFlipX, isFlipY;
	if (flipX == false)isFlipX = 1;
	else isFlipX = -1;
	if (flipY == false)isFlipY = 1;
	else isFlipY = -1;

	float left = ((0.0f - anchorpoint_.x) * tex_->texbuff_->GetDesc().Width / sheetsNumX) * isFlipX;
	float right = ((1.0f - anchorpoint_.x) * tex_->texbuff_->GetDesc().Width / sheetsNumX) * isFlipX;
	float top = ((0.0f - anchorpoint_.y) * tex_->texbuff_->GetDesc().Height / sheetsNumY) * isFlipY;
	float bottom = ((1.0f - anchorpoint_.y) * tex_->texbuff_->GetDesc().Height / sheetsNumY) * isFlipY;

	uint16_t animationXYNum = sheetsNumX * sheetsNumY;//分割数(総合計)
	uint16_t x = nowNum % sheetsNumX;
	uint16_t y = nowNum / sheetsNumX;

	if (nowNum + 1 > animationXYNum)
	{
		nowNum = 0;//0枚目に戻す処理
	}

	Vertex2D vertices[4];

	enum { LB, LT, RB, RT };

	vertices[LT].pos = { left,	top,	0.0f };
	vertices[LT].uv = { static_cast<float>(x) / static_cast<float>(sheetsNumX),static_cast<float>(y) / static_cast<float>(sheetsNumY) };
	vertices[LB].pos = { left,	bottom,	0.0f };
	vertices[LB].uv = { static_cast<float>(x) / static_cast<float>(sheetsNumX),(static_cast<float>(y) + 1) / static_cast<float>(sheetsNumY) };
	vertices[RT].pos = { right,	top,	0.0f };
	vertices[RT].uv = { (static_cast<float>(x) + 1) / static_cast<float>(sheetsNumX),static_cast<float>(y) / static_cast<float>(sheetsNumY) };
	vertices[RB].pos = { right,	bottom,	0.0f };
	vertices[RB].uv = { (static_cast<float>(x) + 1) / static_cast<float>(sheetsNumX),(static_cast<float>(y) + 1) / static_cast<float>(sheetsNumY) };

	vb_.Transfer(vertices, _countof(vertices));
}

void IFE::Sprite::SetComponent(std::unique_ptr<Component> component)
{
	AddComponentBack<Component>(std::move(component));
}

void IFE::Sprite::SetComponentFront(std::unique_ptr<Component>component)
{
	AddComponent<Component>(std::move(component));
}

void IFE::Sprite::SetTexture(const std::string& texName)
{
	this->tex_ = TextureManager::Instance()->GetTexture(texName);
}

void IFE::Sprite::LoadingScene(nlohmann::json& j)
{
	JsonManager::GetData<uint8_t>(j, "order",order_);
	for (auto& com : j["component"])
	{
		ComponentManager::LoadingScene(j, com);
	}
}

void IFE::Sprite::Destroy()
{
	deleteFlag_ = true;
}

bool IFE::Sprite::GetDeleteFlag() const
{
	return deleteFlag_;
}

#ifdef InverseEditorMode
#else
#include "ImguiManager.h"

void IFE::Sprite::DebugGUI(bool fdelete, std::string* str, bool back)
{
	bool m = false;
	if (ImguiManager::Instance()->SpriteGUI(spriteName_, fdelete, back))
	{
		deleteFlag_ = true;
	}
	if (m)
	{
		*str = spriteName_;
	}
}

void IFE::Sprite::ComponentGUI()
{
	std::function<void(std::unique_ptr<Component>)> addFunc = [&](std::unique_ptr<Component> com)
		{
			SetComponent(std::move(com));
		};
	std::function<void(void)>f = [&]()
		{
			ComponentManager::DebugGUI();
		};
	//std::function<void(std::string)>texFunc = [&](std::string name)
	//{
	//	tex = TextureManager::Instance()->GetTexture(name);
	//};
	ImguiManager::Instance()->ComponentGUI2D(spriteName_, f, addFunc/*, texFunc*/);
}

void IFE::Sprite::DebugUpdate()
{
	ComponentManager::DebugUpdate();
}
void IFE::Sprite::OutputScene(nlohmann::json& j)
{
	j["name"] = spriteName_;
	j["texture"] = tex_->texName_;
	j["order"] = order_;
	uint32_t i = 0;
	for (auto& com : componentList_)
	{
		j["component"][i] = com->componentName_;
		i++;
	}
	ComponentManager::OutputScene(j);
}
#endif

void Sprite::TransferVertex()
{
	enum { LB, LT, RB, RT };
	Vertex2D vertices[4];

	float left = (0.0f - anchorpoint_.x) * size_.x;
	float right = (1.0f - anchorpoint_.x) * size_.x;
	float top = (0.0f - anchorpoint_.y) * size_.y;
	float bottom = (1.0f - anchorpoint_.y) * size_.y;
	if (flipX_)
	{
		left = -left;
		right = -right;
	}

	if (flipY_)
	{
		top = -top;
		bottom = -bottom;
	}

	vertices[LB].pos = { left,	bottom,	0.0f };
	vertices[LT].pos = { left,	top,	0.0f };
	vertices[RB].pos = { right,	bottom,	0.0f };
	vertices[RT].pos = { right,	top,	0.0f };
	float tex_left = 0;
	float tex_right = 1;
	float tex_top = 1;
	float tex_bottom = 0;
	vertices[LB].uv = { tex_left,	tex_bottom };
	vertices[LT].uv = { tex_left,	tex_top };
	vertices[RB].uv = { tex_right,	tex_bottom };
	vertices[RT].uv = { tex_right,	tex_top };

	ComPtr<ID3D12Resource> texBuff = tex_->texbuff_;

	if (texBuff)
	{
		D3D12_RESOURCE_DESC resDesc = texBuff->GetDesc();

		tex_left = texBase_.x / resDesc.Width;
		tex_right = (texBase_.x + texSize_.x) / resDesc.Width;
		tex_top = texBase_.y / resDesc.Height;
		tex_bottom = (texBase_.y + texSize_.y) / resDesc.Height;

		vertices[LB].uv = { tex_left,	tex_bottom }; // 左下
		vertices[LT].uv = { tex_left,	tex_top }; // 左上
		vertices[RB].uv = { tex_right,	tex_bottom }; // 右下
		vertices[RT].uv = { tex_right,	tex_top }; // 右上
	}

	vb_.Transfer(vertices, _countof(vertices));
}
