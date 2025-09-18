#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <string>
#include <fstream>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "shader.h"

GLuint renderingProgram;
GLuint renderingProgramLightCube;
GLuint VAO;
GLuint VAOLightCube;
GLuint VBO;

const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 800;

void init(GLFWwindow* window);
void display(GLFWwindow* window, double currentTime);

int main(void)
{
    /* Initialize the library */
    if (!glfwInit()){
        exit(EXIT_FAILURE);
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

    /* Create a windowed mode window and its OpenGL context */
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Light-DAS: Class Shader", NULL, NULL);
    /* Make the window's context current */
    glfwMakeContextCurrent(window);
    if (glewInit() != GLEW_OK){
        exit(EXIT_FAILURE);
    }
    glfwSwapInterval(1);

    init(window);

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window)){
        display(window, glfwGetTime());

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glfwDestroyWindow(window);
    glfwTerminate();
    exit(EXIT_SUCCESS);
}

void init(GLFWwindow* window) {
    float vertices[] = {
        // front
         1.0,  1.0,  1.0, 1.0, 1.0, 1.0, 0.0, 0.0, 1.0, // V0
        -1.0,  1.0,  1.0, 1.0, 1.0, 1.0, 0.0, 0.0, 1.0, // V1
        -1.0, -1.0,  1.0, 1.0, 1.0, 1.0, 0.0, 0.0, 1.0, // V2
         1.0,  1.0,  1.0, 1.0, 1.0, 1.0, 0.0, 0.0, 1.0, // V0
        -1.0, -1.0,  1.0, 1.0, 1.0, 1.0, 0.0, 0.0, 1.0, // V2
         1.0, -1.0,  1.0, 1.0, 1.0, 1.0, 0.0, 0.0, 1.0, // V3

        // right
         1.0,  1.0,  1.0, 1.0, 1.0, 1.0, 1.0, .0, 0.0, // V0
         1.0, -1.0,  1.0, 1.0, 1.0, 1.0, 1.0, .0, 0.0, // V3
         1.0, -1.0, -1.0, 1.0, 1.0, 1.0, 1.0, .0, 0.0, // V4
         1.0,  1.0,  1.0, 1.0, 1.0, 1.0, 1.0, .0, 0.0, // V0
         1.0, -1.0, -1.0, 1.0, 1.0, 1.0, 1.0, .0, 0.0, // V4
         1.0,  1.0, -1.0, 1.0, 1.0, 1.0, 1.0, .0, 0.0, // V5
        
        // up
         1.0,  1.0,  1.0, 1.0, 1.0, 1.0, 0.0, 1.0, 0.0, // V0
         1.0,  1.0, -1.0, 1.0, 1.0, 1.0, 0.0, 1.0, 0.0, // V5
        -1.0,  1.0, -1.0, 1.0, 1.0, 1.0, 0.0, 1.0, 0.0, // V6
         1.0,  1.0,  1.0, 1.0, 1.0, 1.0, 0.0, 1.0, 0.0, // V0
        -1.0,  1.0, -1.0, 1.0, 1.0, 1.0, 0.0, 1.0, 0.0, // V6
        -1.0,  1.0,  1.0, 1.0, 1.0, 1.0, 0.0, 1.0, 0.0, // V1

        // left
        -1.0,  1.0,  1.0, 1.0, 1.0, 1.0, -1.0, 0.0, 0.0, // V1
        -1.0,  1.0, -1.0, 1.0, 1.0, 1.0, -1.0, 0.0, 0.0, // V6
        -1.0, -1.0, -1.0, 1.0, 1.0, 1.0, -1.0, 0.0, 0.0, // V7
        -1.0,  1.0,  1.0, 1.0, 1.0, 1.0, -1.0, 0.0, 0.0, // V1
        -1.0, -1.0, -1.0, 1.0, 1.0, 1.0, -1.0, 0.0, 0.0, // V7
        -1.0, -1.0,  1.0, 1.0, 1.0, 1.0, -1.0, 0.0, 0.0, // V2

        // down
        -1.0, -1.0, -1.0, 1.0, 1.0, 1.0, 0.0, -1.0, 0.0, // V7
         1.0, -1.0, -1.0, 1.0, 1.0, 1.0, 0.0, -1.0, 0.0, // V4
         1.0, -1.0,  1.0, 1.0, 1.0, 1.0, 0.0, -1.0, 0.0, // V3
        -1.0, -1.0, -1.0, 1.0, 1.0, 1.0, 0.0, -1.0, 0.0, // V7
         1.0, -1.0,  1.0, 1.0, 1.0, 1.0, 0.0, -1.0, 0.0, // V3
        -1.0, -1.0,  1.0, 1.0, 1.0, 1.0, 0.0, -1.0, 0.0, // V2

        // back
         1.0, -1.0, -1.0, 1.0, 1.0, 1.0, 0.0, 0.0, -1.0, // V4
        -1.0, -1.0, -1.0, 1.0, 1.0, 1.0, 0.0, 0.0, -1.0, // V7
        -1.0,  1.0, -1.0, 1.0, 1.0, 1.0, 0.0, 0.0, -1.0, // V6
         1.0, -1.0, -1.0, 1.0, 1.0, 1.0, 0.0, 0.0, -1.0, // V4
        -1.0,  1.0, -1.0, 1.0, 1.0, 1.0, 0.0, 0.0, -1.0, // V6
         1.0,  1.0, -1.0, 1.0, 1.0, 1.0, 0.0, 0.0, -1.0  // V5
    };

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    // vertices - Position
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // color
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    // Normal
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    glGenVertexArrays(1, &VAOLightCube);
    glBindVertexArray(VAOLightCube);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    // vertices - Position
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

}

