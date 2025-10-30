/**
 * @file camera.h
 * @brief Contains everything needed to make a configurable camera.
 */

#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

/**
 * @class CameraInstance
 * @brief Contains methods needed to get the view matrix.
 * @todo Add an input registry and make it so it isn't per class.
 * @todo Make it conifgureable
 * @todo Add DeltaTime, i.e a timer
 */
class CameraInstance {
public:
	CameraInstance() {}			// Default constructor
								
	/**
	 * @brief Gets the view matrix in column major order.
	 * @return Returns the value pointer to the view matrix.
	 */
	glm::mat4* GetViewMatrix() {
		return &ViewMatrix;
		
	}
	 
	 
private:
	glm::mat4 ViewMatrix = glm::mat4(1.0f);
	
};
