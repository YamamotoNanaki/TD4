#include "EnemyManager.h"

void IFE::EnemyManager::Initialize()
{
	//敵キャラの初期化
	for (const std::unique_ptr<NormalEnemy>& enemys_ : enemyList_) {
		enemys_->Initialize();
	}
}

void IFE::EnemyManager::Update()
{
	//敵キャラの更新
	for (const std::unique_ptr<NormalEnemy>& enemys_ : enemyList_) {
		enemys_->Update();
	}
}

void IFE::EnemyManager::Draw()
{
}

void IFE::EnemyManager::Finalize()
{
}

void IFE::EnemyManager::ComponentDebugGUI()
{
}
