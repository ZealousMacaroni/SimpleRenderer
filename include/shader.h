/**
 * @file shader.h
 * @brief Contains tools needed for shader creation and use in rendering.
 */

#pragma once

#include "string.h"
#include <fstream>

/**
 * @brief A simple class which handles shader creation and use w/uniforms. 
 *
 */
class ShaderInstance {
	ShaderInstance() {}			// Default constructor
	
};

/**
 * @brief A simple function which reads a file.
 * @param Path The path of the file to read.
 * @return Returns an std::string which contains the contents of the file.
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
