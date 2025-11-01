/**
 * @file window.h
 * @brief Contains everything needed for window creation/use.
 */

#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "string"

/**
 * @class WindowInstance
 * @brief A class representing a single window.
 */
class WindowInstance {
public:
	WindowInstance() {}			// Default constructor
	
	/**
	 * @brief A constructor which creates a window and initializes the GLFW context.
	 * @param Parameters A WindowInstanceParameters which contains all data for the window.
	 */
	WindowInstance(const char* _Title, int _Width, int _Height, int VersionMajor, int VersionMinor) : Title(_Title), Width(_Width), Height(_Height) {
		// Prepping window creation
		glfwInit();
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, VersionMajor);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, VersionMinor);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
		glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
		
		// Creating window
		Window = glfwCreateWindow(Width, Height,Title, NULL, NULL);
		
		// Disabling cursor
		glfwSetInputMode(Window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);  
		
		// Checking for failure
		if(!Window) {
			std::cout << "Error: WindowInstance: Constructor: Window creation failed.\n";
			return;
			
		}
		
		// Incrementing the count of windows
		WindowCount++;
		
		// Doing context
		glfwMakeContextCurrent(Window);
		
		// Initialzing glew
		if(glewInit() != GLEW_OK) {
			std::cout << "Error: WindowInstance: Constructor: GLEW init failed.\n";
			return;
			
		}
		
	}
	
	/**
	 * @brief A function which registers the cursor position callback.
	 * @param Callback The function to use as the callback.
	 * @note The callback function takes in a GLFWwindow*, a double representing the x position of the cursor, and a double representing the y position.
	 */
	void SetCursorPositionCallback(GLFWcursorposfun Callback) {
		// Checking
		if(!Window) {
			return;
		}
		
		// Setting callback
		glfwSetCursorPosCallback(Window, Callback);
		
	}
	
	/**
	 * @brief A function which checks whether the window should close.
	 * @returns Returns a boolean of or not the window should close.
	 */
	bool ShouldWindowClose() {
		// Closing the window if it doesnt exist.
		if(!Window) {
			return true;
		}
		
		// Returning whether the window should close
		return glfwWindowShouldClose(Window);
		
	}
	
	/**
	 * @brief A function which finishes off rendering the frame by polling events and swapping buffers.
	 * @note Effectively the same as RendererInstance.FinishFrame()
	 */
	void FinishFrame() {
		// Returning if the window doesnt exist.
		if(!Window) {
			return;
			
		}
		
		// Doing end frame stuff
		glfwPollEvents();
		glfwSwapBuffers(Window);
		
	}
	
	/**
	 * @brief Function to get the GLFW window pointer.
	 * @return Returns GLFWwindow pointer, which is the window pointer. Like bruh this is self explanatory do we need ts.
	 */
	GLFWwindow* GetWindowPointer() {
		return Window;
		
	}
	
	/**
	 * @brief Function to get the width of the window.
	 * @return Returns an int which is the width of the window.
	 */
	int GetWindowWidth() {
		return Width;
	}
	
	/**
	 * @brief Function to get the height of the window.
	 * @return Returns an int which is the height of the window.
	 */
	int GetWindowHeight() {
		return Height;
	}
	
	/**
	 * @brief A function which terminates destorys the window.
	 * @note If there are multiple WindowInstances, then GLFW will not be terminated until the last window is terminated.
	 */
	~WindowInstance() {
		// Checking if the window exists before doing stuff
		if(Window) {
			// Destroys the window.
			glfwDestroyWindow(Window);
		
			// If it is the last window, also terminate GLFW
			if(WindowCount == 1) {
				glfwTerminate();
				
			}
			
			// Subtracting from WindowCount.
			WindowCount -= 1;
		
		}
		
	}
								
private:
	GLFWwindow* Window;			// The pointer to the window.
	const char* Title;			// Title of the window to be displayed
	int Width;					// Width of the window in pixels
	int Height;					// Height of the window in pixels
	static int WindowCount;		// The count of windows across all instances of Windows.
	
};
