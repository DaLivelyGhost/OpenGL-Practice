#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

const char* vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"void main()\n"
"{\n"
"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
"}\0";

const char* fragmentShaderSource = "#version 330 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);"
"}\0";

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
    //------------------------------------------------------------------------------
    //----Vertex Shader-------------------------------------------------------------
    
    //establish vertex shader
    unsigned int vertexShader;
    vertexShader = glCreateShader(GL_VERTEX_SHADER);

    //bind vertex shader to one defined previously and compile
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);

    //----error check the vertex shader
        int  success;
        char infoLog[512];
        glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);

        if (!success)
        {
            glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
            std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
        }

    //------------------------------------------------------------------------------
    //----Fragment Shader-----------------------------------------------------------

    //establish fragment shader
    unsigned int fragmentShader;
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    
    //bind fragment shader to one defined previously and compile
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);

    //----error check the fragment shader
    int  success2;
    char infoLog2[512];
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success2);

    if (!success2)
    {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog2);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog2 << std::endl;
    }
    //------------------------------------------------------------------------------

    //----Link the shaders to the shader program------------------------------------

    unsigned int shaderProgram;
    shaderProgram = glCreateProgram();

    //attach shaders to the program
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    //----error check the shader program
    int successProgram;
    char infoLogProgram[512];
    glGetProgramiv(shaderProgram, GL_COMPILE_STATUS, & successProgram);

    if (!successProgram) 
    {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLogProgram);
        std::cout << "ERROR::SHADER::PROGRAM::COMPILATION_FAILED\n" << infoLogProgram << std::endl;
    }

    //----clean up excess shader objects (they are stored in the shader program now)
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    //------------------------------------------------------------------------------

    //----Create the rectangle's vertices
    float vertices[] = {
      0.5f,  0.5f, 0.0f,  // top right
      0.5f, -0.5f, 0.0f,  // bottom right
     -0.5f, -0.5f, 0.0f,  // bottom left
     -0.5f,  0.5f, 0.0f   // top left 
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

    //----Define the vertex attribute pointers
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);



    //----MAIN RENDER LOOP
    while (!glfwWindowShouldClose(window))
    {
        processInput(window);

        glUseProgram(shaderProgram);
        glBindVertexArray(VAO);

        //----Define the color of the viewport when cleared
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        //----Clear the viewport
        glClear(GL_COLOR_BUFFER_BIT);

        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);

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