void display(GLFWwindow* window, double currentTime) {
    glClearColor(0.1, 0.1, 0.1, 1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    Shader cube("vertexShader.glsl", "fragmentShader.glsl");
    Shader lightCube("lightCubeVertexShader.glsl", "lightCubeFragmentShader.glsl");

    // Cube
    cube.use();
    // Light
    cube.setVec3("uObjectColor", 1.0f, 0.5f, 0.31f);
    cube.setVec3("uLightColor", 1.0f, 1.0f, 1.0f);
    cube.setVec3("uLightDirection", 1.2f, 1.0f, 2.0f);
    cube.setVec3("uViewPosition", 0.2f, 0.2f, 0.2f);
    cube.setVec3("uAmbientLight", -0.5f, 0.0f, -1.0f);
    // Projection
    float ANGLE = 45.0f;
    float radian = glm::radians(ANGLE);
    float aspect = (float)SCR_WIDTH/(float)SCR_HEIGHT;
    float Znear = 0.1f;
    float Zfar = 100.0f;
    glm::mat4 projMatrix = glm::mat4(1.0);
    projMatrix = glm::perspective(
        radian, // fovy	Specifies the field of view angle in the y direction. Expressed in radians.
        aspect, // Specifies the aspect ratio that determines the field of view in the x direction. The aspect ratio is the ratio of x (width) to y (height).
        Znear, // Specifies the distance from the viewer to the near clipping plane (always positive).
        Zfar // Specifies the distance from the viewer to the far clipping plane (always positive).
    );
    cube.setMat4("uProjMatrix", projMatrix);
    // View
    glm::mat4 viewMatrix = glm::mat4(1.0f);
    glm::vec3 cameraPosition(3.0f, 3.0f, 7.0f); 
    glm::vec3 targetPoint(0.0, 0.0, 0.0);
    glm::vec3 upVector(0.0, 1.0, 0.0);
    viewMatrix = glm::lookAt(
        cameraPosition, //Position of the camera
        targetPoint, // Position where the camera is looking at
        upVector // Normalized up vector, how the camera is oriented. Typically (0, 0, 1)
    );
    cube.setMat4("uViewMatrix", viewMatrix);
    // Model
    glm::mat4 modelMatrix = glm::mat4(1.0);
    cube.setMat4("uModelMatrix", modelMatrix);

    glBindVertexArray(VAO);
    glEnable(GL_DEPTH_TEST);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    // Light Cube
    lightCube.use();
    // Projection
    lightCube.setMat4("uProjMatrixLightCube", projMatrix);
    // View
    lightCube.setMat4("uViewMatrixLightCube", viewMatrix);
    // Model
    modelMatrix = glm::mat4(1.0);
    modelMatrix = glm::translate(modelMatrix, glm::vec3(0.0f, 0.0f, 4.0f));
    modelMatrix = glm::scale(modelMatrix, glm::vec3(0.2f));
    lightCube.setMat4("uModelMatrixLightCube", modelMatrix);

    glBindVertexArray(VAOLightCube);
    glEnable(GL_DEPTH_TEST);
    glDrawArrays(GL_TRIANGLES, 0, 36);

}