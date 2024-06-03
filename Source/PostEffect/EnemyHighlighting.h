#pragma once
#include "IPostEffect.h"

class EnemyHighlighting : public IFE::IPostEffect
{
public:
	void Initialize()override;
	void Update()override;
	void Draw()override;
};

