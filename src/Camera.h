#pragma once

#include "math_headers.h"

static const float YAW = -90.0f;
static const float PITCH = 0.0f;
static const float SPEED = 8.0f;
static const float SENSITIVITY = 0.1f;
static const float ZOOM = 45.0f;
static const float NEAR_PLANE = 0.1f;
static const float FAR_PLANE = 100.0f;

class Camera
{
public:
	Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), float yaw = YAW, float pitch = PITCH)
		: m_Front(glm::vec3(0.0f, 0.0f, -1.0f)), m_MovementSpeed(SPEED),
		m_MouseSensitivity(SENSITIVITY), m_Zoom(ZOOM),
		m_Position(position), m_WorldUp(up), m_Yaw(yaw), m_Pitch(pitch),
		m_NearPlane(NEAR_PLANE), m_FarPlane(FAR_PLANE)
	{
		updateCameraVectors();
	}
	Camera(float posX, float posY, float posZ, float upX, float upY, float upZ,
		float yaw, float pitch)
		: m_Front(glm::vec3(0.0f, 0.0f, -1.0f)), m_MovementSpeed(SPEED), 
		m_MouseSensitivity(SENSITIVITY), m_Zoom(ZOOM),
		m_Position(glm::vec3(posX, posY, posZ)), m_WorldUp(glm::vec3(upX, upY, upZ)), 
		m_Yaw(yaw), m_Pitch(pitch),
		m_NearPlane(NEAR_PLANE), m_FarPlane(FAR_PLANE)
	{
		
		updateCameraVectors();
	}
	
	inline void MoveForward() { m_Position += m_Front * Velocity(); }
	inline void MoveBackward() { m_Position -= m_Front * Velocity(); }
	inline void MoveLeft() { m_Position -= m_Right * Velocity(); }
	inline void MoveRight() { m_Position += m_Right * Velocity(); }
	inline const float Speed() const { return m_MovementSpeed; }

	inline void SetSpeed(const float spd) { m_MovementSpeed = spd; }

	inline const glm::mat4 GetViewMat() const
	{
		return glm::lookAt(m_Position, m_Position + m_Front, m_Up);
	}

	inline const glm::vec3& Position() const { return m_Position; }
	inline const glm::vec3& Up() const { return m_Up; }
	inline const glm::vec3& Front() const { return m_Front; }
	inline const glm::vec3& Right() const { return m_Right; }
	inline const float Zoom() const { return m_Zoom; }
	
	void ProcessMouseMovement(float xoffset, float yoffset, bool constrainPitch = true);
	void ProcessMouseScroll(float yoffset);

	const glm::mat4 GetProjMat() const;
	

	~Camera() {}
private:
	void updateCameraVectors();
	const float Velocity() const;


	//std::unique_ptr<GLContext> m_Context;
	//GLContext& m_Context;

	glm::vec3 m_Position;
	glm::vec3 m_Up;
	glm::vec3 m_Front;
	glm::vec3 m_Right;
	glm::vec3 m_WorldUp;

	float m_Yaw;
	float m_Pitch;

	float m_MovementSpeed;
	float m_MouseSensitivity;
	float m_Zoom;
	
	float m_NearPlane;
	float m_FarPlane;
};

