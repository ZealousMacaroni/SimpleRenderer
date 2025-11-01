/**
 * @file renderer.h
 * @brief Contains the main renderer.
 * @note Todos are general, not specific to this
 * @todo add more robust guards checking on function returns.
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
 * @todo Add functions to set render distance.
 */
class RendererInstance {
public:
	/**
	 * @brief Initializes the renderer. 
	 * @param _Window Pointer to the window instance to be rendered in.
	 * @param _Camera Pointer to the camera to be used for rendering.
	 * @param _RenderRangeMin The minimum distance from the camera position objects have to be for rendering.
	 * @param _RenderRangeMax The maximum distance from the camera position objects can be to be rendered.
	 */
	RendererInstance(WindowInstance* _Window, CameraInstance* _Camera, float _RenderRangeMin, float _RenderRangeMax) : 
		Window(_Window), 
		Camera(_Camera),
		RenderRangeMin(_RenderRangeMin),
		RenderRangeMax(_RenderRangeMax) 
	{
		// Doing starting functions
		glViewport(0, 0, Window->GetWindowWidth(), Window->GetWindowHeight());
		glEnable(GL_DEPTH_TEST);
		
		// Setting the window pointer
		glfwSetWindowUserPointer(Window->GetWindowPointer(), Camera);
		
		// Setting cursor pos callback
		Window->SetCursorPositionCallback(MouseCallback);
		
		// Setting the perspective matrix
		Perspective = glm::perspective(glm::radians(Camera->GetFOV()), (float)Window->GetWindowWidth() / (float)Window->GetWindowHeight(), RenderRangeMin, RenderRangeMax);
		
	}
	
	/**
	 * @brief Function which initializes the renderer to begin drawing the frame.
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
	 * @brief Renders a single object from a pointer.
	 * @param Object ObjectInstance pointer to be renderered.
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
			Shader->UseViewMatrix        (View);
			Shader->UsePerspectiveMatrix (glm::value_ptr(Perspective));
			
			
			// Actually drawing
			glDrawElements(GL_TRIANGLES, Object->GetIndicesCount(), GL_UNSIGNED_INT, 0);
		
		}
	
	}
	
	
private:
	WindowInstance* Window;		// Window
	CameraInstance* Camera;		// Camera 
	
	glm::mat4 Perspective;		// The perspective matrix
	const float* View;			// The view matrix value pointer
	
	float FOV;					// The FOV of the camera.
	float RenderRangeMin;		// The minimum range of objects from the camera to be rendered.
	float RenderRangeMax;		// The maximum range of objects from the camera to ve rendered.
								
};
