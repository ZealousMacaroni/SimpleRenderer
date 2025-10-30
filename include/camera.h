/**
 * @file camera.h
 * @brief Contains everything needed to make a configurable camera.
 */

#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

/**
 * @class CameraInstance
 * @brief Contains methods needed to get the view matrix.
 * @todo Add an input registry and make it so it isn't per class.
 * @todo Make it conifgureable
 * @todo Add DeltaTime, i.e a timer
 * @todo Make it all work bruh
 * @todo Make it look nice
 */
class CameraInstance {
public:
	CameraInstance() {}			// Default constructor
								
	/**
	 * @brief Gets the view matrix in column major order.
	 * @return Returns the value pointer to the view matrix.
	 */
	glm::mat4 GetViewMatrix() {
		ViewMatrix = glm::lookAt(Position, Position + Front, Up);
		return ViewMatrix;
		
	}
	
	/**
	 * 
	 */
	void ProcessMouseMovement(float _XPos, float _YPos) {
		float _XOffset = LastX - _XPos;
		float _YOffset = _YPos - LastY;
		LastX = _XPos;
		LastY = _YPos;
		_XOffset *= SENSITIVITY;
		_YOffset *= SENSITIVITY;
		Yaw += _XOffset;
		Pitch += _YOffset;
		if(Pitch > 89.0f)
			Pitch =  89.0f;
		if(Pitch < -89.0f)
			Pitch = -89.0f;
		UpdateCameraVectors();

	}
	 
	/**
	 * 
	 */
	void ProcessKeyboardInput(GLFWwindow* _Window) {
		if (glfwGetKey(_Window, GLFW_KEY_W) == GLFW_PRESS)
			Position += SPEED * Front;
		if (glfwGetKey(_Window, GLFW_KEY_S) == GLFW_PRESS)
			Position -= SPEED * Front;
		if (glfwGetKey(_Window, GLFW_KEY_A) == GLFW_PRESS)
			Position -= glm::normalize(glm::cross(Front, Up)) * SPEED;
		if (glfwGetKey(_Window, GLFW_KEY_D) == GLFW_PRESS)
			Position += glm::normalize(glm::cross(Front, Up)) * SPEED;
		if (glfwGetKey(_Window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
			Position += SPEED * WorldUp;
		if (glfwGetKey(_Window, GLFW_KEY_SPACE) == GLFW_PRESS)
			Position -= SPEED * WorldUp;
		if (glfwGetKey(_Window, GLFW_KEY_J) == GLFW_PRESS)
			SPEED = 0.05f;
		if (glfwGetKey(_Window, GLFW_KEY_H) == GLFW_PRESS)
			SPEED = 20.0f;
	}
	 
private:
	glm::mat4 ViewMatrix = glm::mat4(1.0f);
	
	float SPEED = 0.05f;
	const float SENSITIVITY = 0.1f;
	
	float Yaw = -90.0f;
	float Pitch = 0.0f;
	float LastX = 400.0f;
	float LastY = 400.0f;
	
	glm::vec3 Position = glm::vec3(0.0f, 0.0f, 3.0f);
	glm::vec3 Front = glm::vec3(0.0f, 0.0f, -1.0f);
	glm::vec3 WorldUp = glm::vec3(0.0f, 1.0f, 0.0f);
	
	glm::vec3 Up;
	glm::vec3 Right;
	
	void UpdateCameraVectors() {
		Front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
		Front.y = sin(glm::radians(Pitch));
		Front.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
		Front = glm::normalize(Front);
		Right = glm::normalize(glm::cross(Front, WorldUp));
		Up = glm::normalize(glm::cross(Front, Right));
	}
	
};

void MouseCallback(GLFWwindow* _Window, double _XPos, double _YPos) {
	CameraInstance* Cam = (CameraInstance*)glfwGetWindowUserPointer(_Window);
	Cam->ProcessMouseMovement(_XPos, _YPos);
}
