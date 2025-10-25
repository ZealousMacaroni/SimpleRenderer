/**
 * @file window.h
 * @brief Contains everything needed for window creation/use.
 */

#pragma once
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
	WindowInstance(WindowInstanceParameters Parameters) : Data(Parameters) {
		// Prepping window creation
		glfwInit();
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, Data.VersionMajor);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, Data.VersionMinor);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
		glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
		
		// Creating window
		Window = glfwCreateWindow(Data.Width, Data.Height, Data.Title, NULL, NULL);
		
		// Doing context
		glfwMakeContextCurrent(Window);
		
	}
	
	/**
	 * @brief A function which registers the cursor position callback.
	 * @param The function to use as the callback.
	 * @note The callback function takes in a GLFWwindow*, a double representing the x position of the cursor, and a double representing the y position.
	 */
	void SetCursorPositionCallback(GLFWcursorposfun Callback) {
		glfwSetCursorPosCallback(Window, Callback);
	}
	
	/**
	 * @brief A function which checks whether the window should close.
	 * @returns Returns a boolean of or not the window should close.
	 */
	bool ShouldWindowClose() {
		return glfwWindowShouldClose(Window);
		
	}
	
	/**
	 * @brief A function which finishes off rendering the frame by polling events and swapping buffers.
	 */
	void FinishFrame() {
		glfwPollEvents();
		glfwSwapBuffers(Window);
		
	}
	
	/**
	 * @brief A function which terminates GLFW.
	 */
	void Terminate() {
		glfwTerminate();
		
	}
								
private:
	GLFWwindow* Window;			// The pointer to the window.
	WindowInstanceParameters Data;	// The data for this window instance.
	bool ParametersInit = false;	// Bool guard determining whether or not the parameters have been loaded.

};
