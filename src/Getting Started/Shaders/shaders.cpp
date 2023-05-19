#include <glad/glad.h>
#include <glfw/glfw3.h>

#include <iostream>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

//////// GLSL ////

// typical shader structure

//#version version_number
//in type in_variable_name;
//in type in_variable_name;
//
//out type out_variable_name;
//
//uniform type uniform_name;
//
//void main()
//{
//	// process input(s) and do some weird graphics stuff
//	...
//	// output processed stuff to output variable
//	out_variable_name = weird_stuff_we_processed;
//}

//// GLSL Types //

// basic types - int, float, double, uint, and bool
// container types - vectors and matrices

//// Vectors

// where n represents the # of components...
//  vecn - default vector of n floats - most commonly used
//  bvecn - vector of bools
//  ivecn - vector of ints
//  uvecn - vector of unsigned ints
//  dvecn - vector of doubles

// swizzling - using .x, .y, .z, .w, or any combination of them to access/set values of another vector
// ex. vec2 someVec;
//      vec4 differentVec = someVec.xyxx;
//      vec3 anotherVec = differentVec.zyw;
//      vec4 otherVec = someVec.xxxx + anotherVec.yxzy; // you can math values of vectors

// you can also pass vectors as arguments to vector construction calls
// ex. vec2 vect = vec2(0.5, 0.7);
//      vec4 result = vec4(vect, 0.0, 0.0);
//      vec4 otherResult = vec4(result.xyz, 1.0);

//////// INS AND OUTS ////

// in and out keywords - specifying shader inputs and outputs

// vertex shader SHOULD always receive some sort of input, straight from the vertex data
// layout (location = 0) defines how the vertex data is setup with vertex attribs

// frag shader requires vec4 color output variable, which will be the final output color

// If we want to send data from one shader to another, we need to define the same variable type and name for both
// check the shader code below

const char* vertexShaderSource =
    "#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n"     // the position var has attrib position 0
    "layout (location = 1) in vec3 aColor;\n"   // the color variable has attrib position 1
    "\n"
    "out vec3 ourColor;\n" // output to frag shader
    "\n"
    "void main()\n"
    "{\n"
    "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
    "   ourColor = aColor;\n"   // set ourColor to input color we get from the vertex data
    "}\n\0";
//const char* fragmentShaderSource =
//    "#version 330 core\n"
//    "out vec4 FragColor;\n"
//    "\n"
//    "in vec4 vertexColor;\n" // input from vertex shader
//    "\n"
//    "void main()\n"
//    "{\n"
//    "	FragColor = vertexColor;\n"
//    "}\n\0";

//////// UNIFORMS ////

// uniforms allow us to pass data from the application
// uniforms are global in the scope of any one shader program
// uniforms will maintain their value until they're reset or updated (static)

const char* fragmentShaderSource =
    "#version 330 core\n"
    "out vec4 FragColor;\n"
    "in vec3 ourColor;\n"
    "\n"
    //"uniform vec4 ourColor;\n" // we'll set this variable in the OpenGL code
    "\n"
    "void main()\n"
    "{\n"
    //"	FragColor = ourColor;\n"
    "	FragColor = vec4(ourColor, 1.0);\n"
    "}\n\0";


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

//////// MORE ATTRIBUTES ////

    // define some vertices for a triangle
    float vertices[] = {
        // positions        // colors
        0.5f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f,   // bottom right
        -0.5f, -0.5f, 0.0f,	0.0f, 1.0f, 0.0f,   // bottom left
        0.0f, 0.5f, 0.0f,	0.0f, 0.0f, 1.0f    // top
    };

    unsigned int VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO); // "select" this buffer of type GL_ARRAY_BUFFER

    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*) 0);
    glEnableVertexAttribArray(0);
    // color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

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

        glUseProgram(shaderProgram); // activate program

//////// UNIFORMS ////

        // let's add data to the uniform in the fragment shader
        //float timeValue = glfwGetTime();
        //float greenValue = (sin(timeValue) / 2.0f) + 0.5f;
        //int vertexColorLocation = glGetUniformLocation(shaderProgram, "ourColor"); // query for location of ourColor uniform
        //glUniform4f(vertexColorLocation, 0.0f, greenValue, 0.0f, 1.0f); // setting the uniform value

////////////

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