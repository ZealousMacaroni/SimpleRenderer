/**
 * @file window.h
 * @brief Contains everything needed for window creation/use.
 */

#pragma once
#include <GLFW/glfw3.h>

/**
 * @class WindowInstance
 * @brief A class representing a single window.
 */
class WindowInstance {
public:
	WindowInstance() {}			// Default constructor
	
	/**
	 * @brief A constructor which creates a window and initializes the GLFW context.
	 */
	WindowInstance() {
		
	}
								
private:
	GLFWwindow* Window;
}
