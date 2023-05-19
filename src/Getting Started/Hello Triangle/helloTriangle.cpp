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
const char* fragmentShaderSource = "#version 330 core\n"
	"out vec4 FragColor;\n"
	"void main()\n"
	"{\n"
	"	FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
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


//////// COMPILING A SHADER ////

	// setup vertex shader object
	unsigned int vertexShader; // object id
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	// attach shader src code (defined at the top of the file ^^^) to shader object and compile
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);
	// check for shader compilation errors
	int success; // indicator
	char infoLog[512]; // to store error message
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

//////// FRAGMENT SHADER ////

	// very similar thing as the vertex shader object setup
	unsigned int fragmentShader; // object id
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);
	// check for shader compilation errors
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::FRAGEMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

//////// SHADER PROGRAM //// - linked version of multiple shaders
	
	// create program object
	unsigned int shaderProgram;
	shaderProgram = glCreateProgram();
	// attach each of the shaders to the program and link them
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);
	// error checking
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(shaderProgram, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
	}

	// clean up shader objects, no longer needed now that they're linked
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

//////// VERTEX ARRAY OBJECT ////
	// can be bound just like a VBO, any vertex attr calls that follow will be stored in the VAO
	// Note - Core OpenGL actually requires that we use a VAO
	// stores the following:
	// - calls to glEnableVertexAttribArray or glDisableVertexAttribArray
	// - configurations via glVertexAttribPointer
	// - VBOs associated with vertex attribs by calls to glVertexAttribPointer
	unsigned int VAO;
	glGenVertexArrays(1, &VAO);
	// bind vertex array object
	glBindVertexArray(VAO);

//////// VERTEX INPUT ////

	// define some vertices for a triangle
	float vertices[] = {
		-0.5f, -0.5f, 0.0f,	// bottom left
		0.5f, -0.5f, 0.0f,	// bottom right
		0.0f, 0.5f, 0.0f,	// top middle
	};
	// Normalized Device Coordinates (NDC)
	// coord values beyond the bounds of [-1.0, 1.0] will be discarded/clipped by OpenGL

	// send this vertex data to the vertex shader
	// set aside memory for the data called a vertex buffer object (VBO)
	unsigned int VBO;
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO); // "select" this buffer of type GL_ARRAY_BUFFER
	// copy the vertex data into the buffer's memory
	// args | 1 - buffer type  2 - size of data in bytes  3 - actual data we want to send
	//			4 - specifies how the graphics card should manage the data, comes in 3 forms:
	//			GL_STREAM_DRAW - data is set only once and useda few times (at most) by the GPU
	//			GL_STATIC_DRAW - data is set only once and used many times
	//			GL_DYNAMIC_DRAW - data is changed a lot and used many times
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

//////// LINKING VERTEX ATTRIBUTES ////

	// tell OpenGL how to interpret the vertex data (per attribute)
	// args | 1 - vertex attr index  2 - vertex attr size (vec3 in this case so we pass in 3)
	//			3 - data type  4 - bool, want the data to be normalized?
	//			5 - "stride" - space between consecutive vertex attr
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*) 0);
	// enable vertex arribs
	glEnableVertexAttribArray(0);

	// note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
	// VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
	glBindVertexArray(0);

	// render loop - every iteration is known as a "frame"
	while (!glfwWindowShouldClose(window))
	{
		// input
		processInput(window);

		// rendering commands here
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		// draw our first triangle
		glUseProgram(shaderProgram); // activate program
		// every shader and rendering call after this line will use this program (and the attached shaders)
		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 3);

		// check and call events and swap the buffers
		glfwPollEvents(); // checking if any events are triggered (like keyboard input or mouse movement)
		glfwSwapBuffers(window); // swaps the color buffer (large 2D buffer of color values for every pixel
									// in GLFW's window, uses the double buffer system
	}

	// de-allocate all resources
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteProgram(shaderProgram);

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