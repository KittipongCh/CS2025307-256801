#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <string>
#include <fstream>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

using namespace std;

GLuint renderingProgram;
GLuint VAO;
GLuint VBO;

const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 800;

string readFile(const char *filePath){
    string content;
    ifstream fileStream(filePath, ios::in);
    string line = "";

    while(!fileStream.eof()){
        getline(fileStream, line);
        content.append(line + "\n");
    }

    fileStream.close();
    return content;
}

GLuint createShaderProgram(){
    int success;
    char infoLog[512];

    GLuint vShader = glCreateShader(GL_VERTEX_SHADER);
    GLuint fShader = glCreateShader(GL_FRAGMENT_SHADER);
    GLuint vfProgram = glCreateProgram();

    string vertShaderStr = readFile("vertexShader.glsl");
    string fragShaderStr = readFile("fragmentShader.glsl");
    const char *vertShaderSrc = vertShaderStr.c_str();
    const char *fragShaderSrc = fragShaderStr.c_str();

    glShaderSource(vShader, 1, &vertShaderSrc,NULL);
    glCompileShader(vShader);
    glGetShaderiv(vShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vShader, 512, NULL, infoLog);
        cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << endl;
    }

    glShaderSource(fShader, 1, &fragShaderSrc, NULL);
    glCompileShader(fShader);
    glGetShaderiv(fShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fShader, 512, NULL, infoLog);
        cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << endl;
    }

    glAttachShader(vfProgram, vShader);
    glAttachShader(vfProgram, fShader);
    glLinkProgram(vfProgram);
    glGetProgramiv(vfProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(vfProgram, 512, NULL, infoLog);
        cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << endl;
    }

    glDeleteShader(vShader);
    glDeleteShader(fShader);

    return vfProgram;
}

void init(GLFWwindow* window) {
    renderingProgram = createShaderProgram();
    float vertices[] = {
        // front
         1.0,  1.0,  1.0, 1.0, 1.0, 1.0, // V0-White
        -1.0,  1.0,  1.0, 1.0, 0.0, 1.0, // V1-Magenta
        -1.0, -1.0,  1.0, 1.0, 0.0, 0.0, // V2-Red
         1.0,  1.0,  1.0, 1.0, 1.0, 1.0, // V0-White
        -1.0, -1.0,  1.0, 1.0, 0.0, 0.0, // V2-Red
         1.0, -1.0,  1.0, 1.0, 1.0, 0.0, // V3-Yellow

        // right
         1.0,  1.0,  1.0, 1.0, 1.0, 1.0, // V0-White
         1.0, -1.0,  1.0, 1.0, 1.0, 0.0, // V3-Yellow
         1.0, -1.0, -1.0, 0.0, 1.0, 0.0, // V4-Green
         1.0,  1.0,  1.0, 1.0, 1.0, 1.0, // V0-White
         1.0, -1.0, -1.0, 0.0, 1.0, 0.0, // V4-Green
         1.0,  1.0, -1.0, 0.0, 1.0, 1.0, // V5-cyan
        
        // up
         1.0,  1.0,  1.0, 1.0, 1.0, 1.0, // V0-White
         1.0,  1.0, -1.0, 0.0, 1.0, 1.0, // V5-cyan
        -1.0,  1.0, -1.0, 0.0, 0.0, 1.0, // V6-blue
         1.0,  1.0,  1.0, 1.0, 1.0, 1.0, // V0-White
        -1.0,  1.0, -1.0, 0.0, 0.0, 1.0, // V6-blue
        -1.0,  1.0,  1.0, 1.0, 0.0, 1.0, // V1-Magenta

        // left
        -1.0,  1.0,  1.0, 1.0, 0.0, 1.0, // V1-Magenta
        -1.0,  1.0, -1.0, 0.0, 0.0, 1.0, // V6-blue
        -1.0, -1.0, -1.0, 0.0, 0.0, 0.0, // V7-Black
        -1.0,  1.0,  1.0, 1.0, 0.0, 1.0, // V1-Magenta
        -1.0, -1.0, -1.0, 0.0, 0.0, 0.0, // V7-Black
        -1.0, -1.0,  1.0, 1.0, 0.0, 0.0, // V2-Red

        // down
        -1.0, -1.0, -1.0, 0.0, 0.0, 0.0,  // V7-Black
         1.0, -1.0, -1.0, 0.0, 1.0, 0.0, // V4-Green
         1.0, -1.0,  1.0, 1.0, 1.0, 0.0, // V3-Yellow
        -1.0, -1.0, -1.0, 0.0, 0.0, 0.0, // V7-Black
         1.0, -1.0,  1.0, 1.0, 1.0, 0.0, // V3-Yellow
        -1.0, -1.0,  1.0, 1.0, 0.0, 0.0, // V2-Red

        // back
         1.0, -1.0, -1.0, 0.0, 1.0, 0.0, // V4-Green
        -1.0, -1.0, -1.0, 0.0, 0.0, 0.0, // V7-Black
        -1.0,  1.0, -1.0, 0.0, 0.0, 1.0, // V6-blue
         1.0, -1.0, -1.0, 0.0, 1.0, 0.0, // V4-Green
        -1.0,  1.0, -1.0, 0.0, 0.0, 1.0, // V6-blue
         1.0,  1.0, -1.0, 0.0, 1.0, 1.0 // V5-cyan
    };

    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ARRAY_BUFFER, 1);

    glBindVertexArray(0);
}

