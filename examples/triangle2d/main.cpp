// You can use this to effectively include everything
#include <SimpleRenderer/sr.h>

// Initializing static variables - will include this in different CPP file eventually but for now is neccessary
int WindowInstance::WindowCount = 0;

int main() {
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
	// Vertex shader path, fragment shader path
	ShaderInstance Shader("examples/triangle2d/shaders/vert.glsl", "examples/triangle2d/shaders/frag.glsl");
	
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
		// Effectively the same as WindowInstance.FinishFrame() but it is recommended to use this as the sameness between them may change at some point in the future
		Renderer.FinishFrame();
		
	}
	
}
