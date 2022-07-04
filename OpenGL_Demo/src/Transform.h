#pragma once
#include "math_headers.h"
#include "glm/gtc/quaternion.hpp"
#include "glm/gtx/quaternion.hpp"
#include "glm/gtx/euler_angles.hpp"
class Transform
{
public:
	Transform(const glm::vec3 pos = glm::vec3(0.0f),
		const float rotAngle = 0.0f,
		const glm::vec3 rotAxis = WORLD_UP,
		const float scale = 1.0f)
		: m_Model(MAT4_IDENTITY), m_Position(0.0f),
		m_Scale(1.0f)
	{
		TranslateTo(pos);
		//Rotate(rotAngle, rotAxis);
		ScaleTo(scale);
	}
	~Transform() {}

	inline void Translate(glm::vec3 dist)
	{
		m_Position += dist;
		m_Model = glm::translate(m_Model, dist);
	}
	inline void TranslateTo(glm::vec3 pos)
	{
		m_Position = pos;

		m_Model[3] = glm::vec4(pos, 1.f);
	}
	inline void Rotate(float angle, glm::vec3 axis)
	{ 

		m_Model = glm::rotate(m_Model, angle, axis);
	}
	inline void Scale(float factor)
	{
		m_Scale *= factor;
		m_Model = glm::scale(m_Model, glm::vec3(factor));
	}
	inline void ScaleTo(float factor)
	{
		m_Model = glm::scale(m_Model, glm::vec3(1/m_Scale));
		m_Model = glm::scale(m_Model, glm::vec3(factor));
		m_Scale = factor;
		
	}

	inline void SetEulerAngles(glm::vec3 angles)
	{
		
		m_Quaternions = glm::quat(angles);
		m_Model = glm::translate(MAT4_IDENTITY, m_Position) *
			glm::eulerAngleYXZ(angles.x, angles.y, angles.z) *
			glm::scale(MAT4_IDENTITY, glm::vec3(m_Scale));
	}

	inline const glm::vec3 GetPosition()
	{
		return m_Model[3];
	}
	inline const glm::quat GetRotation()
	{
		return m_Quaternions;
	}
	inline const float GetScale()
	{
		return m_Scale;
	}
	inline void ResetTransform()
	{ 

		m_Model = MAT4_IDENTITY;
	};
	inline void ReportPosition()
	{
		std::cout << "Position is " << Vec3String(m_Position) << '\n';
	}
	inline void ReportScale()
	{
		std::cout << "Scale is " << std::to_string(m_Scale) << '\n';
	}
	inline glm::vec3 Up()    { return m_Up; }
	inline glm::vec3 Right() { return m_Right; }
	inline glm::vec3 Front() { return m_Front; }

	static constexpr const glm::vec3 WORLD_UP = glm::vec3(0.0f, 1.0f, 0.0f);

protected:
	inline const std::string Vec3String(const glm::vec3& vec)
	{
		return "(" + std::to_string(vec.x) + ", "
			+ std::to_string(vec.y) + ", " + std::to_string(vec.z) + ')';
		
	}

	glm::mat4 m_Model;

	static constexpr const glm::mat4 MAT4_IDENTITY = glm::mat4(1.0f);
	glm::vec3 m_Position;
	float m_Scale;
	glm::quat m_Quaternions;
	glm::vec3 m_EulerAngles;

	glm::vec3 m_Up;
	glm::vec3 m_Right;
	glm::vec3 m_Front;

};

