#pragma once

#include <glm/glm.hpp>

namespace Crave
{
	class Camera
	{
	public:
		Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), bool isPerspective = true,
			glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), float yaw = YAW, float pitch = PITCH);

		void MoveForward(float deltaTime) {
			m_Position += m_Front * m_MovementSpeed * deltaTime;
		}
		void MoveBackward(float deltaTime) {
			m_Position -= m_Front * m_MovementSpeed * deltaTime;
		}
		void MoveLeft(float deltaTime) {
			m_Position -= m_Right * m_MovementSpeed * deltaTime;
		}
		void MoveRight(float deltaTime) {
			m_Position += m_Right * m_MovementSpeed * deltaTime;
		}
		void MoveUp(float deltaTime) {
			m_Position += m_Up * m_MovementSpeed * deltaTime;
		}
		void MoveDown(float deltaTime) {
			m_Position -= m_Up * m_MovementSpeed * deltaTime;
		}

		bool GetIsPerspective() const { return m_IsPerspective; }
		void SetIsPerspective(bool isPersepctive) { m_IsPerspective = isPersepctive; }

		const float Speed() const { return m_MovementSpeed; }

		void SetSpeed(const float spd) { m_MovementSpeed = spd; }

		const glm::mat4& GetProjMat()     const { 
			return m_IsPerspective ? m_PerspectiveProjMat : m_OrthographicProjMat; }
		const glm::mat4& GetViewMat()     const { return m_ViewMat; }
		const glm::mat4  GetProjViewMat() const { return GetProjMat() * m_ViewMat; }

		const glm::vec3& Position()		  const { return m_Position; }
		const glm::vec3& Up()			  const { return m_Up; }
		const glm::vec3& Front()		  const { return m_Front; }
		const glm::vec3& Right()		  const { return m_Right; }

		const float Zoom() const { return m_Zoom; }

		void OnUpdate();

		void UpdateProjMat(int width, int height);

		void ProcessMouseMovement(float xoffset, float yoffset, bool constrainPitch = true);
		void ProcessMouseScroll(float yoffset);

		~Camera() {}
	private:
		void UpdateViewMat();
	private:
		glm::ivec2 m_LastViewportDim;

		glm::mat4 m_ViewMat;
		glm::mat4 m_PerspectiveProjMat;
		glm::mat4 m_OrthographicProjMat;

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

		bool  m_IsPerspective;

		static constexpr const float YAW = -90.0f;
		static constexpr const float PITCH = 0.0f;
		static constexpr const float SPEED = 8.0f;
		static constexpr const float SENSITIVITY = 0.1f;
		static constexpr const float ZOOM = 45.f;
		static constexpr const float ZOOM_MAX = 180.0f;
		static constexpr const float PERSP_NEAR_PLANE = 0.1f;
		static constexpr const float PERSP_FAR_PLANE = 500.f;
		static constexpr const float PERSP_FOVY_DEGREES = 45.f;

		static constexpr const float ORTHO_NEAR_PLANE = -1000.f;
		static constexpr const float ORTHO_FAR_PLANE = 1000.f;
	};

}