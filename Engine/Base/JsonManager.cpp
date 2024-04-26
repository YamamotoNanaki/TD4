#include "JsonManager.h"
#include <iostream>
#include <fstream>

using namespace IFE;
using namespace std;


//bool IFE::JsonManager::ErrorCheck(const nlohmann::json& json, std::string name)
//{
//	json; name;
//	assert(json.contains(name));
//	return false;
//}
//
//bool IFE::JsonManager::ErrorCheck(const nlohmann::json& json, int32_t num)
//{
//	json; num;
//	assert(json.contains(num));
//	return false;
//}

void IFE::JsonManager::SetSceneName(const std::string& s)
{
	sceneName_ = s;
}

JsonManager* IFE::JsonManager::Instance()
{
	static JsonManager inst;
	return &inst;
}

Float2 IFE::JsonManager::InputFloat2(const nlohmann::json& jsonName)
{
	return Float2(jsonName[0], jsonName[1]);
}

Float3 IFE::JsonManager::InputFloat3(const nlohmann::json& jsonName)
{
	return Float3(jsonName[0], jsonName[1], jsonName[2]);
}

Float4 IFE::JsonManager::InputFloat4(const nlohmann::json& jsonName)
{
	return Float4(jsonName[0], jsonName[1], jsonName[2], jsonName[3]);
}

void IFE::JsonManager::InputVectorFloat3(nlohmann::json& json, std::vector<Float3>& v)
{
	v.resize(json["size"]);
	for (size_t i = 0; i < v.size(); i++)
	{
		InputFloat3(json[i]);
	}
}

void IFE::JsonManager::Input(const std::string& filename)
{
	JsonReset();
	string name = "Data/Scene/" + sceneName_ + "/" + filename + ".json";
	ifstream readingFile;
	readingFile.open(name, std::ios::in);
	if (!readingFile) {
		readingFile.close();
		JsonReset();
		error_ = true;
		return;
	}
	readingFile >> json_;
	readingFile.close();
}

void IFE::JsonManager::JsonReset()
{
	nlohmann::json j;
	json_ = j;
	error_ = false;
}

std::string IFE::JsonManager::SceneInit()
{
	JsonReset();
	string name = "Data/Scene/SceneManager.json";
	ifstream readingFile;
	readingFile.open(name, std::ios::in);
	if (!readingFile) {
		readingFile.close();
		error_ = true;
		return "";
	}
	readingFile >> json_;
	readingFile.close();
#ifdef InverseEditorMode
	return json_["start"];
#else
	return json_["debug"];
#endif
}

bool IFE::JsonManager::IsError() const
{
	return error_;
}

nlohmann::json& IFE::JsonManager::GetJsonData()
{
	return json_;
}

bool IFE::JsonManager::SceneCheck(const std::string& name)
{
	string a = "Data/Scene/" + name + "/" + "ObjectManager" + ".json";
	ifstream readingFile;
	readingFile.open(a, std::ios::in);
	if (!readingFile) {
		readingFile.close();
		return true;
	}
	return false;
}


#ifdef InverseEditorMode
#else
void IFE::JsonManager::OutputFloat2(nlohmann::json& jsonName, const Float2& f)
{
	jsonName[0] = f.x;
	jsonName[1] = f.y;
}

void IFE::JsonManager::OutputFloat3(nlohmann::json& jsonName, const Float3& f)
{
	jsonName[0] = f.x;
	jsonName[1] = f.y;
	jsonName[2] = f.z;
}

void IFE::JsonManager::OutputFloat4(nlohmann::json& jsonName, const Float4& f)
{
	jsonName[0] = f.x;
	jsonName[1] = f.y;
	jsonName[2] = f.z;
	jsonName[3] = f.w;
}

void IFE::JsonManager::OutputFloat2(nlohmann::json& jsonName, const Vector2& v)
{
	jsonName[0] = v.x;
	jsonName[1] = v.y;
}

void IFE::JsonManager::OutputFloat3(nlohmann::json& jsonName, const Vector3& v)
{
	jsonName[0] = v.x;
	jsonName[1] = v.y;
	jsonName[2] = v.z;
}

void IFE::JsonManager::OutputVectorFloat3(nlohmann::json& json, std::vector<Float3> v)
{
	json["size"] = v.size();
	for (size_t i = 0; i < v.size(); i++)
	{
		OutputFloat3(json[i], v[i]);
	}
}

void IFE::JsonManager::Output(const std::string& filename)
{
	string s = json_.dump(4);
	ofstream writingFile;
	string name = "Data/Scene/" + sceneName_ + "/" + filename + ".json";
	writingFile.open(name, std::ios::out);
	writingFile << s << std::endl;
	writingFile.close();
	JsonReset();
}

#include <direct.h>
void IFE::JsonManager::OutputAndMakeDirectry(const std::string& filename, const std::string& directry)
{
	string dir = "Data/Scene/" + sceneName_ + "/" + directry;
	_mkdir(dir.c_str());
	string s = json_.dump(4);
	ofstream writingFile;
	string name = dir + "/" + filename + ".json";
	writingFile.open(name, std::ios::out);
	writingFile << s << std::endl;
	writingFile.close();
	JsonReset();
}

void IFE::JsonManager::MakeDirectry(const std::string& directry)
{
	string dir = "Data/Scene/" + sceneName_ + "/" + directry;
	_mkdir(dir.c_str());
}

void IFE::JsonManager::MakeSceneDirectry()
{
	string dir = "Data/Scene/" + sceneName_;
	_mkdir(dir.c_str());
}

void IFE::JsonManager::SetInitScene()
{
	JsonReset();
	string name = "Data/Scene/SceneManager.json";
	json_["start"] = sceneName_;
	json_["debug"] = sceneName_;
	string s = json_.dump(4);
	ofstream writingFile;
	writingFile.open(name, std::ios::out);
	writingFile << s << std::endl;
	writingFile.close();
}

void IFE::JsonManager::SetDebugScene()
{
	JsonReset();
	string name = "Data/Scene/SceneManager.json";
	ifstream readingFile;
	readingFile.open(name, std::ios::in);
	if (!readingFile) {
		readingFile.close();
		error_ = true;
		return;
	}
	readingFile >> json_;
	readingFile.close();

	string start = json_["start"];

	JsonReset();
	json_["start"] = start;
	json_["debug"] = sceneName_;

	string s = json_.dump(4);
	ofstream writingFile;
	writingFile.open(name, std::ios::out);
	writingFile << s << std::endl;
	writingFile.close();
}
#endif

