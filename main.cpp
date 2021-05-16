#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "Shader.h"
#include "camera.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include <iostream>

/* Resize OpenGL viewport when window size changes */
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
/* Handle user input */
void processInput(GLFWwindow* window);
/* Create window */
GLFWwindow* initWindow(int& width, int& height);

unsigned int createVAO();
void createVBO(float* vertices, int byteSize, int vertexLen);
void addVertexAttrib(int location, int attribLen, int vertexLen, int offset);
void createEBO(unsigned int* indices, int byteSize);
unsigned int createTexture(const char* path);
void calcFPS(int& nbFrames, double& lastTime);

int width = 1080;
int height = 720;

float mixValue = 0.5f;
float fov = 45.0f;

float deltaTime = 0.0f;
float lastFrame = 0.0f;

Camera camera = Camera(width, height);

int main() {

	// Create window
	GLFWwindow* window = initWindow(width, height);
	if (window == NULL) return -1;

	// Create shader program
	Shader shader1("vertex_shader_1.vs", "fragment_shader_1.fs");
	Shader shader2("vertex_shader_2.vs", "fragment_shader_2.fs");

	// Total system attributes
	int nrAttributes;
	glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &nrAttributes);
	std::cout << "Maximum nr of vertex attributes supported: " << nrAttributes << std::endl;

	float square[] = {
		// Position			// Color		  // Texture
		-0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,	// Bottom left
		 0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,	// Bottom right
		 0.5f,  0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f,	// Top right
		-0.5f,  0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f	// Top left
	};
	unsigned int squareIndices[] = {
		0, 1, 2,
		2, 3, 0
	};

	unsigned int texture1 = createTexture("image.png");
	unsigned int texture2 = createTexture("pattern.jpg");
	shader2.use();
	shader2.setInt("texture1", 0);
	shader2.setInt("texture2", 1);

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

	float cube[] = {
		-0.5f, -0.5f, -0.5f, 0.0f, 0.0f,
		 0.5f, -0.5f, -0.5f, 1.0f, 0.0f,
		 0.5f,  0.5f, -0.5f, 1.0f, 1.0f,
		 0.5f,  0.5f, -0.5f, 1.0f, 1.0f,
		-0.5f,  0.5f, -0.5f, 0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f, 0.0f, 0.0f,

		-0.5f, -0.5f,  0.5f, 0.0f, 0.0f,
		 0.5f, -0.5f,  0.5f, 1.0f, 0.0f,
		 0.5f,  0.5f,  0.5f, 1.0f, 1.0f,
		 0.5f,  0.5f,  0.5f, 1.0f, 1.0f,
		-0.5f,  0.5f,  0.5f, 0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f, 0.0f, 0.0f,

		-0.5f,  0.5f,  0.5f, 1.0f, 0.0f,
		-0.5f,  0.5f, -0.5f, 1.0f, 1.0f,
		-0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f, 0.0f, 0.0f,
		-0.5f,  0.5f,  0.5f, 1.0f, 0.0f,

		 0.5f,  0.5f,  0.5f, 1.0f, 0.0f,
		 0.5f,  0.5f, -0.5f, 1.0f, 1.0f,
		 0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
		 0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
		 0.5f, -0.5f,  0.5f, 0.0f, 0.0f,
		 0.5f,  0.5f,  0.5f, 1.0f, 0.0f,

		-0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
		 0.5f, -0.5f, -0.5f, 1.0f, 1.0f,
		 0.5f, -0.5f,  0.5f, 1.0f, 0.0f,
		 0.5f, -0.5f,  0.5f, 1.0f, 0.0f,
		-0.5f, -0.5f,  0.5f, 0.0f, 0.0f,
		-0.5f, -0.5f, -0.5f, 0.0f, 1.0f,

		-0.5f,  0.5f, -0.5f, 0.0f, 1.0f,
		 0.5f,  0.5f, -0.5f, 1.0f, 1.0f,
		 0.5f,  0.5f,  0.5f, 1.0f, 0.0f,
		 0.5f,  0.5f,  0.5f, 1.0f, 0.0f,
		-0.5f,  0.5f,  0.5f, 0.0f, 0.0f,
		-0.5f,  0.5f, -0.5f, 0.0f, 1.0f
	};
	glm::vec3 cubePositions[] = {
		glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3(2.0f, 5.0f, -15.0f),
		glm::vec3(-1.5f, -2.2f, -2.5f),
		glm::vec3(-3.8f, -2.0f, -12.3f),
		glm::vec3(2.4f, -0.4f, -3.5f),
		glm::vec3(-1.7f, 3.0f, -7.5f),
		glm::vec3(1.3f, -2.0f, -2.5f),
		glm::vec3(1.5f, 2.0f, -2.5f),
		glm::vec3(1.5f, 0.2f, -1.5f),
		glm::vec3(-1.3f, 1.0f, -1.5f)
	};

	unsigned int VAO = createVAO();
	createVBO(square, sizeof(square), 8);
	addVertexAttrib(0, 3, 8, 0);
	addVertexAttrib(1, 3, 8, 3);
	addVertexAttrib(2, 2, 8, 6);
	createEBO(squareIndices, sizeof(squareIndices));

	unsigned int VAO1 = createVAO();
	createVBO(triangle1, sizeof(triangle1), 3);
	addVertexAttrib(0, 3, 3, 0);

	unsigned int VAO2 = createVAO();
	createVBO(triangle2, sizeof(triangle2), 3);
	addVertexAttrib(0, 3, 3, 0);

	unsigned int VAO3 = createVAO();
	createVBO(rectangle, sizeof(rectangle), 3);
	addVertexAttrib(0, 3, 3, 0);
	createEBO(indices, sizeof(indices));

	unsigned int VAO4 = createVAO();
	createVBO(cube, sizeof(cube), 5);
	addVertexAttrib(0, 3, 5, 0);
	addVertexAttrib(2, 2, 5, 3);

	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);		// Wireframe mode

	// Model matrix
	glm::mat4 model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(0.5f, -0.5f, 0.0f));
	model = glm::rotate(model, glm::radians(-55.0f), glm::vec3(1.0f, 0.0f, 0.0f));

	// Perspective projection

	// Render loop
		// FPS
	double lastTime = glfwGetTime();
	int nbFrames = 0;
	do {
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		// Measure speed
		nbFrames++;
		calcFPS(nbFrames, lastTime);

		// Clear previous buffer
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Handle input
		processInput(window);

		// Update shader
		float timeValue = glfwGetTime();
		float colorValue = (sin(timeValue) / 2.0f) + 0.5f;
		float posValue = sin(timeValue);

		// Render
		//shader1.use();
		//// Draw rectangle
		//shader1.setFloat4("ourColor", colorValue, 0.0f, 0.0f, 1.0f);
		//glBindVertexArray(VAO3);
		//glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);						// Draw based on indices

		////shader1.use();
		//// Draw triangle 1
		//shader1.setFloat4("ourColor", 0.0f, colorValue, 0.0f, 1.0f);
		//glBindVertexArray(VAO1);
		//glDrawArrays(GL_TRIANGLES, 0, 3);

		//// Draw triangle 2
		//glBindVertexArray(VAO2);
		//glDrawArrays(GL_TRIANGLES, 0, 3);

		// Draw square
		//glBindVertexArray(VAO);
		glBindVertexArray(VAO4);
		shader2.use();

		glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)width / (float)height, 0.1f, 100.0f);
		glm::mat4 view = camera.GetViewMatrix();

		shader2.setMat4("projection", glm::value_ptr(projection));
		shader2.setMat4("view", glm::value_ptr(view));

		shader2.setFloat3("offset", posValue, 0.0f, 0.0f);
		shader2.setFloat("mixValue", mixValue);
		// Bind textures
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture1);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, texture2);

		for (unsigned int i = 0; i < 10; i++) {
			model = glm::mat4(1.0f);
			model = glm::translate(model, cubePositions[i % 10]);
			float angle = 20.0f * i;
			model = glm::rotate(model, glm::radians(angle) * timeValue, glm::vec3(1.0f, 0.3f, 0.5f));
			shader2.setMat4("model", glm::value_ptr(model));
			//glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);						// Draw based on indices
			glDrawArrays(GL_TRIANGLES, 0, 36);
		}
		// Unbind texture
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, 0);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, 0);

		// Call events and swap buffers
		glfwPollEvents();
		glfwSwapBuffers(window);
	} while (!glfwWindowShouldClose(window));

	glfwTerminate();
	return 0;
}

