#include "pch.h"
#include "Camera.h"
#include "core/Window.h"
#include <glm/gtc/matrix_transform.hpp>

Camera::Camera(glm::vec3 position,
    glm::vec3 up, float yaw, float pitch)
        : m_Front(glm::vec3(0.0f, 0.0f, -1.0f)), m_MovementSpeed(SPEED),
    m_MouseSensitivity(SENSITIVITY), m_Zoom(ZOOM),
    m_Position(position), m_WorldUp(up), m_Yaw(yaw), m_Pitch(pitch),
    m_NearPlane(NEAR_PLANE), m_FarPlane(FAR_PLANE),
    m_ViewMat(glm::lookAt(m_Position, m_Position + m_Front, m_Up)),
    m_ProjMat(glm::perspective(glm::radians(m_Zoom),
        (float)(Window::Width()) / Window::Height(),
        m_NearPlane, m_FarPlane))
{
    m_Front = glm::normalize(glm::vec3(0.f) - position);
    m_Pitch = glm::degrees(glm::asin(m_Front.y)); //Look down to world origin by default
   
    UpdateCameraVectors();
}

void Camera::OnUpdate()
{
    UpdateCameraVectors();
    m_ViewMat = glm::lookAt(m_Position, m_Position + m_Front, m_Up);
    m_ProjViewMat = m_ProjMat * m_ViewMat;
}

//void Camera::SetViewportDimensions(unsigned width, unsigned height)
//{
//    m_ViewportDim.x = width;
//    m_ViewportDim.y = height;
//    UpdateProjMat(width, height);
//}

void Camera::UpdateProjMat(unsigned width, unsigned height)
{
    m_ProjMat = glm::perspective(glm::radians(m_Zoom),
        (float)(width) / height,
        m_NearPlane, m_FarPlane);
    m_ProjViewMat = m_ProjMat * m_ViewMat;
}

void Camera::UpdateCameraVectors()
{
	glm::vec3 front;
	front.x = cos(glm::radians(m_Yaw)) * cos(glm::radians(m_Pitch));
	front.y = sin(glm::radians(m_Pitch));
	front.z = sin(glm::radians(m_Yaw)) * cos(glm::radians(m_Pitch));
	m_Front = glm::normalize(front);

	m_Right = glm::normalize(glm::cross(m_Front, m_WorldUp));
	m_Up = glm::normalize(glm::cross(m_Right, m_Front));
}

void Camera::ProcessMouseMovement(float xoffset, float yoffset, bool constrainPitch)
{
    xoffset *= m_MouseSensitivity;
    yoffset *= m_MouseSensitivity;

    m_Yaw   += xoffset;
    m_Pitch += yoffset;

    if (constrainPitch)
    {
        if (m_Pitch > 89.0f)
            m_Pitch = 89.0f;
        if (m_Pitch < -89.0f)
            m_Pitch = -89.0f;
    }

    UpdateCameraVectors();
}

void Camera::ProcessMouseScroll(float yoffset)
{
    MoveBackward(yoffset);
    /*m_Zoom -= (float)yoffset;
    if (m_Zoom < 1.0f)
        m_Zoom = 1.0f;
    if (m_Zoom > ZOOM_MAX)
        m_Zoom = ZOOM_MAX;
    UpdateProjMat(m_ViewportDim.x, m_ViewportDim.y);*/
}