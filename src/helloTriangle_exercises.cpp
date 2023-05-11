#include <glad/glad.h>
#include <glfw/glfw3.h>

#include <iostream>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

// vertex shader
const char* vertexShaderSource = "#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "void main()\n"
    "{\n"
    "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
    "}\n\0";
const char* fragmentShaderSource1 = "#version 330 core\n"
	"out vec4 FragColor;\n"
	"void main()\n"
	"{\n"
	"	FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
	"}\n\0";
const char* fragmentShaderSource2 = "#version 330 core\n"
	"out vec4 FragColor;\n"
	"void main()\n"
	"{\n"
	"	FragColor = vec4(1.0f, 1.0f, 0.2f, 1.0f);\n"
	"}\n\0";

//// notes ////
// OpenGL maps coords from (-1 to 1) to (0, width) and (0, height)
	// ex. (-0.5, 0.5) == (200, 450)

int main()
{
	// initialize GLFW, set context options for version 3.3 using the core profile
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// create a window object, 800 x 600, named LearnOpenGL
	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	// make the window context the main context on the current thread
	glfwMakeContextCurrent(window);
	// setup viewport resizing with GLFW
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	// initializing GLAD to manage function pointers before we call OpenGL functions
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to init GLAD" << std::endl;
		return -1;
	}


//////// VERTEX SHADER ////

	// setup vertex shader object
	unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);

//////// FRAGMENT SHADER ////

	// very similar thing as the vertex shader object setup
	unsigned int fragmentShaderOrange = glCreateShader(GL_FRAGMENT_SHADER);
	unsigned int fragmentShaderYellow = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShaderOrange, 1, &fragmentShaderSource1, NULL);
	glCompileShader(fragmentShaderOrange);
	glShaderSource(fragmentShaderYellow, 1, &fragmentShaderSource2, NULL);
	glCompileShader(fragmentShaderYellow);

//////// SHADER PROGRAM //// - linked version of multiple shaders
	
	unsigned int shaderProgramOrange = glCreateProgram();
	unsigned int shaderProgramYellow = glCreateProgram();

	glAttachShader(shaderProgramOrange, vertexShader);
	glAttachShader(shaderProgramOrange, fragmentShaderOrange);
	glLinkProgram(shaderProgramOrange);
	glAttachShader(shaderProgramYellow, vertexShader);
	glAttachShader(shaderProgramYellow, fragmentShaderYellow);
	glLinkProgram(shaderProgramYellow);

	// clean up shader objects, no longer needed now that they're linked
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShaderOrange);
	glDeleteShader(fragmentShaderYellow);

//////// VERTEX INPUT ////

	// define some vertices for 2 triangles, without specifying the same vertex twice
	float triangle1[] = {
		// first triangle
		-0.45f, 0.5f, 0.0f,	// top
		-0.8f, -0.5f, 0.0f,	// bottom left
		-0.1f, -0.5f, 0.0f,	// bottom right
	};
	
	float triangle2[] = {
		0.45f, 0.5f, 0.0f,	// top
		0.1f, -0.5f, 0.0f,	// bottom left
		0.8f, -0.5f, 0.0f,	// bottom right
	};

	// declare and generate Vertex Buffer Object, Vertex Array Object, and Element Buffer Object IDs
	unsigned int VAOs[2], VBOs[2];
	glGenVertexArrays(2, VAOs); // we can generate multiple VAOs or buffers at the same time
	glGenBuffers(2, VBOs);

	// first triangle
	glBindVertexArray(VAOs[0]); // bind vertex array object
	glBindBuffer(GL_ARRAY_BUFFER, VBOs[0]); // bind vertex buffer object
	glBufferData(GL_ARRAY_BUFFER, sizeof(triangle1), triangle1, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	
	// second triangle
	glBindVertexArray(VAOs[1]); // bind vertex array object
	glBindBuffer(GL_ARRAY_BUFFER, VBOs[1]); // bind vertex buffer object
	glBufferData(GL_ARRAY_BUFFER, sizeof(triangle2), triangle2, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);


	// uncomment this call to draw in wireframe polygons.
	//glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	// render loop - every iteration is known as a "frame"
	while (!glfwWindowShouldClose(window))
	{
		// input
		processInput(window);

		// rendering commands here
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		// draw our first triangle
		glUseProgram(shaderProgramOrange);
		glBindVertexArray(VAOs[0]); // bind first VAO
		glDrawArrays(GL_TRIANGLES, 0, 3); // draw first triangle

		glUseProgram(shaderProgramYellow);
		glBindVertexArray(VAOs[1]); // bind second VAO, no need to unbind
		glDrawArrays(GL_TRIANGLES, 0, 3); // draw second triangle

		// check and call events and swap the buffers
		glfwPollEvents(); // checking if any events are triggered (like keyboard input or mouse movement)
		glfwSwapBuffers(window); // swaps the color buffer (large 2D buffer of color values for every pixel
									// in GLFW's window, uses the double buffer system
	}

	glDeleteVertexArrays(2, VAOs);
	glDeleteBuffers(2, VBOs);
	glDeleteProgram(shaderProgramOrange);
	glDeleteProgram(shaderProgramYellow);

	glfwTerminate();
	return 0;
}

// adjusting viewport when window is resized by the user
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	//// VIEWPORT ////
	// first two #s set location of lower left corner, second two #s set width and height
	glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE ) == GLFW_PRESS) // if user presses the ESC key
		glfwSetWindowShouldClose(window, true);				// close the window passes in
}