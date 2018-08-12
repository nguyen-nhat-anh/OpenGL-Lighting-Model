#include <glad\glad.h>
#include <GLFW\glfw3.h>
#include <iostream>
#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtc\type_ptr.hpp>
#include "Shader.h"
#include "Camera.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow *window);

// screen settings
const unsigned int SCR_WIDTH = 700;
const unsigned int SCR_HEIGHT = 700;

// initial camera settings
Camera camera = Camera(glm::vec3(0.02f, 0.74f, 3.46f), glm::vec3(0.0f, 1.0f, 0.0f), -89.4f, -13.8f);
// initial mouse position
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

// timing
float deltaTime = 0.0f; // time between current frame and last frame
float lastFrame = 0.0f;

// lighting
struct Light
{
	glm::vec3 position;
	
	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;

	//attenuation constants
	float constant;
	float linear;
	float quadratic;
};

struct Material
{
	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;

	float shininess;
};

int main(void)
{
	// Initialize GLFW
	// ---------------
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	// ---------------

	// Create window
	// -------------
	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Flat Shading", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	// -------------

	// Set callback functions
	// ----------------------
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);
	// ----------------------

	// tell GLFW to capture our mouse
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// GLAD: load all OpenGL function pointers
	// ---------------------------------------
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}
	// ---------------------------------------

	// configure global opengl state
	glEnable(GL_DEPTH_TEST); // enable z-buffer depth test

	// build and compile shader program
	// --------------------------------
	Shader myShader = Shader("../FlatShading/shader.vert", "../FlatShading/shader.frag");
	// --------------------------------


	// set up vertex data (and buffers) and configure vertex attributes
	// ----------------------------------------------------------------
	 // contain both vertices' position and normal vector data
	float vertices[] =
	{
		// position					// normal
		1.0f,     0.0f,      0.5f,  0.98079f, 0.19510f, 0.0f,
		1.0f,     0.0f,     -0.5f,  0.98079f, 0.19510f, 0.0f,
		0.92388f, 0.38268f, -0.5f,  0.98079f, 0.19510f, 0.0f,

		1.0f,     0.0f,      0.5f,  0.98079f, 0.19510f, 0.0f,
		0.92388f, 0.38268f, -0.5f,  0.98079f, 0.19510f, 0.0f,
		0.92388f, 0.38268f,  0.5f,  0.98079f, 0.19510f, 0.0f,

		0.92388f, 0.38268f,  0.5f,  0.83147f, 0.55557f, 0.0f,
		0.92388f, 0.38268f, -0.5f,  0.83147f, 0.55557f, 0.0f,
		0.70711f, 0.70711f, -0.5f,  0.83147f, 0.55557f, 0.0f,

		0.92388f, 0.38268f,  0.5f,  0.83147f, 0.55557f, 0.0f,
		0.70711f, 0.70711f, -0.5f,  0.83147f, 0.55557f, 0.0f,
		0.70711f, 0.70711f,  0.5f,  0.83147f, 0.55557f, 0.0f,

		0.70711f, 0.70711f,  0.5f,  0.55557f, 0.83147f, 0.0f,
		0.70711f, 0.70711f, -0.5f,  0.55557f, 0.83147f, 0.0f,
		0.38268f, 0.92388f, -0.5f,  0.55557f, 0.83147f, 0.0f,

		0.70711f, 0.70711f,  0.5f,  0.55557f, 0.83147f, 0.0f,
		0.38268f, 0.92388f, -0.5f,  0.55557f, 0.83147f, 0.0f,
		0.38268f, 0.92388f,  0.5f,  0.55557f, 0.83147f, 0.0f,

		0.38268f, 0.92388f,  0.5f,  0.19510f, 0.98079f, 0.0f,
		0.38268f, 0.92388f, -0.5f,  0.19510f, 0.98079f, 0.0f,
		0.0f,     1.0f,     -0.5f,  0.19510f, 0.98079f, 0.0f,

		0.38268f, 0.92388f,  0.5f,  0.19510f, 0.98079f, 0.0f,
		0.0f,     1.0f,     -0.5f,  0.19510f, 0.98079f, 0.0f,
		0.0f,     1.0f,      0.5f,  0.19510f, 0.98079f, 0.0f,

		0.0f,     1.0f,      0.5f, -0.19510f, 0.98079f, 0.0f,
		0.0f,     1.0f,     -0.5f, -0.19510f, 0.98079f, 0.0f,
	   -0.38268f, 0.92388f, -0.5f, -0.19510f, 0.98079f, 0.0f,

		0.0f,     1.0f,      0.5f, -0.19510f, 0.98079f, 0.0f,
       -0.38268f, 0.92388f, -0.5f, -0.19510f, 0.98079f, 0.0f,
	   -0.38268f, 0.92388f,  0.5f, -0.19510f, 0.98079f, 0.0f,

	   -0.38268f, 0.92388f,  0.5f, -0.55557f, 0.83147f, 0.0f,
	   -0.38268f, 0.92388f, -0.5f, -0.55557f, 0.83147f, 0.0f,
	   -0.70711f, 0.70711f, -0.5f, -0.55557f, 0.83147f, 0.0f,

	   -0.38268f, 0.92388f,  0.5f, -0.55557f, 0.83147f, 0.0f,
	   -0.70711f, 0.70711f, -0.5f, -0.55557f, 0.83147f, 0.0f,
	   -0.70711f, 0.70711f,  0.5f, -0.55557f, 0.83147f, 0.0f,

	   -0.70711f, 0.70711f,  0.5f, -0.83147f, 0.55557f, 0.0f,
	   -0.70711f, 0.70711f, -0.5f, -0.83147f, 0.55557f, 0.0f,
	   -0.92388f, 0.38268f, -0.5f, -0.83147f, 0.55557f, 0.0f,

	   -0.70711f, 0.70711f,  0.5f, -0.83147f, 0.55557f, 0.0f,
	   -0.92388f, 0.38268f, -0.5f, -0.83147f, 0.55557f, 0.0f,
	   -0.92388f, 0.38268f,  0.5f, -0.83147f, 0.55557f, 0.0f,

	   -0.92388f, 0.38268f,  0.5f, -0.98079f, 0.19510f, 0.0f,
	   -0.92388f, 0.38268f, -0.5f, -0.98079f, 0.19510f, 0.0f,
	   -1.0f,     0.0f,     -0.5f, -0.98079f, 0.19510f, 0.0f,

	   -0.92388f, 0.38268f,  0.5f, -0.98079f, 0.19510f, 0.0f,
	   -1.0f,     0.0f,     -0.5f, -0.98079f, 0.19510f, 0.0f,
	   -1.0f,     0.0f,      0.5f, -0.98079f, 0.19510f, 0.0f,
	};
	// ----------------------------------------------------------------

	// Lighting model
	// --------------
	 // Light
	Light light;
	
	light.position = glm::vec3(0.0f, 0.1f, 1.5f);
	
	light.ambient = glm::vec3(0.5f, 0.5f, 0.5f);
	light.diffuse = glm::vec3(1.0f, 1.0f, 1.0f);
	light.specular = glm::vec3(1.0f, 1.0f, 1.0f);
	
	light.constant = 1.0f;
	light.linear = 0.22f;
	light.quadratic = 0.20f;
	 // Material (chrome)
	Material material;
	
	material.ambient = glm::vec3(0.25f, 0.25f, 0.25f);
	material.diffuse = glm::vec3(0.4f, 0.4f, 0.4f);
	material.specular = glm::vec3(0.774597f, 0.774597f, 0.774597f);

	material.shininess = 0.6f * 128.0f;
	// --------------


	// Setup buffers and configure vertex attributes
	// ---------------------------------------------
	 // create Vertex Buffer Object, Vertex Array Object
	unsigned int VBO, VAO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	 // bind Vertex Array Object
	glBindVertexArray(VAO);
	 // bind buffer to buffer type target
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	 // copy vertices data into buffer's memory
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	 // configure vertex attributes
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0); // attribute 0: position
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float))); // attribute 1: normal vector
	glEnableVertexAttribArray(1);
	// ---------------------------------------------

	// Wireframe mode
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	// Render loop
	// -----------
	while (!glfwWindowShouldClose(window))
	{
		// Per-frame time logic
		// --------------------
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;
		// --------------------

		// Process keyboard input
		processInput(window);

		// Render
		// ------
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		// ------

		// Viewing and geometric transformations: Model - View - Projection matrices
		// -------------------------------------------------------------------------
		glm::mat4 view;
		glm::mat4 projection;
		glm::mat4 model;
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		view = camera.GetViewMatrix();
		projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
		// -------------------------------------------------------------------------

		// Draw
		// ----
		myShader.use();

		glUniformMatrix4fv(glGetUniformLocation(myShader.ID, "view"), 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(glGetUniformLocation(myShader.ID, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(glGetUniformLocation(myShader.ID, "model"), 1, GL_FALSE, glm::value_ptr(model));

		glUniform3fv(glGetUniformLocation(myShader.ID, "light.position"), 1, &light.position[0]);
		glUniform3fv(glGetUniformLocation(myShader.ID, "light.ambient"), 1, &light.ambient[0]);
		glUniform3fv(glGetUniformLocation(myShader.ID, "light.diffuse"), 1, &light.diffuse[0]);
		glUniform3fv(glGetUniformLocation(myShader.ID, "light.specular"), 1, &light.specular[0]);
		glUniform1f(glGetUniformLocation(myShader.ID, "light.constant"), light.constant);
		glUniform1f(glGetUniformLocation(myShader.ID, "light.linear"), light.linear);
		glUniform1f(glGetUniformLocation(myShader.ID, "light.quadratic"), light.quadratic);

		glUniform3fv(glGetUniformLocation(myShader.ID, "material.ambient"), 1, &material.ambient[0]);
		glUniform3fv(glGetUniformLocation(myShader.ID, "material.diffuse"), 1, &material.diffuse[0]);
		glUniform3fv(glGetUniformLocation(myShader.ID, "material.specular"), 1, &material.specular[0]);
		glUniform1f(glGetUniformLocation(myShader.ID, "material.shininess"), material.shininess);

		glUniform3fv(glGetUniformLocation(myShader.ID, "viewPos"), 1, &camera.Position[0]);

		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 48);
		// ----

		// GLFW: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
		// -------------------------------------------------------------------------------
		glfwSwapBuffers(window);
		glfwPollEvents();
		// -------------------------------------------------------------------------------
	}
	// -----------

	glfwTerminate();
	
	return 0;
}

// GLFW: whenever the window size changed (by OS or user resize) this callback function executes
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	std::cout << "called framebuffer_size_callback" << std::endl;
	glViewport(0, 0, width, height);
}

// GLFW: whenever the mouse moves, this callback is called
void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top
	lastX = xpos;
	lastY = ypos;

	camera.ProcessMouseMovement(xoffset, yoffset);
}

// GLFW: whenever the mouse scroll wheel scrolls, this callback is called
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	camera.ProcessMouseScroll(yoffset);
}

// Process all keyboard input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camera.ProcessKeyboard(FORWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera.ProcessKeyboard(BACKWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera.ProcessKeyboard(LEFT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera.ProcessKeyboard(RIGHT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
		camera.ProcessKeyboard(DOWN, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
		camera.ProcessKeyboard(UP, deltaTime);
}