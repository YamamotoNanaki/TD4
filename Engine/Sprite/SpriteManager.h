#pragma once
#include "Sprite.h"
#include "Texture.h"
#include "IFMath.h"
#include <list>
#include <memory>

namespace IFE
{
	class SpriteManager
	{
		std::list<std::unique_ptr<Sprite>>backgroundList_;
		std::list<std::unique_ptr<Sprite>>foregroundList_;

		SpriteManager() {}
		SpriteManager(const SpriteManager&) {}
		SpriteManager& operator=(const SpriteManager&) {}
		~SpriteManager() {}
	public:
		static SpriteManager* Instance();
		static void Finalize();
		void SPRITEInitialize();
		void Initialize();
		void Update();
		void ForeDraw();
		void BackDraw();
		Sprite* AddInitialize(const std::string& spriteName, const std::string& textureName);
		Sprite* AddInitializePushFront(const std::string& spriteName, const std::string& textureName);
		Sprite* Add(const std::string& spriteName);
		Sprite* AddBackGround(const std::string& spriteName);
		Sprite* GetSpritePtr(const std::string& spriteName);
		void SetTexture(const std::string& spriteName, const std::string& texName);
		std::list<std::unique_ptr<Sprite>>* GetBackgroundList();
		std::list<std::unique_ptr<Sprite>>* GetForegroundList();
		void Reset();
		bool SearchName(const std::string& name);

#ifdef NDEBUG
#else
		void DebugGUI();
		void OutputScene();
		void DebugUpdate();
#endif
		void LoadingScene();

	public:
		std::string GetNewName(const std::string& spriteName);
	};
}
