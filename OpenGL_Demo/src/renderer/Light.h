#pragma once
#include "Transform.h"
#include "math_headers.h"

class Light
{
public:
	Light(const glm::vec3& diffuse, const glm::vec3& specular)
		: m_Diffuse(diffuse), m_Specular(specular) {}

	~Light() {}

	const glm::vec3& Specular() const { return m_Specular; }
	const glm::vec3& Diffuse()  const { return m_Diffuse;  }
private:
	glm::vec3 m_Diffuse;
	glm::vec3 m_Specular;
};

class PointLight : virtual public Light
{
public:
	PointLight(const glm::vec3& position, 
		const glm::vec3& diffuse, const glm::vec3& specular)
		: Light(diffuse, specular), m_Position(position) {}
	~PointLight() {}

	void Translate(const glm::vec3& by) { m_Position += by; }
	void TranslateTo(const glm::vec3& pos) { m_Position += pos; }

	const glm::vec3& Position() const { return m_Position; }
private:
	glm::vec3 m_Position;
	
};

class DirectionalLight : virtual public Light
{
public:
	DirectionalLight(const glm::vec3& direction,
		const glm::vec3& diffuse, const glm::vec3& specular)
		: Light(diffuse, specular), m_Direction(direction) {}
	~DirectionalLight() {}

	//void Rotate(const float angle, const glm::vec3& axis) { glm::rotate(10.f, glm::vec3(0.f, 0.f, 1.f)); }
	//void RotateTo(const glm::vec3& rot) { m_Position += pos; }
	void SetDirection(const glm::vec3& dir) { m_Direction = dir; }
	const glm::vec3& Direction() const { return m_Direction; }
private:
	glm::vec3 m_Direction;

};

class SpotLight : public PointLight, public DirectionalLight
{
public:
	struct SpotLightParams
	{
		float constant;
		float linear;
		float quadratic;

		float cutOff;
		float outerCutOff;
	};
public:
	SpotLight(const glm::vec3& position, const glm::vec3& direction,
		const glm::vec3& diffuse, const glm::vec3& specular)
		: PointLight(position, diffuse, specular), 
		DirectionalLight(direction, diffuse, specular), 
		Light(diffuse, specular) {}
	~SpotLight() {}

	void SetParams(const SpotLightParams params) { m_Params = params; }
	const SpotLightParams& Params() const { return m_Params; }
private:
	SpotLightParams m_Params;
};