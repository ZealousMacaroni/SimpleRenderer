/**
 * @file shader.h
 * @brief Contains tools needed for shader creation and use in rendering.
 */

#pragma once

#include <string>
#include <fstream>
#include <GL/glew.h>
#include <iostream>
#include <glm/gtc/type_ptr.hpp>

/**
 * @brief A simple function which reads a file.
 * @param Path The path of the file to read.
 * @return Returns an std::string which contains the contents of the file.
 * @todo Maybe focus on making this have less of a memory footprint, especially for large files.
 */
std::string GetContentFromFile(std::string Path) {
	// Creating file
	std::ifstream File(Path);
	
	if(!File.is_open()) {
		std::cout << "Error: GetContentFromFile: " << Path << " file does not exist.\n";
		return "";
	}
	
	// Strings for use later
	std::string Line, Source;
	
	// While there is content to read, run the loop.
	while(getline(File, Line)) {
		// Appending the source
		Source.append(Line + '\n');
	}
	
	// Returning the file contents
	return Source;
}

/**
 * @brief A simple class which handles shader creation and use w/uniforms. 
 * @todo Add a method which allows for checking to make sure that the data is in the shader.
 * @todo Implement more uniforms besides just matrices
 * @todo Make sure to implement a static vs dynamic thing for uniforms
 * @todo Make the matrices apply to the uniforms system instead of their own thing
 * @todo Add guards checking for using matrices
 * @note Matrix names must be uModel, uView, and uPerspective
 */
class ShaderInstance {
public:
	ShaderInstance() {}			// Default constructor
	/**
	 * @brief A constructor which creates the shader.
	 * @param VPath Filepath of the vertex shader.
	 * @param FPath Filepath of the fragment shader.
	 * @warning Paths are relative to where the program is executed from, not where shader.h is.
	 * @todo Try to lessen memory footprint with strings.
	 */
	ShaderInstance(std::string VPath, std::string FPath) {
		// Getting file contents
		std::string VSrcStr = GetContentFromFile(VPath);
		std::string FSrcStr = GetContentFromFile(FPath);
		
		// Turning file contents into C string
		const char* VSrc = VSrcStr.c_str();
		const char* FSrc = FSrcStr.c_str();
		
		// Creating vertex shader
		unsigned int VertexShader = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(VertexShader, 1, &VSrc, NULL);
		glCompileShader(VertexShader);
		
		// Compile error checking
		int Success;
		glGetShaderiv(VertexShader, GL_COMPILE_STATUS, &Success);
		if(!Success) {
			char InfoLog[512];
			glGetShaderInfoLog(VertexShader, 512, NULL, InfoLog);
			std::cout << "Error: ShaderInstance: ShaderInstance(): Vertex Shader compilation failed. Info Log: " << InfoLog << "\n";
		}
		
		// Creating fragment shader
		unsigned int FragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(FragmentShader, 1, &FSrc, NULL);
		glCompileShader(FragmentShader);
		
		// Compile error checking
		glGetShaderiv(FragmentShader, GL_COMPILE_STATUS, &Success);
		if(!Success) {
			char InfoLog[512];
			glGetShaderInfoLog(FragmentShader, 512, NULL, InfoLog);
			std::cout << "Error: ShaderInstance: ShaderInstance(): Fragment Shader compilation failed. Info Log: " << InfoLog << "\n";
		}
		
		// Creating program
		ID = glCreateProgram();
		glAttachShader(ID, VertexShader);
		glAttachShader(ID, FragmentShader);
		
		// Linking program
		glLinkProgram(ID);
		
		// Error checking
		glGetProgramiv(ID, GL_LINK_STATUS, &Success);
		if (!Success) {
			char InfoLog[512];
			glGetProgramInfoLog(ID, 512, NULL, InfoLog);
			std::cout << "Error: ShaderInstance: Shader program linking failed. Info Log: " << InfoLog << "\n";
		}
		
		// Deleting shader
		glDeleteShader(VertexShader);
		glDeleteShader(FragmentShader);
		
		// Getting uniform locations
		Model = glGetUniformLocation(ID, "uModel");
		View = glGetUniformLocation(ID, "uView");
		Perspective = glGetUniformLocation(ID, "uPerspective");
		
		// Confirming that the program has been created
		ProgramCreated = true;
		
	}
	
	/**
	 * 
	 */
	void UseModelMatrix(const float* Matrix) {
		glUniformMatrix4fv(Model, 1, GL_FALSE, Matrix);

		
	}
	
	/**
	 * 
	 */
	void UseViewMatrix(const float* Matrix) {
		 glUniformMatrix4fv(View, 1, GL_FALSE, Matrix);
		 
	}
	 
	/**
	 * 
	 */
	void UsePerspectiveMatrix(const float* Matrix) {
		glUniformMatrix4fv(Perspective, 1, GL_FALSE, Matrix);
		
	}
	
	/**
	 * @brief Function which uses the program stored in the class.
	 */
	void UseProgram() {
		if(!ProgramCreated) {
			std::cout << "Error: ShaderInstance: UseProgram(): Program has not been created.\n";
			return;
		}
		glUseProgram(ID);
	}
	
	/** 
	 * @brief Function which deletes the shader program.
	 */
	~ShaderInstance() {
		// Guard checking
		if(!ProgramCreated) {
			std::cout << "Error: ShaderInstance: Terminate(): Program has not been created.\n";
			return;
		}
		
		// Deleting program
		glDeleteProgram(ID);
		
	}
	
private:
	unsigned int ID;			// The OpenGL ID of the shader program.
	bool ProgramCreated = false;// A bool representing whether or not the program has been created.
	
	int Model = -1;					// Location of model matrix uniform
	int View = -1;					// Location of view matrix uniform
	int Perspective = -1;			// Location of perspective matrix uniform
	
};
