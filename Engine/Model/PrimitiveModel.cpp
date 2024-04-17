#include "PrimitiveModel.h"
#include "ImguiManager.h"
#include "JsonManager.h"
#include <unordered_map>

using namespace std;
using namespace IFE;

void IFE::PrimitiveModel::CreateCube(bool smoothing)
{
	const float size = 1.f;

	Vertex vertices[] = {
		// x   y   z        u    v
		//前
		{{-size, -size, -size},{},{0.0f, 1.0f}},	//左下
		{{-size, +size, -size},{},{0.0f, 0.0f}},	//左上
		{{+size, -size, -size},{},{1.0f, 1.0f}},	//右下
		{{+size, +size, -size},{},{1.0f, 0.0f}},	//右上
		//後
		{{+size, -size, +size},{},{1.0f, 1.0f}},	//右下
		{{+size, +size, +size},{},{1.0f, 0.0f}},	//右上
		{{-size, -size, +size},{},{0.0f, 1.0f}},	//左下
		{{-size, +size, +size},{},{0.0f, 0.0f}},	//左上
		//左
		{{-size, -size, -size},{},{0.0f, 1.0f}},	//左下
		{{-size, -size, +size},{},{0.0f, 0.0f}},	//左上
		{{-size, +size, -size},{},{1.0f, 1.0f}},	//右下
		{{-size, +size, +size},{},{1.0f, 0.0f}},	//右上
		//右
		{{+size, +size, -size},{},{1.0f, 1.0f}},	//右下
		{{+size, +size, +size},{},{1.0f, 0.0f}},	//右上
		{{+size, -size, -size},{},{0.0f, 1.0f}},	//左下
		{{+size, -size, +size},{},{0.0f, 0.0f}},	//左上
		//下
		{{-size, +size, +size},{},{1.0f, 1.0f}},	//右下
		{{+size, +size, +size},{},{1.0f, 0.0f}},	//右上
		{{-size, +size, -size},{},{0.0f, 1.0f}},	//左下
		{{+size, +size, -size},{},{0.0f, 0.0f}},	//左上
		//上
		{{-size, -size, -size},{},{0.0f, 1.0f}},	//左下
		{{+size, -size, -size},{},{0.0f, 0.0f}},	//左上
		{{-size, -size, +size},{},{1.0f, 1.0f}},	//右下
		{{+size, -size, +size},{},{1.0f, 0.0f}},	//右上
	};

	//インデックスデータ
	uint32_t indices[] = {
		//前
		0,1,2,
		2,1,3,
		//後
		4,5,6,
		6,5,7,
		//左
		8,9,10,
		10,9,11,
		//右
		12,13,14,
		14,13,15,
		//下
		16,17,18,
		18,17,19,
		//上
		20,21,22,
		22,21,23
	};

	CreateNormal(vertices, indices, _countof(indices));
	if (smoothing)SmoothNormal(vertices);

	vb_.SetVerticle(vertices, _countof(vertices));
	ib_.SetIndex(indices, _countof(indices));

	vb_.Initialize();
	ib_.Initialize();

	setting_ = AddModelSettings::CreateCube;
}

void IFE::PrimitiveModel::CreatePolygonSquare(bool smoothing)
{
	const float size = 1.f;

	Vertex vertices[] = {
		// x   y   z        u    v
		//前
		{{-size, -size, -size},{},{0.0f, 1.0f}},	//左下
		{{-size, +size, -size},{},{0.0f, 0.0f}},	//左上
		{{+size, -size, -size},{},{1.0f, 1.0f}},	//右下
		{{+size, +size, -size},{},{1.0f, 0.0f}}		//右上
	};

	//インデックスデータ
	uint32_t indices[] = {
		//前
		0,1,2,
		2,1,3
	};

	CreateNormal(vertices, indices, _countof(indices));
	if (smoothing)SmoothNormal(vertices);

	vb_.SetVerticle(vertices, _countof(vertices));
	ib_.SetIndex(indices, _countof(indices));

	vb_.Initialize();
	ib_.Initialize();
	setting_ = AddModelSettings::CreateSquare;
}

void IFE::PrimitiveModel::CreateTriangle(bool smoothing)
{
	Vertex vertices[] = {
		// x   y   z        u    v
		//前
		{{-1, -1, 0},{},{0.0f, 0.0f}},	//左下
		{{0, 1, 0},{},{0.0f, 0.0f}},	//上
		{{+1, -1, 0},{},{0.0f, 0.0f}},	//右下
	};

	//インデックスデータ
	uint32_t indices[] = {
		//前
		0,1,2
	};


	CreateNormal(vertices, indices, _countof(indices));
	if (smoothing)SmoothNormal(vertices);

	vb_.SetVerticle(vertices, _countof(vertices));
	ib_.SetIndex(indices, _countof(indices));

	vb_.Initialize();
	ib_.Initialize();
	setting_ = AddModelSettings::CreateTriangle;
}

