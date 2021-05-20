#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "headers/Shader.h"
#include "headers/camera.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#define STB_IMAGE_IMPLEMENTATION
#include "headers/stb_image.h"

#include <iostream>

// Window
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);
GLFWwindow* initWindow(int& width, int& height);

// OpenGL
unsigned int createVAO();
void createVBO(float* vertices, int byteSize, int vertexLen);
void addVertexAttrib(int location, int attribLen, int vertexLen, int offset);
void createEBO(unsigned int* indices, int byteSize);
unsigned int createTexture(const char* path);
void calcFPS(int& nbFrames, double& lastTime);

// Global Variables
int width = 1080;
int height = 720;

float mixValue = 0.5f;
float fov = 45.0f;

float deltaTime = 0.0f;
float lastFrame = 0.0f;

Camera camera = Camera(width, height);

glm::vec3 lightOffset(1.0f, 1.0f, 4.0f);
glm::vec3 lightPos(0.0f, 0.0f, 4.0f);					// Light source position
glm::vec3 lightColor(1.0f);

int main()
{
	// Create window
	GLFWwindow* window = initWindow(width, height);
	if (window == NULL)
		return -1;

	// Create shader program
	Shader shader1("shaders/vertex_shader_1.vs", "shaders/fragment_shader_1.fs");
	Shader shader2("shaders/vertex_shader_2.vs", "shaders/fragment_shader_2.fs");
	Shader lightingShader("shaders/lighting.vs", "shaders/lighting.fs");
	Shader lightShader("shaders/lighting.vs", "shaders/light.fs");

	// Total system attributes
	int nrAttributes;
	glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &nrAttributes);
	std::cout << "Maximum nr of vertex attributes supported: " << nrAttributes << std::endl;

	// Create textures
	unsigned int texture1 = createTexture("rsc/imgs/image.png");
	unsigned int texture2 = createTexture("rsc/imgs/pattern.jpg");
	shader2.use();
	shader2.setInt("texture1", 0);
	shader2.setInt("texture2", 1);

	// Cube vertices
	float cube[] = {
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		 0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,

		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
		 0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,

		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
		-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
		-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,

		 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
		 0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
		 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
		 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
		 0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
		 0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,

		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
	};
	int cubeVertexLen = 6;
	int cubeVertexCount = (sizeof(cube) / sizeof(*cube)) / cubeVertexLen;

	// VAO & VBO for the object
	unsigned int objectVAO = createVAO();
	createVBO(cube, sizeof(cube), cubeVertexLen);
	addVertexAttrib(0, 3, cubeVertexLen, 0); // Attribute 0 for the vertex coordinates
	addVertexAttrib(1, 3, cubeVertexLen, 3); // Attribute 1 for the normal vecotr

	// VAO & VBO for the light source
	unsigned int lightVAO = createVAO();
	createVBO(cube, sizeof(cube), cubeVertexLen);
	addVertexAttrib(0, 3, cubeVertexLen, 0); // Attribute 0 for the vertex coordinates
	addVertexAttrib(1, 3, cubeVertexLen, 3); // Attribute 1 for the normal vecotr

	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);		// Wireframe mode

	// Light source model matrix
	glm::mat4 lightModel = glm::mat4(1.0f);
	lightModel = glm::translate(lightModel, lightPos);
	lightModel = glm::scale(lightModel, glm::vec3(0.2f));

	// Object mode matrix
	glm::mat4 model = glm::mat4(1.0f);
	glm::mat4 wireModel = glm::scale(model, glm::vec3(1.05f));

	// Materials
	Material ruby = {
		glm::vec3(0.1745f, 0.01175f, 0.01175f),
		glm::vec3(0.61424f, 0.04136f, 0.04136f),
		glm::vec3(0.727811f, 0.626959f, 0.626959f),
		0.6f
	};

	Material gold = {
		glm::vec3(0.24725f, 0.1995f, 0.0745f),
		glm::vec3(0.75164f, 0.60648f, 0.22648f),
		glm::vec3(0.628281f, 0.555802f, 0.366065f),
		0.4f
	};

	Material mat = {
		glm::vec3(1.0f, 0.5f, 0.31f),
		glm::vec3(1.0f, 0.5f, 0.31f),
		glm::vec3(0.5f, 0.5f, 0.5f),
		0.25f
	};

	// Render loop
	double lastTime = glfwGetTime();
	int nbFrames = 0;
	do
	{
		// Calculate delta time
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		// Calculate FPS
		nbFrames++;
		calcFPS(nbFrames, lastTime);

		// Clear previous color and depth buffer
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClearStencil(0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

		// Handle input
		processInput(window);

		// Update time values
		float timeValue = glfwGetTime();
		float colorValue = (sin(timeValue) / 2.0f) + 0.5f;
		float posValue = sin(timeValue);

		// Camera
		glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)width / (float)height, 0.1f, 100.0f);
		glm::mat4 view = camera.GetViewMatrix();

		// Light movement
		int radius = 3;
		//lightPos = lightOffset;
		lightPos = glm::vec3(cos(timeValue) * radius, 0.0f, sin(timeValue) * radius);
		//lightColor = glm::vec3(cos(timeValue), 0.0f, sin(timeValue));
		glm::vec3 diffuseColor = lightColor;
		glm::vec3 ambientColor = lightColor;

		// Render Light
		glBindVertexArray(lightVAO);
		lightShader.use();

		lightModel = glm::mat4(1.0f);
		lightModel = glm::translate(lightModel, lightPos);
		lightModel = glm::scale(lightModel, glm::vec3(0.2f));
		glm::mat3 tiLightModel = glm::transpose(glm::inverse(lightModel));

		lightShader.setVec3("lightColor", lightColor);
		lightShader.setMat4("model", glm::value_ptr(lightModel));
		lightShader.setMat4("view", glm::value_ptr(view));
		lightShader.setMat4("projection", glm::value_ptr(projection));
		lightShader.setMat3("tiModel", glm::value_ptr(tiLightModel));
		glDrawArrays(GL_TRIANGLES, 0, cubeVertexCount);

		Light light = {
			lightPos,
			ambientColor,
			diffuseColor,
			lightColor,
		};

		// Render Object
		glBindVertexArray(objectVAO);
		lightingShader.use();

		model = glm::mat4(1.0f);
		glm::mat3 tiModel = glm::transpose(glm::inverse(model));

		lightingShader.setMaterial(gold);
		lightingShader.setLight(light);
		lightingShader.setVec3("ViewPos", camera.Position);

		lightingShader.setMat4("model", glm::value_ptr(model));
		lightingShader.setMat4("view", glm::value_ptr(view));
		lightingShader.setMat4("projection", glm::value_ptr(projection));
		lightingShader.setMat3("tiModel", glm::value_ptr(tiModel));
		glDrawArrays(GL_TRIANGLES, 0, cubeVertexCount);

		model = glm::translate(model, glm::vec3(-1.0f, 0.0f, 0.0f));
		tiModel = glm::transpose(glm::inverse(model));
		lightingShader.setMat4("model", glm::value_ptr(model));
		lightingShader.setMat3("tiModel", glm::value_ptr(tiModel));
		lightingShader.setMaterial(ruby);
		glDrawArrays(GL_TRIANGLES, 0, cubeVertexCount);

		model = glm::translate(model, glm::vec3(2.0f, 0.0f, 0.0f));
		tiModel = glm::transpose(glm::inverse(model));
		lightingShader.setMat4("model", glm::value_ptr(model));
		lightingShader.setMat3("tiModel", glm::value_ptr(tiModel));
		lightingShader.setMaterial(mat);
		glDrawArrays(GL_TRIANGLES, 0, cubeVertexCount);

		// Render the mesh into the stencil buffer.
		//glEnable(GL_STENCIL_TEST);

		//glStencilFunc(GL_ALWAYS, 1, -1);
		//glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);

		//glDrawArrays(GL_TRIANGLES, 0, cubeVertexCount);

		//// Render the thick wireframe version.
		//glStencilFunc(GL_NOTEQUAL, 1, -1);
		//glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);

		//glLineWidth(3);
		//glPolygonMode(GL_FRONT, GL_LINE);

		//lightShader.use();
		//lightShader.setFloat3("lightColor", 1.0f, 0.0f, 0.0f);
		//lightShader.setMat4("model", glm::value_ptr(wireModel));
		//glDrawArrays(GL_TRIANGLES, 0, cubeVertexCount);


		// Call events and swap buffers
		glfwPollEvents();
		glfwSwapBuffers(window);

	} while (!glfwWindowShouldClose(window));

	glfwTerminate();
	return 0;
}

