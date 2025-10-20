/**
 * @file object.h
 * @brief Contains the object data type for rendering.
 */

#pragma once

#include <shader.h>
#include <glm/glm.hpp>
#include <GL/glew.h>

/**
 * @brief Stores all data needed for rendering.
 * @todo Implement a system of static vs dynamic memory for GPU.
 * @todo Figure out a better way to store shaders. Maybe a pointer.
 * @todo Overload CreateVAO to support vectors and maybe even more data types.
 */ 
struct ObjectInstance {
	/**
	 * @brief Method that creates the VAO and initiates IndicesCount through regular arrays.
	 * @param VerticesPointer Pointer to the vertices. Expects vertices to be composed of glm::vec3s.
	 * @param VerticesCount Number of vertices. 
	 * @param IndicesPointer Pointer to the indices. Expects indices to be composed of unsigned ints.
	 * @param _IndicesCount Number of indices.
	 * @warning Only supports data in contiguous blocks of memory.
	 */
	void CreateVAO(glm::vec3* VerticesPointer, int VerticesCount, unsigned int* IndicesPointer, int _IndicesCount) {
		// Initializing IndicesCount
		IndicesCount = _IndicesCount;
		
		// Creating vertex array object
		glGenVertexArrays(1, &VAO);
		glBindVertexArray(VAO);
		
		// Creating vertex buffer object
		unsigned int VBO;
		glGenBuffers(1, &VBO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, VerticesCount * sizeof(glm::vec3), VerticesPointer, GL_STATIC_DRAW);
		
		// Creating index buffer object
		unsigned int IBO;
		glGenBuffers(1, &IBO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, IndicesCount * sizeof(unsigned int), IndicesPointer, GL_STATIC_DRAW);
		
		// Vertex attributes
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, (void*)0);
		glEnableVertexAttribArray(0);
		
		// Deleting buffers, not needed because of VAO
		glBindVertexArray(0);
		glDeleteBuffers(1, &VBO);
		glDeleteBuffers(1, &IBO);
		
	}
	
	unsigned int VAO;			// Unsigned int storing the Vertex Array Object ID.
	int IndicesCount;  			// Unsigned int storing the number of indices for the object.
	ShaderInstance Shader;		// ShaderInstance storing the shader to be used on the object.
	bool HasData = false;		// Bool storing whether or not the object has had its data created.
};
