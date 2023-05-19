#include <glad/glad.h>
#include <glfw/glfw3.h>

#include <iostream>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

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


	//// GLAD - function manager for OpenGL ////

	// initializing GLAD to manage function pointers before we call OpenGL functions
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to init GLAD" << std::endl;
		return -1;
	}


	//// READY YOUR ENGINES ////

	// render loop - every iteration is known as a "frame"
	while (!glfwWindowShouldClose(window))
	{
		// input
		processInput(window);

		// rendering commands here
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		// check and call events and swap the buffers
		glfwPollEvents(); // checking if any events are triggered (like keyboard input or mouse movement)
		glfwSwapBuffers(window); // swaps the color buffer (large 2D buffer of color values for every pixel
									// in GLFW's window, uses the double buffer system
	}

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
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) // if user presses the ESC key
		glfwSetWindowShouldClose(window, true);				// close the window passes in
}