/**
 * @file object.h
 * @brief Contains the object data type for rendering.
 */

#pragma once

#include <shader.h>
#include <GL/glew.h>
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

/**
 * @class WorldDataInstance
 * @brief Stores data such as position and rotation for positioning within the world.
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
		
		// Generating model matrix
		GenerateMatrix();
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
		
		// Setting the model to identity to prevent undefined behavior
		Model = glm::mat4(1.0f);
		
		// Translating the model
		Model = glm::translate(Model, Position);
		
		// Rotating the model
		Model = glm::rotate(Model, glm::radians(Rotation.x), glm::vec3(1, 0, 0));
		Model = glm::rotate(Model, glm::radians(Rotation.y), glm::vec3(0, 1, 0));
		Model = glm::rotate(Model, glm::radians(Rotation.z), glm::vec3(0, 0, 1));
		
		// Scaling the model
		Model = glm::scale(Model, Scale);
		
		// Setting the guard
		HasModelMatrix = true;
		
	}
	
	/**
	 * @brief Function which gets the model matrix.
	 * @return Returns a const float pointer to the floats of the matrix. Column major order.
	 * @warning Modifying the data in the pointer can result in undefined behavior with GLM.
	 * @warning If the model matrix has not been made, a nullptr is returned.
	 */
	glm::mat4* GetModelMatrix() {
		// Checking guard
		if(!HasModelMatrix) {
			std::cout << "Error: WorldDataInstance: GetModelMatrix(): Model matrix has not been created.";
			return nullptr;
		}
		
		// Returning
		return &Model;
		
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
 * @todo Overload CreateVAO to support vectors and maybe even more data types.
 * @warning ShaderInstance must be created manually.
 * @warning The renderer must be initialized before creating any VAOs.
 * @todo For CanRender, print an error message or smth but not every frame
 * @todo Maybe add some identifiers so objects can be identified in errors
 */ 
class ObjectInstance {
public:
	ObjectInstance() {}			// Default constructor
	
	/**
	 * @brief Constructor that loads the shader and world data.
	 * @param _Shader The pointer to the shader which is inspected to be constructed to be used to render the object.
	 * @param _WorldData The data of the particular object that is used to position it in the world.
	 */
	ObjectInstance(ShaderInstance* _Shader, WorldDataInstance _WorldData) : WorldData(_WorldData), Shader(_Shader) {
		// Setting guards to true
		HasShader = true;
		HasWorldData = true;
	
	}
	
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
		glGenBuffers(1, &VBO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, VerticesCount * sizeof(glm::vec3), VerticesPointer, GL_STATIC_DRAW);
		
		// Creating index buffer object
		glGenBuffers(1, &IBO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, IndicesCount * sizeof(unsigned int), IndicesPointer, GL_STATIC_DRAW);
		
		// Vertex attributes
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, (void*)0);
		glEnableVertexAttribArray(0);
		
		// Deleting buffers, not needed because of VAO
		glBindVertexArray(0);
		//glDeleteBuffers(1, &VBO);
		//glDeleteBuffers(1, &IBO);
		
		// Setting the guard to true.
		HasVertexData = true;
		
	}
	
	/**
	 * @brief Function which uses the VAO.
	 * @warning if CreateVAO has not been called, this function will not do anything and will print an error.
	 */
	void UseVAO() {
		// Guard checking
		if(!HasVertexData) {
			std::cout << "Error: ObjectInstance: UseVAO(): VAO is not present.\n";
			return;
		}
		
		// Using VAO
		glBindVertexArray(VAO);
		
	}
	
	/** 
	 * @brief Gets a pointer to shader
	 * @return Returns a pointer to the ShaderInstance.
	 */
	ShaderInstance* GetShader() {
		return Shader;
		
	}
	
	/**
	 * @brief Function which gets the model matrix.
	 * @return Returns a const float pointer to the matrix in column major order.
	 * @warning Returns a nullptr if the class does not have any world data or vectors have not been created.
	 */
	glm::mat4* GetModelMatrix() {
		// Guard checking
		if(!HasWorldData) {
			std::cout << "Error: ObjectInstance: GetModelMatrix: WorldData is not present.\n";
			return nullptr;
		}
		
		// Returning
		return WorldData.GetModelMatrix();
		
	}
	
	/**
	 * @brief Function for checking whether or not the object is renderable.
	 * @return Returns a bool representing whether or not the object is renderable.
	 */
	bool CanRender() {
		// Checking guards
		if(HasShader && HasVertexData) {
			return true;
			
		} else {
			return false;
			
		}
		
	}
	
	/**
	 * @brief For rendering, gets the number of indices to be rendered.
	 * @returns Returns the number of indices to be rendered.
	 */
	int GetIndicesCount() {
		return IndicesCount;
		
	}
	
	/**
	 * @brief Function which deletes all OpenGL data associated with the program.
	 */
	~ObjectInstance() {
		// Guard checking
		if(!HasVertexData) {
			std::cout << "Error: ObjectInstance: Deconstructor: Buffers cannot be deleted because data is not present.\n";
			return;
		}
		
		// Deleting data
		glDeleteVertexArrays(1, &VAO);
		glDeleteBuffers(1, &VBO);
		glDeleteBuffers(1, &IBO);
		
	}
	
private:
	unsigned int VAO;			// Unsigned int storing the Vertex Array Object ID.
	unsigned int VBO, IBO;		// Buffers
	int IndicesCount;  			// Int storing the number of indices for the object.
	ShaderInstance* Shader;		// ShaderInstance pointer storing the address of the shader to be used on the object.
	WorldDataInstance WorldData;	// Struct storing all of the data for the model matrix and the matrix itself.
	bool HasShader = false;		// Bool guard determining whether or not the class has a shader.
	bool HasWorldData = false;	// Bool guard determining whether or not the class has WorldData.
	bool HasVertexData = false;	// Bool guard determining whether or not the VAO and IndicesCount have been created/initialized.
	
};