void display(GLFWwindow* window, double currentTime) {
    glClearColor(0.0, 0.0, 0.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glUseProgram(renderingProgram);
    glBindVertexArray(VAO);

    // Model
    /*
    int uniformModelMatrix = glGetUniformLocation(renderingProgram, "uModelMatrix");
    float ANGLE = 45;
    float radian = glm::radians(ANGLE);
    glm::vec3 axis = glm::vec3(0.0, 0.0, 1.0);
    glm::mat4 modelMatrix = glm::mat4(1.0);
    modelMatrix = glm::rotate(modelMatrix, radian, axis);
    modelMatrix = glm::translate(modelMatrix, glm::vec3(-3.0, 0.0, 0.0));
    glUniformMatrix4fv(uniformModelMatrix, 1, GL_FALSE, glm::value_ptr(modelMatrix));
    */

    // View
    int uniformViewMatrix = glGetUniformLocation(renderingProgram, "uViewMatrix");
    glm::mat4 viewMatrix = glm::mat4(1.0f);
    glm::vec3 cameraPosition(5.0f, 0.0f, 8.0f); 
    glm::vec3 target1Point(0.0f, -2.0f, 0.0f);
    glm::vec3 target2Point(0.0f, 2.0f, 0.0f);
    glm::vec3 target3Point(2.0f, 0.0f, 0.0f);
    glm::vec3 target4Point(-2.0f, 0.0f, 0.0f);
    glm::vec3 upVector(0.0, 1.0, 0.0);

    // Projection
    int uniformProjMatrix = glGetUniformLocation(renderingProgram, "uProjMatrix");
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

    // Draw Cube 1
    viewMatrix = glm::lookAt(
        cameraPosition, //Position of the camera
        target1Point, // Position where the camera is looking at
        upVector // Normalized up vector, how the camera is oriented. Typically (0, 0, 1)
    );
    
    glUniformMatrix4fv(uniformViewMatrix, 1, GL_FALSE, glm::value_ptr(viewMatrix));
    glUniformMatrix4fv(uniformProjMatrix, 1, GL_FALSE, glm::value_ptr(projMatrix));

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    // vertices - Position
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // color
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glEnable(GL_DEPTH_TEST);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    // Draw Cube 2
    viewMatrix = glm::lookAt(
        cameraPosition, //Position of the camera
        target2Point, // Position where the camera is looking at
        upVector // Normalized up vector, how the camera is oriented. Typically (0, 0, 1)
    );

    glUniformMatrix4fv(uniformViewMatrix, 1, GL_FALSE, glm::value_ptr(viewMatrix));
    glUniformMatrix4fv(uniformProjMatrix, 1, GL_FALSE, glm::value_ptr(projMatrix));

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    // vertices - Position
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // color
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glEnable(GL_DEPTH_TEST);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    // Draw Cube 3
    viewMatrix = glm::lookAt(
        cameraPosition, //Position of the camera
        target3Point, // Position where the camera is looking at
        upVector // Normalized up vector, how the camera is oriented. Typically (0, 0, 1)
    );

    glUniformMatrix4fv(uniformViewMatrix, 1, GL_FALSE, glm::value_ptr(viewMatrix));
    glUniformMatrix4fv(uniformProjMatrix, 1, GL_FALSE, glm::value_ptr(projMatrix));

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    // vertices - Position
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // color
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glEnable(GL_DEPTH_TEST);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    // Draw Cube 4
    viewMatrix = glm::lookAt(
        cameraPosition, //Position of the camera
        target4Point, // Position where the camera is looking at
        upVector // Normalized up vector, how the camera is oriented. Typically (0, 0, 1)
    );

    glUniformMatrix4fv(uniformViewMatrix, 1, GL_FALSE, glm::value_ptr(viewMatrix));
    glUniformMatrix4fv(uniformProjMatrix, 1, GL_FALSE, glm::value_ptr(projMatrix));

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    // vertices - Position
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // color
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glEnable(GL_DEPTH_TEST);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    glBindVertexArray(0);
}

int main(void)
{
    /* Initialize the library */
    if (!glfwInit()){
        exit(EXIT_FAILURE);
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

    /* Create a windowed mode window and its OpenGL context */
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "3D: Cube V4 - Draw Two Cubes", NULL, NULL);
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