/* Resize OpenGL viewport when window size changes */
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

/* Handle inputs on the window */
void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, true);
	}
	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
	{
		if (mixValue < 1.0f)
			mixValue += 0.01f;
	}
	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
	{
		if (mixValue > 0.0f)
			mixValue -= 0.01f;
	}
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
	{
		camera.ProcessKeyboard(FORWARD, deltaTime);
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
	{
		camera.ProcessKeyboard(BACKWARD, deltaTime);
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
	{
		camera.ProcessKeyboard(LEFT, deltaTime);
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
	{
		camera.ProcessKeyboard(RIGHT, deltaTime);
	}
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
	{
		camera.ProcessKeyboard(UP, deltaTime);
	}
	if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
	{
		camera.ProcessKeyboard(DOWN, deltaTime);
	}
	if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
	{
		camera.boost = true;
	}
	if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_RELEASE)
	{
		camera.boost = false;
	}
}

/* Handles mouse input */
void mouse_callback(GLFWwindow* window, double xPos, double yPos)
{
	camera.ProcessMouseMovement(xPos, yPos);
}

/* Handles scroll input */
void scroll_callback(GLFWwindow* window, double xOffset, double yOffset)
{
	camera.ProcessMouseScroll(yOffset);
}

/* Init GLFW, create window, init GLAD, set OpenGL viewport */
GLFWwindow* initWindow(int& width, int& height)
{
	// Initialize GLFW
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);				   // Set OpenGL major version [3]
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);				   // Set OpenGL minor version 3.[3]
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // Set Modern OpenGL

	// Create window object
	GLFWwindow* window = glfwCreateWindow(width, height, "LearnOpenGL", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return window;
	}
	glfwMakeContextCurrent(window); // Make the window the main context on this thread
	//glfwSwapInterval(0);				// Uncap fps

	// Initialize GLAD
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		glfwTerminate();
		return NULL;
	}

	// Setup Viewport
	glViewport(0, 0, width, height);								   // Set OpenGL viewport size
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback); // Set OpenGL to call function to resize the viewport
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetCursorPosCallback(window, mouse_callback);
	glEnable(GL_DEPTH_TEST); // Enable depth test using z-index

	glfwSetScrollCallback(window, scroll_callback);

	return window;
}

