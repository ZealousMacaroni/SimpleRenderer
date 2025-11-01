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
	/**
	 * @brief Constructor for the camera.
	 * @param StartPos glm::vec3 representing the starting position of the camera.
	 * @param StartDir glm::vec3 representing the starting direction of the camera.
	 * @param Speed float representing a scalar value which is the speed of the directional movement.
	 * @param Sense float representing a scalar value which is the speed of the camera by mouse movement.
	 */
	CameraInstance(glm::vec3 StartPos, glm::vec3 StartDir, float Speed, float Sense, float _FOV) : Position(StartPos), Front(StartDir), MovementSpeed(Speed), MouseSense(Sense), FOV(_FOV) {
		Up = WorldUp;
	}
							
	/**
	 * @brief Creates and returns the view matrix in column major order.
	 * @return Returns the value pointer to the view matrix. 
	 */
	const float* GetViewMatrix() {
		// Creating the view matrix
		ViewMatrix = glm::lookAt(Position, Position + Front, Up);
		
		// Returning the value of the view matrix
		return glm::value_ptr(ViewMatrix);
		
	}
	
	/**
	 * @brief Updates the camera based off of current mouse position.
	 * @param XPos float representing the Y position of the cursor.
	 * @param XPos float representing the Y position of the cursor.
	 */
	void ProcessMouseMovement(float XPos, float YPos) {
		// Checking if first mouse, correcting
		if(FirstMouse) {
			// Setting positions
			LastX = XPos;
			LastY = YPos;
			
			// Setting to false so it doesnt run again
			FirstMouse = false;
			
		}
		
		// Getting the offsets of the cursor from the last position
		// Note: Order is arbitrary, since it only affects the movement direction, which is also arbitrary
		float XOffset = LastX - XPos;
		float YOffset = YPos - LastY;
		
		// Setting the last x and y
		LastX = XPos;
		LastY = YPos;
		
		// Scaling the offset to sensitivity
		XOffset *= MouseSense;
		YOffset *= MouseSense;
		
		// Adding the offset to the yaw and pitch, again offset is arbitrary. sensitivity is what matters
		Yaw += XOffset;
		Pitch += YOffset;
		
		// Pitch constrains, prevents weird behavior when the camera is
		if(Pitch > 89.0f) {
			Pitch =  89.0f;
			
		}
		
		if(Pitch < -89.0f) {
			Pitch = -89.0f;
			
		}
		
		// Okay, doozy here. I am not going to try and explain it, see learnopengl.com transformations and camera section to understand it. Basically complex ge
		Front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
		Front.y = sin(glm::radians(Pitch));
		Front.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
		
		// After front is calculated, normalize to 1.
		Front = glm::normalize(Front);
		
		// Calculate the right vector, which is perpendicular to both Front and WorldUp
		glm::vec3 Right = glm::normalize(glm::cross(Front, WorldUp));
		
		// Calculate the Up vector(not WorldUp), which is perpendicular to both Front and Right
		Up = glm::normalize(glm::cross(Front, Right));

	}
	 
	/**
	 * @brief Changes variables needed to create view matrix based on input.
	 * @param Window GLFWwindow pointer of the window that the input is gotten from.
	 * @todo Get rid of redundant calculations
	 * @todo Add this to an input system
	 */
	void ProcessKeyboardInput(GLFWwindow* Window) {
		// Moving forward
		if (glfwGetKey(Window, GLFW_KEY_W) == GLFW_PRESS) { 
			Position += MovementSpeed * Front;
		
		}
		
		// Moving backwards
		if (glfwGetKey(Window, GLFW_KEY_S) == GLFW_PRESS) {
			Position -= MovementSpeed * Front;
			
		}
		
		// Moving left
		if (glfwGetKey(Window, GLFW_KEY_A) == GLFW_PRESS) {
			Position -= glm::normalize(glm::cross(Front, Up)) * MovementSpeed;
			
		}
		
		// Moving right
		if (glfwGetKey(Window, GLFW_KEY_D) == GLFW_PRESS) {
			Position += glm::normalize(glm::cross(Front, Up)) * MovementSpeed;
			
		}
		
	}
	
	/**
	 * @brief Function which sets the camera movement speed.
	 * @param Speed The speed of the camera to be set.
	 */
	void SetCameraSpeed(float Speed) {
		MovementSpeed = Speed;
		
	}
	
	/**
	 * @brief Function to set camera sensitivity.
	 * @param Sense The sensitivity of the camera to be set
	 */
	void SetCameraSensitivity(float Sense) {
		MouseSense = Sense;
		
	}
	
	/**
	 * @brief Function to get camera FOV.
	 * @return Returns the camera FOV.
	 */
	float GetFOV() {
		return FOV;
	}
	
	/**
	 * @brief Function to set FOV.
	 * @param _FOV Camera FOV to be set.
	 */
	void SetFOV(float _FOV) {
		FOV = _FOV;
		
	}
	 
private:
	glm::mat4 ViewMatrix = glm::mat4(1.0f);		// The view matrix used for rendering, initialized to identity.
										
	float MovementSpeed;		// The speed at which the camera moves, arbitrary.
	float MouseSense;			// The sensitivity of the camera looking around, arbitrary
								
	float Yaw = 0.0f;			// The starting yaw angle, set to 0
	float Pitch = 0.0f;			// The starting pitch, set to 0
	float LastX = 400.0f;		// The starting position of the cursor, x
	float LastY = 400.0f;		// The starting position of the cursor, y
	
	glm::vec3 Position;			// The position of the camera at the start.
	glm::vec3 Front;			// The starting direction the camera faces
	
	const glm::vec3 WorldUp = glm::vec3(0.0f, 1.0f, 0.0f);		// The constant direction of +y
	
	glm::vec3 Up;				// The vector to be calculated which represents the up direction of the camera.
	
	bool FirstMouse = true;		// Bool so the camera doesnt jump around immediately.
	
	float FOV;					// The FOV of the camera.
	
};

/**
 * @brief Callback for when the mouse position changes.
 * @warning Do not call, will cause weird behavior of the camera.
 * @todo Move this somewhere else. Maybe? idk yet.
 * @param Window GLFW window pointer.
 * @param XPos X Position of the mouse.
 * @param YPos Y Position of the mouse.
 */
void MouseCallback(GLFWwindow* Window, double XPos, double YPos) {
	CameraInstance* Cam = (CameraInstance*)glfwGetWindowUserPointer(Window);
	Cam->ProcessMouseMovement(XPos, YPos);
}
