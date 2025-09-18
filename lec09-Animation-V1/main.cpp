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
GLuint VAO;
GLuint VBO;

const unsigned int SCR_WIDTH = 1080;
const unsigned int SCR_HEIGHT = 800;

float g_last = glfwGetTime();
float ANGLE_STEP = 45.0;

glm::mat4 modelMatrix = glm::mat4(1.0);
float currentAngle = 0.0;

void init(GLFWwindow* window);
void display(GLFWwindow* window, double currentTime);
float animation(float angle);

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
         0.0,  0.5, 0.0,
        -0.5, -0.5, 0.0,
         0.5, -0.5, 0.0 
    };

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    // vertices - Position
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

}

void display(GLFWwindow* window, double currentTime) {
    glClearColor(0.1, 0.1, 0.1, 1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    Shader triangle("vertexShader.glsl", "fragmentShader.glsl");

    // Triangle
    triangle.use();
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
    triangle.setMat4("uProjMatrix", projMatrix);
    // View
    glm::mat4 viewMatrix = glm::mat4(1.0f);
    glm::vec3 cameraPosition(0.0f, 0.0f, 5.0f); 
    glm::vec3 targetPoint(0.0f, 0.0f, 0.0f);
    glm::vec3 upVector(0.0f, 1.0f, 0.0f);
    viewMatrix = glm::lookAt(
        cameraPosition, //Position of the camera
        targetPoint, // Position where the camera is looking at
        upVector // Normalized up vector, how the camera is oriented. Typically (0, 0, 1)
    );
    triangle.setMat4("uViewMatrix", viewMatrix);
    // Model
    currentAngle = animation(currentAngle);
    modelMatrix = glm::rotate(
        modelMatrix,
        glm::radians(currentAngle),
        glm::vec3(0.0f, 1.0f, 1.0f)
    );
    triangle.setMat4("uModelMatrix", modelMatrix);

    glBindVertexArray(VAO);
    glEnable(GL_DEPTH_TEST);
    glDrawArrays(GL_TRIANGLES, 0, 3);

}

float animation(float angle){
    double now = glfwGetTime();
    float elapsed = now - g_last;
    g_last = now;
    float newAngle = angle + (ANGLE_STEP * elapsed) /1000.0;
    return newAngle;
}