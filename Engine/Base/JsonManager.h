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
		///// json�Ƀf�[�^�����邩���m�F����֐�
		///// </summary>
		///// <param name="json">�m�F����f�[�^</param>
		///// <param name="name">�m�F����f�[�^��</param>
		///// <returns>��肪����ꍇtrue</returns>
		//bool ErrorCheck(const nlohmann::json& json,std::string name);
		///// <summary>
		///// json�Ƀf�[�^�����邩���m�F����֐�
		///// </summary>
		///// <param name="json">�m�F����f�[�^</param>
		///// <param name="num">�m�F���鐔��</param>
		///// <returns>��肪����ꍇtrue</returns>
		//bool ErrorCheck(const nlohmann::json& json,int32_t num);

		void SetSceneName(const std::string& sceneName);
		static JsonManager* Instance();
		Float2 InputFloat2(const nlohmann::json& json);
		Float3 InputFloat3(const nlohmann::json& json);
		Float4 InputFloat4(const nlohmann::json& json);
		void InputVectorFloat3(nlohmann::json& json, std::vector<Float3>& v);
		/// <summary>
		/// �f�[�^�擾�֐�
		/// �f�[�^�����邩�̊m�F�����܂�
		/// </summary>
		/// <typeparam name="T">��{�^+std::string</typeparam>
		/// <param name="json">json</param>
		/// <param name="key">key</param>
		/// <param name="data">�������������ϐ�</param>
		/// <returns>����:true ���s:false</returns>
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
