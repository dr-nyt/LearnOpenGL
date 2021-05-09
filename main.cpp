#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "Shader.h"

/* Resize OpenGL viewport when window size changes */
void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
}

/* Handle inputs on the window */
void processInput(GLFWwindow* window) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
	}
}

/* Init GLFW, create window, init GLAD, set OpenGL viewport */
GLFWwindow* initWindow(int& width, int& height) {
	// Initialize GLFW
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);					// Set OpenGL major version [3]
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);					// Set OpenGL minor version 3.[3]
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);	// Set Modern OpenGL


	// Create window object
	GLFWwindow* window = glfwCreateWindow(width, height, "LearnOpenGL", NULL, NULL);
	if (window == NULL) {
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return window;
	}
	glfwMakeContextCurrent(window);		// Make the window the main context on this thread


	// Initialize GLAD
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "Failed to initialize GLAD" << std::endl;
		glfwTerminate();
		return NULL;
	}

	// Setup Viewport
	glViewport(0, 0, width, height);										// Set OpenGL viewport size
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);		// Set OpenGL to call function to resize the viewport

	return window;
}

unsigned int createVAO() {
	// Create a Vertex Array Object (VAO) that stores vertex attribute configurations
	unsigned int VAO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	return VAO;
}

template<size_t N>
void createVBO(float(&vertices)[N], int size) {
	unsigned int VBO;
	glGenBuffers(1, &VBO);															// Generate Vertex Buffer Object
	glBindBuffer(GL_ARRAY_BUFFER, VBO);												// Bind the Array Buffer to use the VBO
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);		// Copy the Vertices Array to the bound Array Buffer
	// Position
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, size * sizeof(float), (void*)0);	// Tell OpenGL how to read the vertex attributes
	glEnableVertexAttribArray(0);														// Activate the vertex attribute location 0
	// Color
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, size * sizeof(float), (void*)(3 * sizeof(float)));	// Tell OpenGL how to read the vertex attributes
	glEnableVertexAttribArray(1);																		// Activate the vertex attribute location 1
}

template<size_t N>
void createEBO(unsigned int(&indices)[N]) {
	unsigned int EBO;
	glGenBuffers(1, &EBO);																	// Generate Element Buffer Object
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);												// Bind the EBO
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);		// Set the EBO data to be indices
}

int main() {
	int width = 800;
	int height = 600;

	// Create window
	GLFWwindow* window = initWindow(width, height);
	if (window == NULL) return -1;

	// Create shader program
	Shader shader1("vertex_shader_1.vs", "fragment_shader_1.fs");
	Shader shader2("vertex_shader_2.vs", "fragment_shader_2.fs");

	int nrAttributes;
	glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &nrAttributes);
	std::cout << "Maximum nr of vertex attributes supported: " << nrAttributes << std::endl;


	float triangle[] = {
		-0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f,
		 0.0f,  0.5f, 0.0f, 0.0f, 1.0f, 0.0f,
		 0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f,
	};

	float triangle1[] = {
		-1.0f, -0.5f, 0.0f,
		-0.5f,  0.5f, 0.0f,
		 0.0f, -0.5f, 0.0f,
	};
	float triangle2[] = {
		 0.0f, -0.5f, 0.0f,
		 0.5f,  0.5f, 0.0f,
		 1.0f, -0.5f, 0.0f
	};

	float rectangle[] = {
		-0.5f, 1.0f, 0.0f,
		 0.5f, 1.0f, 0.0f,
		 0.5f, 0.5f, 0.0f,
		-0.5f, 0.5f, 0.0f,
	};
	// Splitting rectangle to two triangles
	unsigned int indices[] = {
		0, 1, 2,
		2, 3, 0
	};

	unsigned int VAO = createVAO();
	createVBO<18>(triangle, 6);

	unsigned int VAO1 = createVAO();
	createVBO<9>(triangle1, 3);

	unsigned int VAO2 = createVAO();
	createVBO<9>(triangle2, 3);

	unsigned int VAO4 = createVAO();
	createVBO<12>(rectangle, 3);
	createEBO(indices);

	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);		// Wireframe mode

	// Render loop
	while (!glfwWindowShouldClose(window)) {
		// Clear previous buffer
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		// Handle input
		processInput(window);

		// Update shader
		float timeValue = glfwGetTime();
		float colorValue = (sin(timeValue) / 2.0f) + 0.5f;

		// Render
		shader1.use();
		shader1.setFloat4("ourColor", colorValue, 0.0f, 0.0f, 1.0f);
		glBindVertexArray(VAO4);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);						// Draw based on indices

		//shader1.use();
		shader1.setFloat4("ourColor", 0.0f, colorValue, 0.0f, 1.0f);
		glBindVertexArray(VAO1);													// Load VAO
		glDrawArrays(GL_TRIANGLES, 0, 3);

		glBindVertexArray(VAO2);			// Load VAO
		glDrawArrays(GL_TRIANGLES, 0, 3);

		shader2.use();
		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 3);

		// Call events and swap buffers
		glfwPollEvents();
		glfwSwapBuffers(window);
	}

	glfwTerminate();
	return 0;
}