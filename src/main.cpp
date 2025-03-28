#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "shaders.h"
#include <vector>
#include <thread>
#include <chrono>

using namespace std;

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

void processInput(GLFWwindow *window) {
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

vector<unsigned int> triangleSetup() {
    // Initialize buffer object, array object and element object
    unsigned int VBO;
    glGenBuffers(1, &VBO);
    unsigned int VAO;
    glGenVertexArrays(1, &VAO);
    unsigned int EBO;
    glGenBuffers(1, &EBO);

    // Intialize triangle vertices
    float vertices[] = {
        0.6f, -0.2f, 0.0f,
        0.8f, -0.2f, 0.0f,
        0.7f,  0.5f, 0.0f
    };

    // Initialization code (done once unless your object frequently changes)
    // bind VAO
    glBindVertexArray(VAO);
    // copy our vertices array in a buffer for OpenGL to use
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    // then set our vertex attributes pointers
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    return {VAO, VBO, EBO};
}

vector<unsigned int> rectangleSetup(float x, float y, float z, float offsetX, float offsetY, float offsetZ) {
    // Initialize buffer object, array object and element object
    unsigned int VBO;
    glGenBuffers(1, &VBO);
    unsigned int VAO;
    glGenVertexArrays(1, &VAO);
    unsigned int EBO;
    glGenBuffers(1, &EBO);

    float vertices[] = {
        x/2 + offsetX,  y/2 + offsetY, z/2 + offsetZ,  // top right
        x/2 + offsetX, -y/2 + offsetY, z/2 + offsetZ,  // bottom right
       -x/2 + offsetX, -y/2 + offsetY, z/2 + offsetZ,  // bottom left
       -x/2 + offsetX,  y/2 + offsetY, z/2 + offsetZ  // top left 
    };
    unsigned int indices[] = {  // note that we start from 0!
        0, 1, 3,   // first triangle
        1, 2, 3    // second triangle
    };

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW); 

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0); 

    return {VAO, VBO, EBO};
}

vector<unsigned int> circleSetup() {
    return {};
}

void createShape(GLFWwindow* window, vector<unsigned int> shapeData) {
    unsigned int VAO = shapeData[0];

    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);    
}

void delShape(vector<unsigned int> shapeData) {
    glDeleteVertexArrays(1, &shapeData[0]);
    glDeleteBuffers(1, &shapeData[1]);
    glDeleteBuffers(1, &shapeData[2]);
}

int main()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    int WIDTH = 800;
    int HEIGHT = 600;

    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Ping Pong", NULL, NULL);
    if (window == NULL) {
        cout << "Failed to initialize window" << endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);


    // Initializing vertex shaders
    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);

    // Initializing fragment shaders
    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);

    // Initializing main shader
    unsigned int shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    // START TO BUILD DATA TO RENDER

    vector<unsigned int> p1shapeData = rectangleSetup(0.05, 0.6, 0, -1 + 0.1, 0, 0);
    vector<unsigned int> p2shapeData = rectangleSetup(0.05, 0.6, 0, 1 - 0.1, 0, 0);

    // END OF RENDER DATA


    while (!glfwWindowShouldClose(window)) {
        processInput(window);
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(shaderProgram);

        //glBindVertexArray(VAO_tri);
        //glDrawArrays(GL_TRIANGLES, 0, 3);

        createShape(window, p1shapeData);
        createShape(window, p2shapeData);

        // swap the two buffers, this should be here not in any method
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader); 
    glfwTerminate();
    return 0;
}