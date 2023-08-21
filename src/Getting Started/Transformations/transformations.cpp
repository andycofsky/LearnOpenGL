#include <glad/glad.h>
#include <glfw/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <stb_image.h> // image loading library

#include <shader.h>

#include <iostream>


//////// TRANSFORMATIONS ////

//////// VECTORS ////
//  - vectors are directions
//  - vectors have a direction and a magnitude in any dimension
//  - vectors can describe directions AND positions in 2D and 3D space
//  
//// Scalar vector operations
//      - a scalar is a single digit.
//      - Add/sub/mult/div a vector with a scalar by simply add/sub/mult/div each element of the vector by the scalar
//      - ex. (1 2 3) + x = (1 2 3) + (x x x) = (1+x 2+x 3+x)
// 
//// Vector negation
//      - reverse the signs of each element of the vector (- to + or + to -)
// 
//// Addition and subtraction
//      - component-wise addition/subtraction, meaning each component of one vector is added/subbed to the same component of another vector
//      - ex. (1 2 3) + (4 5 6) = (5 7 9)
// 
//// Length
//      - denoted as ||v||
//      - we can use Pythagoras theorem
//      - ex. ||v|| = sqrt(x^2 + y^2)
// 
//      - unit vector - length of exactly 1, found by dividing a vector by its length
//      - this is known as normalization
//      - unit vectors are easier to work with if we only care about direction
// 
//// Vector-vector multiplication
//      - Dot product - used to find whether two vectors are orthogonal (right-angle) or parallel to each other using the angle between them
//      - should use normalized vectors
//          - Dot product is component-wise multiplication, add the results together
//          - ex. (0.6 -0.8 0) * (0 1 0) = (0.6 * 0) + (-0.8 * 1) + (0 * 0) = -0.8
//          - then use inverse cosine to find the angle between them cos^-1(-0.8) = 143.1 degrees
// 
//      - Cross product - only defined in 3D space, takes 2 non-parallel vectors and produces a 3rd vector orthogonal to both input vectors

//////// MATRICES ////
//  - a rectangular array of numbers, symbols, and/or expressions
//  - each item is called an element
//  - elements are indexed by (i, j) where i is the row and j is the column, both i and j start at 1 (not 0 indexed)
//
//// Addition and subtraction
//      - only matrices of the same dimensions can be added/subbed (a 3x2 matrix and 2x3 matrix cannot be added/subbed)
//      - ex.   |   1   2   |   +   |   5   6   |   =   |   1+5 2+6 |   =   |   6   8   |
//              |   3   4   |       |   7   8   |       |   3+7 4+8 |   =   |   10  12  |
//
//// Matrix-scalar products
//      - we can multiply a matrix by a scalar, which just multiplies each element of the matrix by the scalar
// 
//// Matrix-matrix multiplication
//              - this is where things get complicated
//      Restrictions:
//      1. You can only multiply two matrices if # of colums in left-hand matrix = # of rows in right-hand matrix
//      2. Matrix multiplication is NOT commutative, that is A * B != B * A
//
//


void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

float mixValue = 0.2f; // use up and down arrow keys to adjust the mix value between textures

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


    Shader ourShader("src/Getting Started/Transformations/transformations.vert", "src/Getting Started/Transformations/transformations.frag");

    // define some vertices for a triangle
    float vertices[] = {
        // positions          // colors           // texture coords
         0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // top right
         0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // bottom right
        -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // bottom left
        -0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f    // top left 
    };
    unsigned int indices[] = {
        0, 1, 3, // first triangle
        1, 2, 3  // second triangle
    };
    unsigned int VBO, VAO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO); // "select" this buffer of type GL_ARRAY_BUFFER
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*) 0);
    glEnableVertexAttribArray(0); // enable vertex attribute index 0
    // color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1); // enable vertex attribute index 1
    // texture attribute
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2); // enable vertex attr index 2

    //////// GENERATING A TEXTURE ////
    unsigned int texture1, texture2;

    glGenTextures(1, &texture1);
    glBindTexture(GL_TEXTURE_2D, texture1);
    // set the texture wrapping/filtering options
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    int width, height, nrChannels;
    stbi_set_flip_vertically_on_load(true);
    unsigned char* data = stbi_load("assets/container.jpg", &width, &height, &nrChannels, 0);
    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);

    // loading and creating second texture
    glGenTextures(1, &texture2);
    glBindTexture(GL_TEXTURE_2D, texture2);
    // set the texture wrapping/filtering options
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    data = stbi_load("assets/awesomeface.png", &width, &height, &nrChannels, 0);
    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);

    ourShader.use();
    ourShader.setInt("texture1", 0);
    ourShader.setInt("texture2", 1);

    // render loop - every iteration is known as a "frame"
    while (!glfwWindowShouldClose(window))
    {
        // input
        processInput(window);

        // rendering commands here
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glActiveTexture(GL_TEXTURE0); // activate the texture unit first before binding texture
        // bind texture before calling glDrawElements to assign the texture to the frag shader's sampler
        glBindTexture(GL_TEXTURE_2D, texture1);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, texture2);

        // create transformations
        glm::vec4 vec(1.0f, 0.0f, 0.0f, 1.0f);
        glm::mat4 trans = glm::mat4(1.0f);
        glm::mat4 trans2 = glm::mat4(1.0f);
        //trans = glm::translate(trans, glm::vec3(1.0f, 1.0f, 0.0f));
        //vec = trans * vec;
        //std::cout << vec.x << vec.y << vec.z << std::endl;
        //trans = glm::scale(trans, glm::vec3(0.5, 0.5, 0.5));
        trans = glm::translate(trans, glm::vec3(0.5f, -0.5f, 0.0f));
        trans = glm::rotate(trans, 1.0f * (float)glfwGetTime(), glm::vec3(0.0f, 0.0f, 1.0f));

        ourShader.use();
        ourShader.setFloat("mixValue", mixValue);
        unsigned int transformLoc = glGetUniformLocation(ourShader.ID, "transform");
        glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(trans));

        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        trans2 = glm::translate(trans2, glm::vec3(-0.5f, 0.5f, 0.0f));
        trans2 = glm::scale(trans2, glm::vec3(sin(1.0f * (float)glfwGetTime()), sin(1.0f * (float)glfwGetTime()), 1.0f));
        glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(trans2));

        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        // check and call events and swap the buffers
        glfwPollEvents(); // checking if any events are triggered (like keyboard input or mouse movement)
        glfwSwapBuffers(window); // swaps the color buffer (large 2D buffer of color values for every pixel
                                    // in GLFW's window, uses the double buffer system
    }

    // de-allocate all resources
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);

    glDeleteProgram(ourShader.ID);

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
        glfwSetWindowShouldClose(window, true);				// close the window passed in

    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS && mixValue < 1.0f)
        mixValue += 0.005f;
    
    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS && mixValue > 0.0f)
        mixValue -= 0.005f;
}
