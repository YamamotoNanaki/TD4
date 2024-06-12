#include "Propeller.h"

void IFE::Propeller::Turn()
{
	transform_->rotation_.y+= 30;
}