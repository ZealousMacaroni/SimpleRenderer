/**
 * @file renderer.h
 * @brief Contains the main renderer.
 * @note Todos are general, not specific to this
 * @todo add more robust guards checking on function returns.
 * @todo Sort out the naming of StartFrame and EndFrame on the 
 */
 
#pragma once

#include "window.h"
#include "object.h"
#include "camera.h"

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
		
	}
	
	/**
	 * 
	 */
	void StartFrame() {
		// Color stuffs
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
	}
	/**
	 * 
	 */
	void FinishFrame() {
		Window->FinishFrame();
	}
	
	/**
	 * @brief Renders a single object from a pointer, Object.
	 */
	void RenderObject(ObjectInstance* Object) {
		if(Object->CanRender()) {
			Object->UseVAO();
			Object->UseShader();
			glDrawElements(GL_TRIANGLES, Object->GetIndicesCount(), GL_UNSIGNED_INT, 0);
		}
	}
	
	
private:
	WindowInstance* Window;		// Window
	CameraInstance* Camera;		// Camera 
								
};
