#pragma once
#include <string>
#include <unordered_map>
#include "EditorMode.h"

namespace IFE
{
	class Component;
	class ComponentHelp
	{
		using CreateFunc = Component * (*)();
		inline static std::unordered_map<std::string, CreateFunc> creators_;

	private:
		static void Register(const std::string& className, CreateFunc func);

	public:
		static void StaticHelpInitialize();
		static Component* StringToComponent(const std::string& componentName);

#ifdef EditorMode
	private:
		static std::vector<std::string> GetRegisteredClasses();
		static std::vector<std::string> SearchClasses(const std::string& partialName);
	public:
		static std::string GetComponentList();
#endif
	};
}
