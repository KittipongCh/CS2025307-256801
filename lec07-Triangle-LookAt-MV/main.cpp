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
         0.0,  0.5, -0.4, 0.4, 1.0, 0.4,
        -0.5, -0.5, -0.4, 0.4, 1.0, 0.4,
         0.5, -0.5, -0.4, 1.0, 0.4, 0.4,
         0.5,  0.4, -0.2, 1.0, 0.4, 0.4,
        -0.5,  0.4, -0.2, 1.0, 1.0, 0.4,
         0.0, -0.6, -0.2, 1.0, 1.0, 0.4,
         0.0,  0.5,  0.0, 0.4, 0.4, 1.0,
        -0.5, -0.5,  0.0, 0.4, 0.4, 1.0,
         0.5, -0.5,  0.0, 1.0, 0.4, 0.4
    };

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    // vertices - Position
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // color
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ARRAY_BUFFER, 1);

    glBindVertexArray(0);
}

void display(GLFWwindow* window, double currentTime) {
    glClearColor(0.0, 0.0, 0.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT);
    glUseProgram(renderingProgram);
    glBindVertexArray(VAO);

    int uniformModelMatrix = glGetUniformLocation(renderingProgram, "uModelMatrix");
    float ANGLE = 45;
    float radian = glm::radians(ANGLE);
    glm::vec3 axis = glm::vec3(0.0, 0.0, 1.0);
    glm::mat4 modelMatrix = glm::mat4(1.0);
    modelMatrix = glm::rotate(modelMatrix, radian, axis);
    modelMatrix = glm::translate(modelMatrix, glm::vec3(0.5, -0.5, 0.0));
    glUniformMatrix4fv(uniformModelMatrix, 1, GL_FALSE, &modelMatrix[0][0]);

    int uniformViewMatrix = glGetUniformLocation(renderingProgram, "uViewMatrix");
    glm::mat4 viewMatrix = glm::mat4(1.0f);
    glm::vec3 cameraPosition(0.25, 0.25, 0.25); //Position of the camera
    glm::vec3 targetPoint(0.0, 0.0, 0.0); // Position where the camera is looking at
    glm::vec3 upVector(0.0, 1.0, 0.0); // Normalized up vector, how the camera is oriented. Typically (0, 0, 1)
    viewMatrix = glm::lookAt(cameraPosition, targetPoint, upVector);
    glUniformMatrix4fv(uniformViewMatrix, 1, GL_FALSE, &viewMatrix[0][0]);

    glDrawArrays(GL_TRIANGLES, 0, 9);

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
    GLFWwindow* window = glfwCreateWindow(600, 600, "LookAt (GLM), Model Matrix, View Matrix", NULL, NULL);
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