/* Resize OpenGL viewport when window size changes */
void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
}

/* Handle inputs on the window */
void processInput(GLFWwindow* window) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
	}
	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
		if (mixValue < 1.0f) mixValue += 0.01f;
	}
	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
		if (mixValue > 0.0f) mixValue -= 0.01f;
	}
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
		camera.ProcessKeyboard(FORWARD, deltaTime);
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
		camera.ProcessKeyboard(BACKWARD, deltaTime);
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
		camera.ProcessKeyboard(LEFT, deltaTime);
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
		camera.ProcessKeyboard(RIGHT, deltaTime);
	}
}

void mouse_callback(GLFWwindow* window, double xPos, double yPos) {
	camera.ProcessMouseMovement(xPos, yPos);
}

void scroll_callback(GLFWwindow* window, double xOffset, double yOffset) {
	camera.ProcessMouseScroll(yOffset);
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
	//glfwSwapInterval(0);				// Uncap fps

	// Initialize GLAD
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "Failed to initialize GLAD" << std::endl;
		glfwTerminate();
		return NULL;
	}

	// Setup Viewport
	glViewport(0, 0, width, height);										// Set OpenGL viewport size
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);		// Set OpenGL to call function to resize the viewport
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetCursorPosCallback(window, mouse_callback);
	glEnable(GL_DEPTH_TEST);			// Enable depth test using z-index

	glfwSetScrollCallback(window, scroll_callback);

	return window;
}

