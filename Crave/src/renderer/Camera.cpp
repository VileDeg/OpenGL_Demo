#include "pch.h"
#include "Camera.h"
#include "core/Window.h"
#include <glm/gtc/matrix_transform.hpp>

namespace Crave
{
    Camera::Camera(glm::vec3 position, bool isPerspective,
        glm::vec3 up, float yaw, float pitch)
        : m_Front(glm::vec3(0.0f, 0.0f, -1.0f)), m_MovementSpeed(SPEED),
        m_MouseSensitivity(SENSITIVITY), m_Zoom(ZOOM),
        m_Position(position), m_WorldUp(up), m_Yaw(yaw), m_Pitch(pitch),
        //m_NearPlane(NEAR_PLANE), m_FarPlane(FAR_PLANE),
        m_ViewMat(glm::lookAt(m_Position, m_Position + m_Front, m_Up)),
        m_IsPerspective(isPerspective)
    {
        UpdateProjMat(Window::Width(), Window::Height());

        m_Front = glm::normalize(glm::vec3(0.f) - position);
        m_Pitch = glm::degrees(glm::asin(m_Front.y)); //Look down to world origin by default

        UpdateViewMat();
    }

    void Camera::OnUpdate()
    {
        UpdateViewMat();
    }



    void Camera::UpdateProjMat(int width, int height)
    {
     
        m_LastViewportDim = { width, height };
        
        m_PerspectiveProjMat = glm::perspective(glm::radians(PERSP_FOVY_DEGREES),
            (float)width / (float)height, PERSP_NEAR_PLANE, PERSP_FAR_PLANE);
        

        glm::vec2 wh = glm::vec2(width * 0.5f, height * 0.5f) / m_Zoom;
            
        m_OrthographicProjMat = glm::ortho(-wh.x, wh.x, -wh.y, wh.y,
            ORTHO_NEAR_PLANE, ORTHO_FAR_PLANE);
    }

    void Camera::UpdateViewMat()
    {
        glm::vec3 front;
        front.x = cos(glm::radians(m_Yaw)) * cos(glm::radians(m_Pitch));
        front.y = sin(glm::radians(m_Pitch));
        front.z = sin(glm::radians(m_Yaw)) * cos(glm::radians(m_Pitch));
        m_Front = glm::normalize(front);

        m_Right = glm::normalize(glm::cross(m_Front, m_WorldUp));
        m_Up = glm::normalize(glm::cross(m_Right, m_Front));

        m_ViewMat = glm::lookAt(m_Position, m_Position + m_Front, m_Up);
    }

    void Camera::ProcessMouseMovement(float xoffset, float yoffset, bool constrainPitch)
    {
        xoffset *= m_MouseSensitivity;
        yoffset *= m_MouseSensitivity;

        m_Yaw += xoffset;
        m_Pitch += yoffset;

        if (constrainPitch)
        {
            if (m_Pitch > 89.0f)
                m_Pitch = 89.0f;
            if (m_Pitch < -89.0f)
                m_Pitch = -89.0f;
        }

        UpdateViewMat();
    }

    void Camera::ProcessMouseScroll(float yoffset)
    {
        if (m_IsPerspective)
        {
            MoveBackward(yoffset);
        }
        else
        {
            float zoomSens = 10.f;
            m_Zoom -= (float)yoffset * zoomSens;
            if (m_Zoom < 1.0f)
                m_Zoom = 1.0f;
            if (m_Zoom > ZOOM_MAX)
                m_Zoom = ZOOM_MAX;
            UpdateProjMat(m_LastViewportDim.x, m_LastViewportDim.y);
        }
        
    }
}