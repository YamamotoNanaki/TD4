#pragma once
#include <string>
#include "IFEMath.h"
#include "nlohmann/json.hpp"

namespace IFE
{
	class JsonManager
	{
		JsonManager() {}
		JsonManager(const JsonManager&) {}
		JsonManager& operator=(const JsonManager&) {}
		~JsonManager() {}
		nlohmann::json json_;

		std::string sceneName_;
		bool error_ = false;

	public:
		void SetSceneName(const std::string& sceneName);
		static JsonManager* Instance();
		Float2 InputFloat2(const nlohmann::json& json);
		Float3 InputFloat3(const nlohmann::json& json);
		Float4 InputFloat4(const nlohmann::json& json);

		void Input(const std::string& filename);
		void JsonReset();
		std::string SceneInit();
		bool IsError()const;

		nlohmann::json& GetJsonData();
		bool SceneCheck(const std::string& name);

#ifdef NDEBUG
#else
		void OutputFloat2(nlohmann::json& json, const Float2& f);
		void OutputFloat3(nlohmann::json& json, const Float3& f);
		void OutputFloat4(nlohmann::json& json, const Float4& f);
		void OutputFloat2(nlohmann::json& json, const Vector2& v);
		void OutputFloat3(nlohmann::json& json, const Vector3& v);

		void Output(const std::string& filename);
		void OutputAndMakeDirectry(const std::string& filename, const std::string& directry);
		void MakeDirectry(const std::string& directry);
		void MakeSceneDirectry();
		void SetInitScene();
		void SetDebugScene();
#endif
	};
}
