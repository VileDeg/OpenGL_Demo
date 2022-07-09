#pragma once
#include "math_headers.h"

namespace Light
{
	struct LightBase
	{
		glm::vec3 ambient;
		glm::vec3 diffuse;
		glm::vec3 specular;

		float constant;
		float linear;
		float quadratic;

		virtual ~LightBase() = 0;
	};

	

	struct PointLight : virtual public LightBase
	{
		glm::vec3 position;
	};

	struct DirectionalLight : virtual public LightBase
	{
		glm::vec3 direction;
	};

	struct SpotLight : public PointLight, public DirectionalLight
	{
		float cutOff;
		float outerCutOff;
	};
}