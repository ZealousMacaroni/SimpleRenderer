/**
 * @file object.h
 * @brief Contains the object data type for rendering.
 */

#pragma once

#include <shader.h>
#include <glm/glm.hpp>
#include <GL/glew.h>
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

/**
 * @brief Stores data such as position and rotation for positioning within the world.
 * @class WorldDataInstance
 */
class WorldDataInstance {
public:
	WorldDataInstance() {} 		// Default constructor
	
	/**
	 * @brief Constructor which initializes data.
	 * @param _Scale vec3 representing the size of the object for each axis.
	 * @param _Rotation vec3 representing the rotation of the object for each axis, in degrees.
	 * @param _Position vec3 representing the position of the object in the world.
	 * @note The function simply calls SetData.
	 * @see SetData()
	 */
	WorldDataInstance(glm::vec3 _Scale, glm::vec3 _Rotation, glm::vec3 _Position) {
		SetData(_Scale, _Rotation, _Position);
		
	}
	 
	/**
	 * @brief Method which initializes data.
	 * @param _Scale vec3 representing the size of the object for each axis.
	 * @param _Rotation vec3 representing the rotation of the object for each axis, in degrees.
	 * @param _Position vec3 representing the position of the object in the world.
	 */
	void SetData(glm::vec3 _Scale, glm::vec3 _Rotation, glm::vec3 _Position) {
		// Setting data
		Scale = _Scale;
		Rotation = _Rotation;
		Position = _Position;
		
		// Setting guard to true
		HasVectorData = true;
		
	}
	
	/**
	 * @brief Calculates the model matrix from vec3s Rotation, Position, and Scale.
	 */
	void GenerateMatrix() {
		// Checking if vector data is present
		if(!HasVectorData) {
			std::cout << "Error: WorldDataInstance: GenerateMatrix(): No vector data present.\n";
			return;
		}
		
		// Scaling the model
		Model = glm::scale(Model, Scale);
		
		// Rotating the model
		// Loop for ease of use
		glm::vec3 Axis = glm::vec3(0.0f, 0.0f, 0.0f);
		for(int i = 0; i < 3; i++) {
			Axis[i] = 1.0f;
			Model = glm::rotate(Model, glm::radians(Rotation[i]), Axis);
			Axis[i] = 0.0f;
		}
		
		// Translating the model
		Model = glm::translate(Model, Position);
		
	}
	
	/**
	 * @brief Function which gets the model matrix.
	 * @return Returns a const float pointer to the floats of the matrix. Column major order.
	 * @warning Modifying the data in the pointer can result in undefined behavior with GLM.
	 * @warning If the model matrix has not been made, a nullptr is returned.
	 */
	const float* GetModelMatrix() {
		// Checking guard
		if(!HasModelMatrix) {
			std::cout << "Error: WorldDataInstance: GetModelMatrix(): Model matrix has not been created.";
			return nullptr;
		}
		
		// Returning
		return glm::value_ptr(Model);
		
	}
	
private:
	glm::vec3 Scale;			// vec3 representing the size of the object for each axis.
	glm::vec3 Rotation;			// vec3 representing the rotation of the object for each axis, in degrees.
	glm::vec3 Position;			// vec3 representing the position of the object in the world.
	glm::mat4 Model = glm::mat4(1.0f);	// Model Matrix
	bool HasVectorData = false;	// Bool guard representing whether or not the vectors have been initialized yet.
	bool HasModelMatrix = false;// Bool guard representing whether or not the model matrix has been made.
	
};

/**
 * @class ObjectInstance
 * @brief Stores all data needed for rendering.
 * @todo Implement a system of static vs dynamic memory for GPU.
 * @todo Figure out a better way to store shaders. Maybe a pointer.
 * @todo Overload CreateVAO to support vectors and maybe even more data types.
 * @todo Add some way to make sure there is data, a non issue for now.
 * @todo Make sure that there is data present in both the ShaderInstance and WorldDataInstance before rendering.
 * @warning ShaderInstance must be created manually.
 */ 
class ObjectInstance {
public:
	ObjectInstance() {}			// Default constructor
	
	/**
	 * @brief Constructor that loads the shader and world data.
	 * @param _Shader The pointer to the shader which is inspected to be constructed to be used to render the object.
	 * @param _WorldData The data of the particular object that is used to position it in the world.
	 */
	ObjectInstance(ShaderInstance* _Shader, WorldDataInstance _WorldData) : WorldData(_WorldData), Shader(_Shader) {}
	
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
		
		// Confirming the object has data.
		HasData = true;
		
	}
	
	/**
	 * @brief Function which deletes all OpenGL data associated with the program.
	 */
	void Terminate() {
		if(!HasData) {
			std::cout << "Error: ObjectInstance: Terminate(): Data has not been created.\n";
			return;
		}
		glDeleteVertexArrays(1, &VAO);
		Shader.Terminate();
		
	}
	
private:
	unsigned int VAO;			// Unsigned int storing the Vertex Array Object ID.
	int IndicesCount;  			// Unsigned int storing the number of indices for the object.
	ShaderInstance* Shader;		// ShaderInstance pointer storing the address of the shader to be used on the object.
	WorldDataInstance WorldData;	// Struct storing all of the data for the model matrix and the matrix itself.
	
};
