#include "pch.h"
#include "Camera.h"
#include "GLContext.h"


const float Camera::Velocity() const {
    return m_MovementSpeed *
        GLContext::getTnstance().DeltaTime();
}

const glm::mat4 Camera::GetProjMat() const
{
    return glm::perspective(glm::radians(m_Zoom),
        (float)(GLContext::getTnstance().Width()) / GLContext::getTnstance().Height(),
        m_NearPlane, m_FarPlane);
}

void Camera::updateCameraVectors()
{
	// calculate the new Front vector
	glm::vec3 front;
	front.x = cos(glm::radians(m_Yaw)) * cos(glm::radians(m_Pitch));
	front.y = sin(glm::radians(m_Pitch));
	front.z = sin(glm::radians(m_Yaw)) * cos(glm::radians(m_Pitch));
	m_Front = glm::normalize(front);
	// also re-calculate the Right and Up vector
	m_Right = glm::normalize(glm::cross(m_Front, m_WorldUp));
	m_Up = glm::normalize(glm::cross(m_Right, m_Front));
    /*std::cout << "Camera:\n";
    std::cout << "front(" << m_Front.x << ", " << m_Front.y << ", " << m_Front.z << '\n';
    std::cout << "right(" << m_Right.x << ", " << m_Right.y << ", " << m_Right.z << '\n';
    std::cout << "up(" << m_Up.x << ", " << m_Up.y << ", " << m_Up.z << '\n';*/
}


void Camera::ProcessMouseMovement(float xoffset, float yoffset, bool constrainPitch)
{
    xoffset *= m_MouseSensitivity;
    yoffset *= m_MouseSensitivity;

    m_Yaw   += xoffset;
    m_Pitch += yoffset;

    // make sure that when pitch is out of bounds, screen doesn't get flipped
    if (constrainPitch)
    {
        if (m_Pitch > 89.0f)
            m_Pitch = 89.0f;
        if (m_Pitch < -89.0f)
            m_Pitch = -89.0f;
    }

    // update Front, Right and Up Vectors using the updated Euler angles
    updateCameraVectors();
}
//
// processes input received from a mouse scroll-wheel event. Only requires input on the vertical wheel-axis
void Camera::ProcessMouseScroll(float yoffset)
{
    m_Zoom -= (float)yoffset;
    if (m_Zoom < 1.0f)
        m_Zoom = 1.0f;
    if (m_Zoom > 45.0f)
        m_Zoom = 45.0f;
#ifdef _DEBUG
    std::cout << "Zoom: " << m_Zoom << '\n';
#endif
}