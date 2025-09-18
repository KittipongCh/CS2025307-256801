#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <string>
#include <fstream>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "shader.h"

GLuint VAO;
GLuint VBO;

const unsigned int SCR_WIDTH = 1080;
const unsigned int SCR_HEIGHT = 800;

glm::mat4 modelMatrix = glm::mat4(1.0);
glm::mat4 viewMatrix = glm::mat4(1.0f);
glm::mat4 projMatrix = glm::mat4(1.0);

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
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Animation 3D, Version 2.0", NULL, NULL);
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
    float verticesPyramid[] = {
        // Front face
         0.0,  1.0,  0.0, 1.0, 0.0, 0.0, 1.0,
        -1.0, -1.0,  1.0, 0.0, 1.0, 0.0, 1.0,
         1.0, -1.0,  1.0, 0.0, 0.0, 1.0, 1.0,
        // Right face
         0.0,  1.0,  0.0, 1.0, 0.0, 0.0, 1.0,
         1.0, -1.0,  1.0, 0.0, 0.0, 1.0, 1.0,
         1.0, -1.0, -1.0, 0.0, 1.0, 0.0, 1.0,
        // Back face
         0.0,  1.0,  0.0, 1.0, 0.0, 0.0, 1.0,
         1.0, -1.0, -1.0, 0.0, 1.0, 0.0, 1.0,
        -1.0, -1.0, -1.0, 0.0, 0.0, 1.0, 1.0,
        // Left face
         0.0,  1.0,  0.0, 1.0, 0.0, 0.0, 1.0,
        -1.0, -1.0, -1.0, 0.0, 0.0, 1.0, 1.0,
        -1.0, -1.0,  1.0, 0.0, 1.0, 0.0, 1.0
    };

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(verticesPyramid), verticesPyramid, GL_STATIC_DRAW);
    // vertices - Position
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // vertices - Color
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

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
    projMatrix = glm::perspective(
        radian, // fovy	Specifies the field of view angle in the y direction. Expressed in radians.
        aspect, // Specifies the aspect ratio that determines the field of view in the x direction. The aspect ratio is the ratio of x (width) to y (height).
        Znear, // Specifies the distance from the viewer to the near clipping plane (always positive).
        Zfar // Specifies the distance from the viewer to the far clipping plane (always positive).
    );
    triangle.setMat4("uProjMatrix", projMatrix);
    // View
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
    for(int i=0; i < 128; i++){
        float tf = currentTime + i; //tf = time factor
        glm::mat4 model_t = glm::translate(
            glm::mat4(1.0f),
            glm::vec3(sin(0.35f * tf)*2.0f, cos(0.52f * tf)*2.0f, sin(0.7f * tf)*2.0f)
        );
        glm::mat4 model_r = glm::rotate(glm::mat4(1.0f), 1.75f*(float)tf, glm::vec3(0.0f, 1.0f, 0.0f));
        model_r = glm::rotate(model_r, 1.75f*(float)tf, glm::vec3(1.0f, 0.0f, 0.0f));
        model_r = glm::rotate(model_r, 1.75f*(float)tf, glm::vec3(0.0f, 0.0f, 1.0f));
        modelMatrix = model_t * model_r;
        modelMatrix = glm::scale(modelMatrix, glm::vec3(0.15f, 0.15f, 0.15f));
        triangle.setMat4("uModelMatrix", modelMatrix);
        
        // render the triangle
        glBindVertexArray(VAO);
        glEnable(GL_DEPTH_TEST);
        glDrawArrays(GL_TRIANGLES, 0, 12);
    }
}