unsigned int createVAO() {
	// Create a Vertex Array Object (VAO) that stores vertex attribute configurations
	unsigned int VAO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	return VAO;
}

void createVBO(float* vertices, int byteSize, int vertexLen) {
	unsigned int VBO;
	glGenBuffers(1, &VBO);													// Generate Vertex Buffer Object
	glBindBuffer(GL_ARRAY_BUFFER, VBO);										// Bind the Array Buffer to use the VBO
	glBufferData(GL_ARRAY_BUFFER, byteSize, vertices, GL_STATIC_DRAW);		// Copy the Vertices Array to the bound Array Buffer
}

void addVertexAttrib(int location, int attribLen, int vertexLen, int offset) {
	glVertexAttribPointer(location, attribLen, GL_FLOAT, GL_FALSE, vertexLen * sizeof(float), (void*)(offset * (sizeof(float))));
	glEnableVertexAttribArray(location);
}

void createEBO(unsigned int* indices, int byteSize) {
	unsigned int EBO;
	glGenBuffers(1, &EBO);																	// Generate Element Buffer Object
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);												// Bind the EBO
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, byteSize, indices, GL_STATIC_DRAW);		// Set the EBO data to be indices
}

unsigned int createTexture(const char* path) {
	unsigned int texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	// Texture wrapping mode for each axis
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// Texture scaling mode with mipmaps
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// Load image
	int tWidth, tHeight, nrChannels;
	stbi_set_flip_vertically_on_load(true);
	unsigned char* data = stbi_load(path, &tWidth, &tHeight, &nrChannels, 0);
	// Create texture
	if (data) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, tWidth, tHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, data);	// Generate texture from data
		glGenerateMipmap(GL_TEXTURE_2D);	// Generate mipmaps
	}
	else std::cout << "Failed to load texture!" << std::endl;
	stbi_image_free(data);					// Free image memory

	return texture;
}

void calcFPS(int& nbFrames, double& lastTime) {
	double currentTime = glfwGetTime();
	nbFrames++;
	if (currentTime - lastTime >= 1.0) { // If last prinf() was more than 1 sec ago
		// printf and reset timer
		printf("%f fps\n", 1000.0 / (1000.0 / double(nbFrames)));
		printf("%f ms/frame\n", 1000.0 / double(nbFrames));
		nbFrames = 0;
		lastTime += 1.0;
	}
}
