#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);

const char *vertexShaderSource = "#version 330 core\n"
"layout(location = 0) in vec3 aPos;\n"
"void main()\n"
"{\n"
"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
"}\0";
const char *fragmentShaderSource = "#version 330 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"   FragColor = vec4(0.0f, 0.0f, 0.5f, 1.0f);\n"
"}\0";

int main()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    //now we create a window object

    GLFWwindow* window = glfwCreateWindow(2545, 1550, "LearnOpenGL", NULL, NULL);
    if (window == NULL) 
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);


    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) //the address of where the "window" is located is sent to the gladGLLoader
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    
    //initialization of Vertex Shader
    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    //attach shader source code to shader object
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    //compile shader
    glCompileShader(vertexShader);
    //check for shader compile errors
    int success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" <<infoLog << std::endl;
    }
    
    //initialization of Fragment Shader
    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    //attach shader source code to shader object
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    //compile shader
    glCompileShader(fragmentShader);
    //check for shader compile errors
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if(!success)
    {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cout << "ERROR:SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    //link shaders by initializing our Shader Program
    unsigned int shaderProgram = glCreateProgram();
    //attach our shaders to the shader program
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    //link our attached shaders within the shader program
    glLinkProgram(shaderProgram);
    //check for linking errors
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }

    //remember to delete shader objects after linking them
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);


    //position of our rectangle
    /*float rectVertices[] = {
        //clockwise starting from top right
        0.5f, 0.5f, 0.0f,
        0.5f, -0.5f, 0.0f,
        -0.5f, -0.5f, 0.0f,
        -0.5f, 0.5f, 0.0
    };*/
    float twoTriangles[] = {
        0.75f, 0.75f, 0.0f,
        0.75f, -0.75f, 0.0f,
        0.0f, -0.75f, 0.0f,
        0.0f, -0.75f, 0.0f,
        -0.75f, -0.75f, 0.0f,
        -0.75f, 0.75f, 0.0f
    };

    //initialization of Vertex Buffer Object and Vertex Array Object
    unsigned int VBO, EBO, VAO;
    //generate our VAO
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    //bind VAO first befor binding VBOs
    glBindVertexArray(VAO);
    //bind buffer to its type
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(twoTriangles), twoTriangles, GL_STATIC_DRAW);
    //bind buffer to its type

    //link vertex attribute pointers
    //NOTE: Trouble understanding this section
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    //we can unbind the VBOs buffer because it is now registered with glVertexAttribPointer
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    // You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
    // VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
    glBindVertexArray(0);
    

    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    //This section not necessary but displays the width and height of the window in the output
    /*
    int width, height;
    glfwGetFramebufferSize(window, &width, &height);
    std::cout << "Framebuffer size: " << width << "x" << height << std::endl;
    */

    

    while(!glfwWindowShouldClose(window))
    {
        processInput(window);

        //rendering here
        glClearColor(1.0f, 0.5f, 0.5f, 1.0f);
        //glClearColor(1.0f, 0.0f, 1.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        //draw triangle
        glUseProgram(shaderProgram);
        // with only one VAO, we don't have to bind it every time. Doing so for consistency
        glBindVertexArray(VAO); 
        glDrawArrays(GL_TRIANGLES, 0, 6);
        glBindVertexArray(0); //currently no need to unbind it every time

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    //OPTIONAL: De-allocate all resources
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteProgram(shaderProgram);

    glfwTerminate();
    return 0;

}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void processInput(GLFWwindow *window)
{
    if(glfwGetKey(window,  GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}


