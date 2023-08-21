#include <glad/glad.h>
#include <glfw/glfw3.h>
#include <stb_image.h> // image loading library

#include <shader.h>

#include <iostream>

//////// TEXTURES ////

// Textures coordinates range from (0,0) to (1,1)
// Uses the s,t,r axis equivalent to x,y,z
// 
// Sampling - retrieving color from a texture using texture coords

//////// TEXTURE WRAPPING ////

// If you specify coordinates outside this range, you get texture wrapping
// Options:
//      GL_REPEAT (default) - repeats texture
//      GL_MIRRORED_REPEAT - same but mirrors the image w/ each repeat
//      GL_CLAMP_TO_EDGE - clamps coordinates, resulting in stretched edge pattern
//      GL_CLAMP_TO_BORDER - coordinates outside the range are a user-specified solid color

//////// TEXTURE FILTERING ////

// Texture coords are float values, not pixel dependent
// So, OpenGL has to figure out which texture pixel (AKA texel) to map the texture coord to
// Options:
//      GL_NEAREST (default) - AKA nearest neighbor or point filtering, selects the texel whose center is closest, rigid result
//      GL_LINEAR - AKA (bi)linear filtering, takes interpolated value from the neighboring texels, smoother result

//////// MIPMAPS ////

// Rendering high resolution textures for something small on the screen (maybe far away from the player) is wasteful and can produce visual artifacts
// Mipmap - collection of textures where each subsequent texture is 2x as small as the previous one

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


    Shader ourShader("src/Getting Started/Textures/texture.vert", "src/Getting Started/Textures/texture.frag");

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


//////// LOADING AND CREATING TEXTURES ////

    // stb_image.h - image loading library included

    //////// GENERATING A TEXTURE ////
    unsigned int texture1, texture2;

    glGenTextures(1, &texture1);
    glBindTexture(GL_TEXTURE_2D, texture1);
    // set the texture wrapping/filtering options
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // stbi_load(string imageLocation, int* width, int* height, int* numberofColorChannels, int desiredChannels)
    int width, height, nrChannels;
    stbi_set_flip_vertically_on_load(true);
    unsigned char* data = stbi_load("src/Getting Started/Textures/container.jpg", &width, &height, &nrChannels, 0);
    if (data)
    {
        // glTexImage2D() parameters:
        //      1 - texture target
        //      2 - mipmap level, default level 0
        //      3 - format of texture, in this case RGB color data
        //      4 & 5 - width & height of the texture
        //      6 - always 0
        //      7 & 8 - format & datatype of source image
        //      9 - actual image data
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }
    // after we're done generating the texture, free the image memory
    stbi_image_free(data);

    // loading and creating second texture
    glGenTextures(1, &texture2);
    glBindTexture(GL_TEXTURE_2D, texture2);
    // set the texture wrapping/filtering options
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    data = stbi_load("src/Getting Started/Textures/awesomeface.png", &width, &height, &nrChannels, 0);
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

    // every shader and rendering call after this line will use this program (and the attached shaders)
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

        ourShader.use();
        ourShader.setFloat("mixValue", mixValue);

        glBindVertexArray(VAO);
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
