#include "EnemyManager.h"

void IFE::EnemyManager::Initialize()
{
	//�G�L�����̏�����
	for (const std::unique_ptr<NormalEnemy>& enemys_ : enemyList_) {
		enemys_->Initialize();
	}
}

void IFE::EnemyManager::Update()
{
	//�G�L�����̍X�V
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
