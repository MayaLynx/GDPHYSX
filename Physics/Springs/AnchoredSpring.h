#pragma once
#include "../ForceGenerator/ForceGenerator.h"

namespace Koyu
{
	class AnchoredSpring : public ForceGenerator
	{
	private:
		glm::vec3 anchorPoint;
		float springConstant;
		float restLength;

	public:
		AnchoredSpring(glm::vec3 pos, float _springConst, float _restLen) :
			anchorPoint(pos), springConstant(_springConst), restLength(_restLen){ }

		void updateForce(PhysicsParticle* particle, float time) override;
	};
}


