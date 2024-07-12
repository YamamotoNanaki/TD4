#pragma once
#include "Component.h"

class Title : public IFE::Component
{
public:
	void Initialize();
	void Update();
	void SetIsNext(bool isNext_) { isNext = isNext_; };
private:
	bool isNext;
};

