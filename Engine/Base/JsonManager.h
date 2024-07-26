#pragma once
#include <string>
#include <vector>
#include <type_traits>
#include "IFEMath.h"
#include "nlohmann/json.hpp"
#include "EditorMode.h"
#include "Util.h"

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
		///// <summary>
		///// jsonにデータがあるかを確認する関数
		///// </summary>
		///// <param name="json">確認するデータ</param>
		///// <param name="name">確認するデータ名</param>
		///// <returns>問題がある場合true</returns>
		//bool ErrorCheck(const nlohmann::json& json,std::string name);
		///// <summary>
		///// jsonにデータがあるかを確認する関数
		///// </summary>
		///// <param name="json">確認するデータ</param>
		///// <param name="num">確認する数字</param>
		///// <returns>問題がある場合true</returns>
		//bool ErrorCheck(const nlohmann::json& json,int32_t num);

		void SetSceneName(const std::string& sceneName);
		static JsonManager* Instance();
		Float2 InputFloat2(const nlohmann::json& json);
		Float3 InputFloat3(const nlohmann::json& json);
		Float4 InputFloat4(const nlohmann::json& json);
		void InputVectorFloat3(nlohmann::json& json, std::vector<Float3>& v);
		/// <summary>
		/// データ取得関数
		/// データがあるかの確認もします
		/// </summary>
		/// <typeparam name="T">基本型+std::string</typeparam>
		/// <param name="json">json</param>
		/// <param name="key">key</param>
		/// <param name="data">書き換えたい変数</param>
		/// <returns>成功:true 失敗:false</returns>
		template <class T>
		typename std::enable_if<IS_BASIC_TYPE(T) || std::is_same<T, std::string>::value, bool>::type
			static GetData(const nlohmann::json& json, const std::string& key, T& data);

		void Input(const std::string& filename);
		void JsonReset();
		std::string SceneInit();
		bool IsError()const;

		nlohmann::json& GetJsonData();
		bool SceneCheck(const std::string& name);

#ifdef InverseEditorMode
#else
		void OutputFloat2(nlohmann::json& json, const Float2& f);
		void OutputFloat3(nlohmann::json& json, const Float3& f);
		void OutputFloat4(nlohmann::json& json, const Float4& f);
		void OutputFloat2(nlohmann::json& json, const Vector2& v);
		void OutputFloat3(nlohmann::json& json, const Vector3& v);
		void OutputVectorFloat3(nlohmann::json& json, std::vector<Float3> v);

		void Output(const std::string& filename);
		void OutputAndMakeDirectry(const std::string& filename, const std::string& directry);
		void MakeDirectry(const std::string& directry);
		void MakeSceneDirectry();
		void SetInitScene();
		void SetDebugScene();

#endif
	};
	template<class T>
	inline typename std::enable_if<IS_BASIC_TYPE(T) || std::is_same<T, std::string>::value, bool>::type JsonManager::GetData(const nlohmann::json& json, const std::string& key, T& data)
	{
		if (json.contains(key)) {
			data = T(json[key]);
			return true;
		}
		return false;
	}
}
