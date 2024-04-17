#include "Particle.h"
#include "View.h"
#include "Ease.h"
#include "GraphicsAPI.h"
#include "Transform.h"
#include "ColorBuffer.h"
#include <cassert>

using namespace IFE;
using namespace std;

VertexBuffer<VertexPos> Particle::vb;

IFE::Particle::Particle()
{
	SetParticlePtr(this);
}

void Particle::Initialize()
{
	AddComponent<ColorBuffer>();
	AddComponent<TransformParticle>();
	ComponentManagerParticle::Initialize();
	transform = GetComponent<TransformParticle>();
	transform->parent = emitter->transform;
}

void IFE::Particle::SetComponent(Component* component)
{
	AddComponentBack<Component>(component);
}
void IFE::Particle::SetComponentFront(Component* component)
{
	AddComponent<Component>(component);
}

void IFE::Particle::StaticInitialize()
{
	VertexPos vp[1];
	vb.SetVerticle(vp, _countof(vp));
	vb.Initialize();
}

void IFE::Particle::Update()
{
	if (!isActive)return;
	ComponentManagerParticle::Update();
}

void IFE::Particle::DrawBefore()
{
	ID3D12GraphicsCommandList* commandList = GraphicsAPI::Instance()->GetCmdList();
	//commandList->SetGraphicsRootSignature(root);
	commandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_POINTLIST);
}

void Particle::Draw()
{
	if (!isActive)return;
	if (!DrawFlag)return;
	ComponentManagerParticle::Draw();
	ID3D12GraphicsCommandList* commandList = GraphicsAPI::Instance()->GetCmdList();
	//頂点バッファの設定
	commandList->IASetVertexBuffers(0, 1, vb.GetVBView());
	//描画コマンド
	commandList->DrawInstanced((UINT)vb.GetSize(), 1, 0, 0);
}

Particle::~Particle()
{
	ComponentManagerParticle::Finalize();
}
