/**
 * @file main.cpp
 * @note These are general todos for the renderer
 * @todo For guard checking/flags, see if there are any built in methods of the libraries which I can use.
 * @todo Go through and rewrite docs
 * @todo Either add a SimpleRenderer.h file to include everything or make a new file structure, or both
 * @todo Make camera function way better and way more easy to use
 * @todo Integrate WorldDataInstance into ObjectInstance.
 * @todo Instead of having guards, remove default constructors. Maybe
 */

#include <iostream>
// You can use this to effectively include everything
#include <renderer.h>

// Initializing static variables - will include this in different CPP file
int WindowInstance::WindowCount = 0;

int main() {
	
	// Beginning the timer
	Timer MainTimer;
	
	// Creating Window
	// Title, width, height, OpenGl version major, OpenGL version minor
	WindowInstance Window("Holy poo", 800, 800, 4, 1);
	
	// Creating Camera
	// Position, target, speed, sense, fov
	CameraInstance Camera(glm::vec3(0.0f, 0.0f, 3.0f), glm::vec3(0.0f, 0.0f, 1.0f), 0.05f, 0.1f, 45.0f);
	
	// Creating Renderer
	// Window, camera, minimun render range, maximum render range
	RendererInstance Renderer(&Window, &Camera, 0.1f, 100.0f);
	
	// Creating Shader
	ShaderInstance Shader("shaders/vert.glsl", "shaders/frag.glsl");
	
	// Vertices
	glm::vec3 Vertices[3] {
		glm::vec3(-0.5f, -0.5f,  0.0f),
		glm::vec3( 0.5f, -0.5f,  0.0f),
		glm::vec3( 0.0f,  0.5f,  0.0f)
	};
	
	// Indices
	unsigned int Indices[3] {
		0, 1, 2
	};
	
	// Creating Object
	// Shader, Scale, rotation, positions
	ObjectInstance Object(&Shader, glm::vec3(1.0f), glm::vec3(0.0f), glm::vec3(0.0f, 0.0f, 5.0f));
	Object.CreateVAO(Vertices, 3, Indices, 3);
	
	// Main loop
	while(!Window.ShouldWindowClose()) {
		// Starting frame
		Renderer.StartFrame();
		
		// Rendering Object
		Renderer.RenderObject(&Object);
		
		// Ending frame
		// Effectively the same as WindowInstance.FinishFrame()
		Renderer.FinishFrame();
		
		
	}
	
}
