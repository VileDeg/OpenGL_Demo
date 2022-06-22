#pragma once

#include "math_headers.h"
#include "GLContext.h"

static const float YAW = -90.0f;
static const float PITCH = 0.0f;
static const float SPEED = 2.5f;
static const float SENSITIVITY = 0.1f;
static const float ZOOM = 45.0f;
//static const float ZOOM_SENS = 0.1f;

class Camera
{
public:
	Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), float yaw = YAW, float pitch = PITCH)
		: m_Front(glm::vec3(0.0f, 0.0f, -1.0f)), m_MovementSpeed(SPEED),
		m_MouseSensitivity(SENSITIVITY), m_Zoom(ZOOM),
		m_Position(position), m_WorldUp(up), m_Yaw(yaw), m_Pitch(pitch)
		//m_Context(std::make_unique<GLContext>(GLContext::getTnstance()))
		
	{
		updateCameraVectors();
	}
	Camera(float posX, float posY, float posZ, float upX, float upY, float upZ,
		float yaw, float pitch)
		: m_Front(glm::vec3(0.0f, 0.0f, -1.0f)), m_MovementSpeed(SPEED), 
		m_MouseSensitivity(SENSITIVITY), m_Zoom(ZOOM),
		m_Position(glm::vec3(posX, posY, posZ)), m_WorldUp(glm::vec3(upX, upY, upZ)), 
		m_Yaw(yaw), m_Pitch(pitch)
		//m_Context(std::make_unique<GLContext>(GLContext::getTnstance()))
		
	{
		
		updateCameraVectors();
	}
	
	inline void MoveForward() { m_Position += m_Front * Velocity(); }
	inline void MoveBackward() { m_Position -= m_Front * Velocity(); }
	inline void MoveLeft() { m_Position -= m_Right * Velocity(); }
	inline void MoveRight() { m_Position += m_Right * Velocity(); }

	glm::mat4 GetViewMatrix()
	{
		return glm::lookAt(m_Position, m_Position + m_Front, m_Up);
	}

	inline const glm::vec3& Position() const { return m_Position; }
	inline const glm::vec3& Up() const { return m_Up; }
	inline const glm::vec3& Front() const { return m_Front; }
	inline const glm::vec3& Right() const { return m_Right; }
	inline const float Zoom() const { return m_Zoom; }
	
	void ProcessMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch = true);
	void ProcessMouseScroll(float yoffset);

	~Camera() {}
private:
	void updateCameraVectors();
	inline const float Velocity() const { return m_MovementSpeed * 
		GLContext::getTnstance().DeltaTime(); }

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

	
};