void IFE::PrimitiveModel::CreateCircle(bool smoothing)
{
	const float radius = 0.5f;
	const uint32_t DIV = 64;
	const float pi = static_cast<float>(M_PI);

	Vertex vertices[DIV + 1]{};

	for (int32_t i = 0; i < DIV; i++)
	{
		vertices[i] = { {radius * sinf(2 * pi / DIV * i),radius * cosf(2 * pi / DIV * i),0},{},{0,0} };
	}
	vertices[DIV] = { 0,0,0 };

	uint32_t indices[DIV * 3]{};

	for (size_t i = 0; i < DIV; i++)
	{
		indices[i * (size_t)3] = (uint32_t)i;
		indices[i * (size_t)3 + (size_t)1] = (uint32_t)i + 1;
		indices[i * (size_t)3 + (size_t)2] = DIV;
	}
	indices[DIV * 3 - 2] = 0;

	CreateNormal(vertices, indices, _countof(indices));
	if (smoothing)SmoothNormal(vertices);

	vb_.SetVerticle(vertices, _countof(vertices));
	ib_.SetIndex(indices, _countof(indices));

	vb_.Initialize();
	ib_.Initialize();
	setting_ = AddModelSettings::CreateCircle;
}

void IFE::PrimitiveModel::CreateSphere(bool smoothing)
{
	const uint32_t DIV = 64;
	const float radius = 1.0f;
	const float pi = static_cast<float>(M_PI);

	std::vector<Vertex> vertices{};

	vertices.push_back({ { 0, radius, 0 }, {}, {0,0} });

	for (size_t i = 1; i < DIV; i++)
	{
		float y = cosf(pi / DIV * i) * radius;
		float r = sinf(pi / DIV * i) * radius;
		for (int32_t j = 0; j < DIV; j++)
		{
			vertices.push_back({ { sinf(2 * pi / DIV * j) * r,y,cosf(2 * pi / DIV * j) * r},{},{0,0} });
		}
	}
	vertices.push_back({ { 0,-radius,0 },{},{0,0} });

	std::vector<uint32_t> indices{};

	for (uint32_t i = 0; i < DIV; i++)
	{
		for (uint32_t j = 0; j < DIV; j++)
		{
			if (i == 0)
			{
				indices.push_back(0);
				indices.push_back(j + (uint32_t)1);
				if (j == DIV - 1)
				{
					indices.push_back(1);
					break;
				}
				indices.push_back(j + 2);

			}
			else if (i == DIV - 1)
			{
				indices.push_back((i - 1) * DIV + 1 + j);
				indices.push_back(static_cast<uint16_t>(vertices.size()) - 1);
				if (j == DIV - 1)
				{
					indices.push_back((i - 1) * DIV + 1 + j - DIV + 1);
					break;
				}
				indices.push_back((i - 1) * DIV + 1 + j + 1);
			}
			else
			{
				if (j == DIV - 1)
				{
					indices.push_back((i - 1) * DIV + 1 + j);
					indices.push_back((i - 1) * DIV + 1 + j + DIV);
					indices.push_back((i - 1) * DIV + 1 + j + 1);
					indices.push_back((i - 1) * DIV + 1 + j);
					indices.push_back((i - 1) * DIV + 1 + j + 1);
					indices.push_back((i - 1) * DIV + 1 + j - DIV + 1);
					break;
				}
				indices.push_back((i - 1) * DIV + 1 + j);
				indices.push_back((i - 1) * DIV + 1 + j + DIV);
				indices.push_back((i - 1) * DIV + 1 + j + DIV + 1);
				indices.push_back((i - 1) * DIV + 1 + j);
				indices.push_back((i - 1) * DIV + 1 + j + DIV + 1);
				indices.push_back((i - 1) * DIV + 1 + j + 1);
			}
		}
	}

	CreateNormal(vertices, indices);
	if (smoothing)SmoothNormal(vertices);

	vb_.SetVerticle(vertices);
	ib_.SetIndex(indices);

	vb_.Initialize();
	ib_.Initialize();
	setting_ = AddModelSettings::CreateSphere;
}

