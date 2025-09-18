#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <string>
#include <fstream>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "shader.h"

GLuint VAO_Cube;
GLuint VBO_Cube;
GLuint VAO_Pyramid;
GLuint VBO_Pyramid;

const unsigned int SCR_WIDTH = 1080;
const unsigned int SCR_HEIGHT = 800;

glm::mat4 modelMatrix = glm::mat4(1.0);
glm::mat4 viewMatrix = glm::mat4(1.0f);
glm::mat4 projMatrix = glm::mat4(1.0);
glm::mat4 vMat = glm::mat4(1.0f);

std::stack<glm::mat4> mvStack;

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
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Animation 3D, Solar System", NULL, NULL);
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
    float verticesCube[] = {
        // front
         1.0,  1.0,  1.0, 1.0, 1.0, 1.0, 1.0, // V0-White
        -1.0,  1.0,  1.0, 1.0, 0.0, 1.0, 1.0, // V1-Magenta
        -1.0, -1.0,  1.0, 1.0, 0.0, 0.0, 1.0, // V2-Red
         1.0,  1.0,  1.0, 1.0, 1.0, 1.0, 1.0, // V0-White
        -1.0, -1.0,  1.0, 1.0, 0.0, 0.0, 1.0, // V2-Red
         1.0, -1.0,  1.0, 1.0, 1.0, 0.0, 1.0, // V3-Yellow

        // right
         1.0,  1.0,  1.0, 1.0, 1.0, 1.0, 1.0, // V0-White
         1.0, -1.0,  1.0, 1.0, 1.0, 0.0, 1.0, // V3-Yellow
         1.0, -1.0, -1.0, 0.0, 1.0, 0.0, 1.0, // V4-Green
         1.0,  1.0,  1.0, 1.0, 1.0, 1.0, 1.0, // V0-White
         1.0, -1.0, -1.0, 0.0, 1.0, 0.0, 1.0, // V4-Green
         1.0,  1.0, -1.0, 0.0, 1.0, 1.0, 1.0, // V5-cyan
        
        // up
         1.0,  1.0,  1.0, 1.0, 1.0, 1.0, 1.0, // V0-White
         1.0,  1.0, -1.0, 0.0, 1.0, 1.0, 1.0, // V5-cyan
        -1.0,  1.0, -1.0, 0.0, 0.0, 1.0, 1.0, // V6-blue
         1.0,  1.0,  1.0, 1.0, 1.0, 1.0, 1.0, // V0-White
        -1.0,  1.0, -1.0, 0.0, 0.0, 1.0, 1.0, // V6-blue
        -1.0,  1.0,  1.0, 1.0, 0.0, 1.0, 1.0, // V1-Magenta

        // left
        -1.0,  1.0,  1.0, 1.0, 0.0, 1.0, 1.0, // V1-Magenta
        -1.0,  1.0, -1.0, 0.0, 0.0, 1.0, 1.0, // V6-blue
        -1.0, -1.0, -1.0, 0.0, 0.0, 0.0, 1.0, // V7-Black
        -1.0,  1.0,  1.0, 1.0, 0.0, 1.0, 1.0, // V1-Magenta
        -1.0, -1.0, -1.0, 0.0, 0.0, 0.0, 1.0, // V7-Black
        -1.0, -1.0,  1.0, 1.0, 0.0, 0.0, 1.0, // V2-Red

        // down
        -1.0, -1.0, -1.0, 0.0, 0.0, 0.0, 1.0,  // V7-Black
         1.0, -1.0, -1.0, 0.0, 1.0, 0.0, 1.0, // V4-Green
         1.0, -1.0,  1.0, 1.0, 1.0, 0.0, 1.0, // V3-Yellow
        -1.0, -1.0, -1.0, 0.0, 0.0, 0.0, 1.0, // V7-Black
         1.0, -1.0,  1.0, 1.0, 1.0, 0.0, 1.0, // V3-Yellow
        -1.0, -1.0,  1.0, 1.0, 0.0, 0.0, 1.0, // V2-Red

        // back
         1.0, -1.0, -1.0, 0.0, 1.0, 0.0, 1.0, // V4-Green
        -1.0, -1.0, -1.0, 0.0, 0.0, 0.0, 1.0, // V7-Black
        -1.0,  1.0, -1.0, 0.0, 0.0, 1.0, 1.0, // V6-blue
         1.0, -1.0, -1.0, 0.0, 1.0, 0.0, 1.0, // V4-Green
        -1.0,  1.0, -1.0, 0.0, 0.0, 1.0, 1.0, // V6-blue
         1.0,  1.0, -1.0, 0.0, 1.0, 1.0, 1.0  // V5-cyan
    };
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

    glGenVertexArrays(1, &VAO_Cube);
    glGenBuffers(1, &VBO_Cube);

    glBindVertexArray(VAO_Cube);
    glBindBuffer(GL_ARRAY_BUFFER, VBO_Cube);
    glBufferData(GL_ARRAY_BUFFER, sizeof(verticesCube), verticesCube, GL_STATIC_DRAW);
    // vertices - Position
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // vertices - Color
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glGenVertexArrays(1, &VAO_Pyramid);
    glGenBuffers(1, &VBO_Pyramid);
    glBindVertexArray(VAO_Pyramid);
    glBindBuffer(GL_ARRAY_BUFFER, VBO_Pyramid);
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

    Shader sunShader("vertexShader.glsl", "fragmentShader.glsl");
    Shader planetShader("vertexShader.glsl", "fragmentShader.glsl");
    Shader moonShader("vertexShader.glsl", "fragmentShader.glsl");

    vMat = glm::translate(glm::mat4(1.0f), glm::vec3(-0.0f, -0.0f, -12.0f));
	mvStack.push(vMat);

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

    //----------------------  cube == sun
    sunShader.use();
    sunShader.setMat4("uProjMatrix", projMatrix);
    mvStack.push(mvStack.top());
	mvStack.top() *= glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f));
	mvStack.push(mvStack.top());
	mvStack.top() *= rotate(glm::mat4(1.0f), (float)currentTime, glm::vec3(1.0, 0.0, 0.0));
    sunShader.setMat4("umvMatrix", mvStack.top());
    glBindVertexArray(VAO_Cube);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	mvStack.pop();

    //-----------------------  cube == planet
    planetShader.use();
    planetShader.setMat4("uProjMatrix", projMatrix);
	mvStack.push(mvStack.top());
	mvStack.top() *= glm::translate(
        glm::mat4(1.0f), glm::vec3(sin((float)currentTime)*4.0f, 0.0f, cos((float)currentTime)*4.0f));
	mvStack.push(mvStack.top());
	mvStack.top() *= rotate(glm::mat4(1.0f), (float)currentTime, glm::vec3(0.0, 1.0, 0.0));
    mvStack.top() *= scale(glm::mat4(1.0f), glm::vec3(0.7f, 0.7f, 0.7f));
    planetShader.setMat4("umvMatrix", mvStack.top());
	glBindVertexArray(VAO_Cube);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	mvStack.pop();

    //-----------------------  smaller Pyramid == moon
    moonShader.use();
    moonShader.setMat4("uProjMatrix", projMatrix);
	mvStack.push(mvStack.top());
	mvStack.top() *= glm::translate(
        glm::mat4(1.0f), glm::vec3(0.0f, sin((float)currentTime)*2.0, cos((float)currentTime)*2.0)
    );
	mvStack.top() *= rotate(glm::mat4(1.0f), (float)currentTime, glm::vec3(0.0, 0.0, 1.0));
	mvStack.top() *= scale(glm::mat4(1.0f), glm::vec3(0.15f, 0.15f, 0.15f));
    moonShader.setMat4("umvMatrix", mvStack.top());
	glBindVertexArray(VAO_Pyramid);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glDrawArrays(GL_TRIANGLES, 0, 12);
	mvStack.pop(); mvStack.pop(); mvStack.pop();
	mvStack.pop();  // the final pop is for the view matrix

}