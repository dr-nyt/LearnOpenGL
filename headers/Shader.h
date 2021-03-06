#ifndef SHADER_H
#define SHADER_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

struct Material {
	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;
	float shininess;
};

struct Light {
	glm::vec3 position;

	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;
};

class Shader {
public:
	unsigned int ID;	// Program ID

	// Constructor reads and builds the shader
	Shader(const char* vertexPath, const char* fragmentPath) {
		// 1. Retrieve the vertex / fragment source code from filePath
		std::string vertexCode;
		std::string fragmentCode;
		std::ifstream vShaderFile;
		std::ifstream fShaderFile;
		// Ensure ifstream objects can throw exceptions
		vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
		fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

		try {
			// Open files
			vShaderFile.open(vertexPath);
			fShaderFile.open(fragmentPath);
			// Read file's buffer contents into streams
			std::stringstream vShaderStream, fShaderStream;
			vShaderStream << vShaderFile.rdbuf();
			fShaderStream << fShaderFile.rdbuf();
			// Close file handlers
			vShaderFile.close();
			fShaderFile.close();
			// Convert stream into string
			vertexCode = vShaderStream.str();
			fragmentCode = fShaderStream.str();
		}
		catch (std::ifstream::failure e) {
			std::cout << "ERROR::SHADER::FILE_NOT_SUCCESSFULLY_READ" << std::endl;
		}

		// 2. Compile shaders
		const char* vShaderCode = vertexCode.c_str();
		const char* fShaderCode = fragmentCode.c_str();

		unsigned int vertex, fragment;
		int success;
		char infoLog[512];

		// Create vertex shader
		vertex = glCreateShader(GL_VERTEX_SHADER);				// Create a vertex shader
		glShaderSource(vertex, 1, &vShaderCode, NULL);			// Attach the vertex shader source code
		glCompileShader(vertex);								// Compile the vertex shader
		// Check for vertex shader compile errors
		glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
		if (!success) {
			glGetShaderInfoLog(vertex, 512, NULL, infoLog);
			std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
		}

		// Create fragment shader
		fragment = glCreateShader(GL_FRAGMENT_SHADER);			// Create a fragment shader
		glShaderSource(fragment, 1, &fShaderCode, NULL);		// Attach the fragment shader source code
		glCompileShader(fragment);								// Compile the fragment shader
		// Check for fragment shader compile errors
		glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
		if (!success) {
			glGetShaderInfoLog(fragment, 512, NULL, infoLog);
			std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
		}

		// Shader program
		ID = glCreateProgram();
		glAttachShader(ID, vertex);
		glAttachShader(ID, fragment);
		glLinkProgram(ID);
		// Check for shader program linking errors
		glGetProgramiv(ID, GL_LINK_STATUS, &success);
		if (!success) {
			glGetProgramInfoLog(ID, 512, NULL, infoLog);
			std::cout << "ERROR::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
		}

		// Delete vertex and fragment shader instances as they have been linked
		glDeleteShader(vertex);
		glDeleteShader(fragment);
	}

	// Use / Activate the shader
	void use() {
		glUseProgram(ID);
	}

	void setMaterial(Material material) {
		setVec3("material.ambient", material.ambient);
		setVec3("material.diffuse", material.diffuse);
		setVec3("material.specular", material.specular);
		setFloat("material.shininess", material.shininess);
	}

	void setLight(Light light) {
		setVec3("light.position", light.position);
		setVec3("light.ambient", light.ambient);
		setVec3("light.diffuse", light.diffuse);
		setVec3("light.specular", light.specular);
	}

	void setBool(const std::string& name, bool value) const {
		glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
	}

	void setInt(const std::string& name, int value) const {
		glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
	}

	void setFloat(const std::string& name, float value) const {
		glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
	}

	void setFloat3(const std::string& name, float value1, float value2, float value3) const {
		glUniform3f(glGetUniformLocation(ID, name.c_str()), value1, value2, value3);
	}

	void setFloat4(const std::string& name, float value1, float value2, float value3, float value4) const {
		glUniform4f(glGetUniformLocation(ID, name.c_str()), value1, value2, value3, value4);
	}

	void setMat3(const std::string& name, const GLfloat* mat) {
		glUniformMatrix3fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, mat);
	}

	void setMat4(const std::string& name, const GLfloat* mat) {
		glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, mat);
	}

	void setVec3(const std::string& name, glm::vec3 vec) {
		glUniform3f(glGetUniformLocation(ID, name.c_str()), vec.x, vec.y, vec.z);
	}

	void setVec4(const std::string& name, glm::vec4 vec) {
		glUniform4f(glGetUniformLocation(ID, name.c_str()), vec.x, vec.y, vec.z, vec.w);
	}
};

#endif