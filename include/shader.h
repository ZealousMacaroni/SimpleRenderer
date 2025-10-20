/**
 * @file shader.h
 * @brief Contains tools needed for shader creation and use in rendering.
 */

#pragma once

#include "string.h"
#include <fstream>
#include <GL/glew.h>

/**
 * @brief A simple function which reads a file.
 * @param Path The path of the file to read.
 * @return Returns an std::string which contains the contents of the file.
 * @todo Maybe focus on making this have less of a memory footprint, especially for large files.
 */
 
std::string GetContentFromFile(std::string Path) {
	// Creating file
	std::ifstream File(Path);
	
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
 *
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
		glLinkProgram(ID);
		glValidateProgram(ID);
		
		// Deleting shader
		glDeleteShader(VertexShader);
		glDeleteShader(FragmentShader);
		
		// Confirming that the program has been created
		ProgramCreated = true;
		
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
	void Terminate() {
		if(!ProgramCreated) {
			std::cout << "Error: ShaderInstance: Terminate(): Program has not been created.\n";
			return;
		}
		glDeleteProgram(ID);
	}
	
private:
	unsigned int ID;			// The OpenGL ID of the shader program.
	bool ProgramCreated = false;// A bool representing whether or not the program has been created.
};
