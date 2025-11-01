/**
 * @file window.h
 * @brief Contains everything needed for window creation/use.
 */

#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "string"

/**
 * @brief A struct which contains all the data neccessary for window configuration.
 */
struct WindowInstanceParameters {
	WindowInstanceParameters() {}	// Default constructor
									
	/**
	 * @brief A constructor which takes in all of the data for the struct.
	 * @param _Title Title of the window to be displayed.
	 * @param _Width Width of the window in pixels.
	 * @param _Height Height of the window in pixels.
	 * @param _VersionMajor The major version of OpenGL, eg 4 in 4.1.
	 * @param _VersionMinor The minor version of OpenGL, eg 1 in 4.1.
	 */
	WindowInstanceParameters(const char* _Title, int _Width, int _Height, int _VersionMajor, int _VersionMinor) : Title(_Title), Width(_Width), Height(_Height), VersionMajor(_VersionMajor), VersionMinor(_VersionMinor) {}
	
	const char* Title;			// String which contains the window title.
	int Width;					// Int which contains the width of the window in pixels.
	int Height;					// Int which contains the height of the window in pixels.
	int VersionMajor;			// The major version of OpenGL, e.g. 1 in v1.2.
	int VersionMinor;			// The minor version of OpenGL, e.g. 2 in v1.2.
	
};

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
	WindowInstance(WindowInstanceParameters& Parameters) : Data(Parameters) {
		// Prepping window creation
		glfwInit();
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, Data.VersionMajor);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, Data.VersionMinor);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
		glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
		
		// Creating window
		Window = glfwCreateWindow(Data.Width, Data.Height, Data.Title, NULL, NULL);
		
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
	 * @brief Returns the pointer to the window used for glfw.
	 * @return
	 */
	GLFWwindow* GetWindowPointer() {
		return Window;
	}
	
	/**
	 * @brief A function which terminates GLFW.
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
		
		}
		
	}
								
private:
	GLFWwindow* Window;			// The pointer to the window.
	WindowInstanceParameters Data;	// The data for this window instance.
	static int WindowCount;		// The count of windows across all instances of Windows.
	
};
