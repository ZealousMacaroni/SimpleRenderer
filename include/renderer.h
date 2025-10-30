/**
 * @file renderer.h
 * @brief Contains the main renderer.
 * @note Todos are general, not specific to this
 * @todo add more robust guards checking on function returns.
 * @todo Sort out the naming of StartFrame and EndFrame on the 
 * @todo Figure out a way to make viewport/perspctive matrix dynamic
 */
 
#pragma once

#include "window.h"
#include "object.h"
#include "camera.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

/**
 * @class RendererInstance
 * @brief A class which contains all code neccessary to render an ObjectInstance.
 */
class RendererInstance {
public:
	RendererInstance() {}		// Default constructor
	
	/**
	 * 
	 */
	RendererInstance(WindowInstance* _Window, CameraInstance* _Camera) : Window(_Window), Camera(_Camera) {
		// Doing starting functions
		glViewport(0, 0, 800, 800);
		glEnable(GL_DEPTH_TEST);
		
		// Setting the window pointer
		Window->SetUserPointer(Camera);
		
		// Setting cursor pos callback
		_Window->SetCursorPositionCallback(MouseCallback);
		
	}
	
	/**
	 * 
	 */
	void StartFrame() {
		// Color stuffs
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		// Camera/view
		View = Camera->GetViewMatrix();
		Camera->ProcessKeyboardInput(Window->GetWindowPointer());
		
	}
	/**
	 * @brief Calls WindowInstance.FinishFrame().
	 * @see See WindowInstance.FinishFrame for more info.
	 * @todo Maybe find a better way to do this?
	 */
	void FinishFrame() {
		Window->FinishFrame();
	}
	
	/**
	 * @brief Renders a single object from a pointer, Object.
	 */
	void RenderObject(ObjectInstance* Object) {
		// Guard checking
		if(Object->CanRender()) {
			// Using the VAO
			Object->UseVAO();
			
			// Getting the shader
			ShaderInstance* Shader = Object->GetShader();
			
			// Using Shader
			Shader->UseProgram();
			
			// Setting uniforms
			Shader->UseModelMatrix       (glm::value_ptr(*Object->GetModelMatrix()));
			Shader->UseViewMatrix        (glm::value_ptr(View));
			Shader->UsePerspectiveMatrix (glm::value_ptr(Perspective));
			
			
			// Actually drawing
			glDrawElements(GL_TRIANGLES, Object->GetIndicesCount(), GL_UNSIGNED_INT, 0);
		
		}
	
	}
	
	
private:
	WindowInstance* Window;		// Window
	CameraInstance* Camera;		// Camera 
	glm::mat4 Perspective = glm::perspective(glm::radians(45.0f), 800.0f / 800.0f, 0.1f, 100.0f);
	glm::mat4 View;
								
};