/* Create a Vertex Array Object (VAO) that stores vertex attribute configurations */
unsigned int createVAO()
{
	unsigned int VAO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	return VAO;
}

/* Create a VBO */
void createVBO(float* vertices, int byteSize, int vertexLen)
{
	unsigned int VBO;
	glGenBuffers(1, &VBO);											   // Generate Vertex Buffer Object
	glBindBuffer(GL_ARRAY_BUFFER, VBO);								   // Bind the Array Buffer to use the VBO
	glBufferData(GL_ARRAY_BUFFER, byteSize, vertices, GL_STATIC_DRAW); // Copy the Vertices Array to the bound Array Buffer
}

/* Add a vertex attribute to a VBO for shaders to use */
void addVertexAttrib(int location, int attribLen, int vertexLen, int offset)
{
	glVertexAttribPointer(location, attribLen, GL_FLOAT, GL_FALSE, vertexLen * sizeof(float), (void*)(offset * (sizeof(float))));
	glEnableVertexAttribArray(location);
}

/* Create EBO for indices */
void createEBO(unsigned int* indices, int byteSize)
{
	unsigned int EBO;
	glGenBuffers(1, &EBO);													  // Generate Element Buffer Object
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);								  // Bind the EBO
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, byteSize, indices, GL_STATIC_DRAW); // Set the EBO data to be indices
}

/* Create a texture from given path */
unsigned int createTexture(const char* path)
{
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
	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, tWidth, tHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, data); // Generate texture from data
		glGenerateMipmap(GL_TEXTURE_2D);															// Generate mipmaps
	}
	else
		std::cout << "Failed to load texture!" << std::endl;
	stbi_image_free(data); // Free image memory

	return texture;
}

/* Calculate and log fps to the console */
void calcFPS(int& nbFrames, double& lastTime)
{
	double currentTime = glfwGetTime();
	nbFrames++;
	if (currentTime - lastTime >= 1.0)
	{ // If last prinf() was more than 1 sec ago
		// printf and reset timer
		printf("%f fps\n", 1000.0 / (1000.0 / double(nbFrames)));
		printf("%f ms/frame\n", 1000.0 / double(nbFrames));
		nbFrames = 0;
		lastTime += 1.0;
	}
}
