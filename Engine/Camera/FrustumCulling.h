#include "IFEMath.h"
#include "CollisionPrimitive.h"

namespace IFE
{
	class FrustumCulling
	{
		Float4 planes[6];
		Matrix* viewMatrix_;
		Matrix* ProjMatrix_;

	public:
		void Initialize(Matrix* viewMatrix,Matrix* ProjMatrix);
		void Update();
		bool IsSphereInFrustum(Sphere spere);
		//bool IsOBBInFrustum(OBB obb);
	};
}