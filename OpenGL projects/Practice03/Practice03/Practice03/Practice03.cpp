#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include "Shader.h"
#include "stb_image.h"

//----input forward declaration
void processInput(GLFWwindow* window);

int main() {

    //----initialize glfw library and set contexts
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    //----create window object
    GLFWwindow* window = glfwCreateWindow(800, 600, "OpenGL Practice 01", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    //----initialize GLAD
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    //----Set Up the Viewport-------------------------------------------------------
        //----create the viewport. Viewport exists within the window.
    glViewport(0, 0, 800, 600);

    //----define callback function that resizes viewport when window resizes.
    void framebuffer_size_callback(GLFWwindow * window, int width, int height);

    //----register viewport resize callback
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
 
    Shader practice03Shader("_vertexShader.vs", "_fragmentShader.fs");

    //----Create the rectangle's vertices
    float vertices[] = {
        // positions          // colors           // texture coords
         0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // top right
         0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // bottom right
        -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // bottom left
        -0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f    // top left 
    };
    unsigned int indices[] = {  // note that we start from 0!
        0, 1, 3,   // first triangle
        1, 2, 3    // second triangle
    };




    unsigned int VBO, VAO, EBO;

    //----Create the vertex buffer object
    glGenBuffers(1, &VBO);
    //----Create the vertex array to hold buffers
    glGenVertexArrays(1, &VAO);
    //----Create the element buffer object
    glGenBuffers(1, &EBO);


    //----Bind the vertex array object first, then bind and set vertex buffers, then configure vertex attributes
    glBindVertexArray(VAO);

    //----copy our vertices array in a vertex buffer for OpenGL to use
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    //----copy our index array in an element buffer for OpenGL to use
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    //----Define the vertex position
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    //----Define the vertex color
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    //----Define the vertex texture position
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    
    
    unsigned int texture1, texture2;

    //--------TEXTURE 1-----------------------------------------
    //----Create the texture object
    glGenTextures(1, &texture1);

    glBindTexture(GL_TEXTURE_2D, texture1);

    //----Set the texture coordinate behavior
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);

    //----Set texture scaling behavior
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    //----Load the image for the texture
    int width, height, nrChannels;

    stbi_set_flip_vertically_on_load(true);
    unsigned char* data = stbi_load("incoming.jpg", &width, &height, &nrChannels, 0);
    
    if (data) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else {
        std::cout << "Failed to load texture 1" << std::endl;
    }


    //--------TEXTURE 2-----------------------------------------
    //----Create the texture object
    glGenTextures(1, &texture2);

    glBindTexture(GL_TEXTURE_2D, texture2);

    //----Set the texture coordinate behavior
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    //----Set texture scaling behavior
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    data = stbi_load("comfort.PNG", &width, &height, &nrChannels, 0);

    if (data) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else {
        std::cout << "Failed to load texture 2" << std::endl;
    }

    //----free the memory held by the image
    stbi_image_free(data);


    practice03Shader.use();
    glUniform1i(glGetUniformLocation(practice03Shader.ID, "texture01"), 0); //setting the texture manually
    practice03Shader.setInt("texture02", 1); //or doing it with the shader class




    //----MAIN RENDER LOOP
    while (!glfwWindowShouldClose(window))
    {
        processInput(window);

        //----Define the color of the viewport when cleared
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        //----Clear the viewport
        glClear(GL_COLOR_BUFFER_BIT);

        glActiveTexture(GL_TEXTURE0); //activate the texture unit first before binding
        glBindTexture(GL_TEXTURE_2D, texture1);

        glActiveTexture(GL_TEXTURE1); //activate the texture unit first before binding
        glBindTexture(GL_TEXTURE_2D, texture2);
       
        //----Transformation
        glm::mat4 trans = glm::mat4(1.0f); //identity matrix
        trans = glm::rotate(trans, cos((float)glfwGetTime()), glm::vec3(0.0f, 0.0f, 1.0f));
        trans = glm::scale(trans, glm::vec3((sin((float)glfwGetTime()) / 2) + 1, (sin((float)glfwGetTime()) / 2) + 1, 1.0f));

        unsigned int transLoc = glGetUniformLocation(practice03Shader.ID, "transform"); //get position of uniform matrix4 in vertex shader
        glUniformMatrix4fv(transLoc, 1, GL_FALSE, glm::value_ptr(trans)); //bind the translation matrix to the uniform

        practice03Shader.use();
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);


        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }


    //exit
    glfwTerminate();
    return 0;
}






void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}
void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}