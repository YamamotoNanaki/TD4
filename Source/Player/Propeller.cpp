#include "Propeller.h"

void IFE::Propeller::Turn()
{
	transform_->eulerAngleDegrees_.y+= 10;
}