#include "Camera.h"

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
}

//void ProcessKeyboard(Camera_Movement direction, float deltaTime)
//{
//    float velocity = MovementSpeed * deltaTime;
//    if (direction == FORWARD)
//        Position += Front * velocity;
//    if (direction == BACKWARD)
//        Position -= Front * velocity;
//    if (direction == LEFT)
//        Position -= Right * velocity;
//    if (direction == RIGHT)
//        Position += Right * velocity;
//}
//
//void processInput(GLFWwindow* window)
//{
//    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
//        glfwSetWindowShouldClose(window, true);
//
//    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
//        camera.ProcessKeyboard(FORWARD, deltaTime);
//    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
//        camera.ProcessKeyboard(BACKWARD, deltaTime);
//    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
//        camera.ProcessKeyboard(LEFT, deltaTime);
//    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
//        camera.ProcessKeyboard(RIGHT, deltaTime);
//}
//
//void ProcessMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch = true)
//{
//    xoffset *= MouseSensitivity;
//    yoffset *= MouseSensitivity;
//
//    Yaw += xoffset;
//    Pitch += yoffset;
//
//    // make sure that when pitch is out of bounds, screen doesn't get flipped
//    if (constrainPitch)
//    {
//        if (Pitch > 89.0f)
//            Pitch = 89.0f;
//        if (Pitch < -89.0f)
//            Pitch = -89.0f;
//    }
//
//    // update Front, Right and Up Vectors using the updated Euler angles
//    updateCameraVectors();
//}
//
//// processes input received from a mouse scroll-wheel event. Only requires input on the vertical wheel-axis
//void ProcessMouseScroll(float yoffset)
//{
//    Zoom -= (float)yoffset;
//    if (Zoom < 1.0f)
//        Zoom = 1.0f;
//    if (Zoom > 45.0f)
//        Zoom = 45.0f;
//}