void IFE::PrimitiveModel::Draw()
{
	ID3D12GraphicsCommandList* commandList = GraphicsAPI::Instance()->GetCmdList();
	//頂点バッファの設定
	commandList->IASetVertexBuffers(0, 1, vb_.GetVBView());
	//インデックスバッファの設定
	commandList->IASetIndexBuffer(ib_.GetIBView());
	//描画コマンド
	commandList->DrawIndexedInstanced((UINT)ib_.GetSize(), 1, 0, 0, 0);
}

void IFE::PrimitiveModel::Update()
{
}

void IFE::PrimitiveModel::SetSmooth(bool smooth)
{
	smooth_ = smooth;
}

void IFE::PrimitiveModel::SmoothNormal(Vertex _vertices[])
{
	std::unordered_map<uint32_t, std::vector<uint32_t>>smoothData;
	auto itr = smoothData.begin();
	for (; itr != smoothData.end(); ++itr)
	{
		vector<uint32_t>& v = itr->second;

		Vector3 normal = {};
		for (uint32_t index : v)
		{
			normal.x += _vertices[index].normal.x;
			normal.y += _vertices[index].normal.y;
			normal.z += _vertices[index].normal.z;
		}
		Vector3 a = normal / (float)v.size();
		normal = Vector3Normalize(a);
		for (uint32_t index : v)
		{
			_vertices[index].normal = { normal.x,normal.y,normal.z };
		}
	}
}

void IFE::PrimitiveModel::CreateNormal(Vertex _vertices[], uint32_t _indices[], size_t indicesSize)
{
	size_t isize = indicesSize;
	for (size_t i = 0; i < isize / 3; i++)
	{
		uint32_t index0 = _indices[i * 3 + 0];
		uint32_t index1 = _indices[i * 3 + 1];
		uint32_t index2 = _indices[i * 3 + 2];

		Vector3 p0 = SetVector3(_vertices[index0].pos);
		Vector3 p1 = SetVector3(_vertices[index1].pos);
		Vector3 p2 = SetVector3(_vertices[index2].pos);

		Vector3 v1 = VectorSubtract(p1, p0);
		Vector3 v2 = VectorSubtract(p2, p0);

		Vector3 normal = Vector3Cross(v1, v2);

		normal = Vector3Normalize(normal);

		_vertices[index0].normal = SetFloat3(normal);
		_vertices[index1].normal = SetFloat3(normal);
		_vertices[index2].normal = SetFloat3(normal);
	}
}

void IFE::PrimitiveModel::SmoothNormal(std::vector<Vertex>& _vertices)
{
	std::unordered_map<uint32_t, std::vector<uint32_t>>smoothData;
	auto itr = smoothData.begin();
	for (; itr != smoothData.end(); ++itr)
	{
		vector<uint32_t>& v = itr->second;

		Vector3 normal = {};
		for (uint32_t index : v)
		{
			normal.x += _vertices[index].normal.x;
			normal.y += _vertices[index].normal.y;
			normal.z += _vertices[index].normal.z;
		}
		Vector3 a = normal / (float)v.size();
		normal = Vector3Normalize(a);
		for (uint32_t index : v)
		{
			_vertices[index].normal = { normal.x,normal.y,normal.z };
		}
	}
}

void IFE::PrimitiveModel::CreateNormal(std::vector<Vertex>& _vertices, const std::vector<uint32_t>& _indices)
{
	for (size_t i = 0; i < _indices.size() / 3; i++)
	{
		uint32_t index0 = _indices[i * 3 + 0];
		uint32_t index1 = _indices[i * 3 + 1];
		uint32_t index2 = _indices[i * 3 + 2];

		Vector3 p0 = SetVector3(_vertices[index0].pos);
		Vector3 p1 = SetVector3(_vertices[index1].pos);
		Vector3 p2 = SetVector3(_vertices[index2].pos);

		Vector3 v1 = VectorSubtract(p1, p0);
		Vector3 v2 = VectorSubtract(p2, p0);

		Vector3 normal = Vector3Cross(v1, v2);

		normal = Vector3Normalize(normal);

		_vertices[index0].normal = SetFloat3(normal);
		_vertices[index1].normal = SetFloat3(normal);
		_vertices[index2].normal = SetFloat3(normal);
	}
}

#ifdef NDEBUG
#else
void IFE::PrimitiveModel::DebugGUI()
{
	ImguiManager* im = ImguiManager::Instance();
	im->ModelGUI(componentName_);
}

void IFE::PrimitiveModel::OutputComponent(nlohmann::json& j)
{
	j["settings"] = (uint32_t)setting_;
	j["smooth"] = smooth_;
}

#endif
