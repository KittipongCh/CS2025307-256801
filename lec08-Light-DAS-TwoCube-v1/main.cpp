#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <string>
#include <fstream>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

GLuint renderingProgram;
GLuint renderingProgramLightCube;
GLuint VAO;
GLuint VAOLightCube;
GLuint VBO;

const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 800;

std::string readFile(const char *filePath);
GLuint createShaderProgram(const char *vertexShader, const char *fragmentShader);
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
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Light-DAS Two Cube", NULL, NULL);
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

std::string readFile(const char *filePath){
    std::string content;
    std::ifstream fileStream(filePath, std::ios::in);
    std::string line = "";

    while(!fileStream.eof()){
        getline(fileStream, line);
        content.append(line + "\n");
    }

    fileStream.close();
    return content;
}

GLuint createShaderProgram(const char *vertexShader, const char *fragmentShader){
    int success;
    char infoLog[512];

    GLuint vShader = glCreateShader(GL_VERTEX_SHADER);
    GLuint fShader = glCreateShader(GL_FRAGMENT_SHADER);
    GLuint vfProgram = glCreateProgram();

    std::string vertShaderStr = readFile(vertexShader);
    std::string fragShaderStr = readFile(fragmentShader);
    const char *vertShaderSrc = vertShaderStr.c_str();
    const char *fragShaderSrc = fragShaderStr.c_str();

    glShaderSource(vShader, 1, &vertShaderSrc,NULL);
    glCompileShader(vShader);
    glGetShaderiv(vShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    glShaderSource(fShader, 1, &fragShaderSrc, NULL);
    glCompileShader(fShader);
    glGetShaderiv(fShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    glAttachShader(vfProgram, vShader);
    glAttachShader(vfProgram, fShader);
    glLinkProgram(vfProgram);
    glGetProgramiv(vfProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(vfProgram, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }

    glDeleteShader(vShader);
    glDeleteShader(fShader);

    return vfProgram;
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

    // Cube
    renderingProgram = createShaderProgram("vertexShader.glsl", "fragmentShader.glsl");
    glUseProgram(renderingProgram);
    glBindVertexArray(VAO);
    // Light
    int ObjectColor      = glGetUniformLocation(renderingProgram, "uObjectColor");
    int LightColor      = glGetUniformLocation(renderingProgram, "uLightColor");
    int LightDirection  = glGetUniformLocation(renderingProgram, "uLightDirection");
    int ViewPosition    = glGetUniformLocation(renderingProgram, "uViewPosition");
    int AmbientLight    = glGetUniformLocation(renderingProgram, "uAmbientLight");
    glUniform3f(ObjectColor, 1.0f, 0.5f, 0.31f);
    glUniform3f(LightColor, 1.0f, 1.0f, 1.0f);
    glUniform3f(LightDirection, 1.2f, 1.0f, 2.0f);
    glUniform3f(AmbientLight, 0.2f, 0.2f, 0.2f);
    glUniform3f(ViewPosition, -0.5f, 0.0f, -1.0f);
    // Model
    int uniformModelMatrix = glGetUniformLocation(renderingProgram, "uModelMatrix");
    //float ANGLE = 45;
    //float radian = glm::radians(ANGLE);
    //glm::vec3 axis = glm::vec3(0.0, 0.0, 1.0);
    glm::mat4 modelMatrix = glm::mat4(1.0);
    //modelMatrix = glm::rotate(modelMatrix, radian, axis);
    //modelMatrix = glm::translate(modelMatrix, glm::vec3(-1.0, 1.0, 1.0));
    glUniformMatrix4fv(uniformModelMatrix, 1, GL_FALSE, glm::value_ptr(modelMatrix));
    // View
    int uniformViewMatrix = glGetUniformLocation(renderingProgram, "uViewMatrix");
    glm::mat4 viewMatrix = glm::mat4(1.0f);
    glm::vec3 cameraPosition(3.0f, 3.0f, 7.0f); 
    glm::vec3 targetPoint(0.0, 0.0, 0.0);
    glm::vec3 upVector(0.0, 1.0, 0.0);
    viewMatrix = glm::lookAt(
        cameraPosition, //Position of the camera
        targetPoint, // Position where the camera is looking at
        upVector // Normalized up vector, how the camera is oriented. Typically (0, 0, 1)
    );
    glUniformMatrix4fv(uniformViewMatrix, 1, GL_FALSE, glm::value_ptr(viewMatrix));
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
    glUniformMatrix4fv(uniformProjMatrix, 1, GL_FALSE, glm::value_ptr(projMatrix));

    glEnable(GL_DEPTH_TEST);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    // Light Cube
    renderingProgramLightCube = createShaderProgram("lightCubeVertexShader.glsl", "lightCubeFragmentShader.glsl");
    glUseProgram(renderingProgramLightCube);
    glBindVertexArray(VAOLightCube);
    // Model
    int ModelMatrixLightCube = glGetUniformLocation(renderingProgramLightCube, "uModelMatrixLightCube");
    modelMatrix = glm::mat4(1.0);
    modelMatrix = glm::translate(modelMatrix, glm::vec3(0.0f, 0.0f, 5.0f));
    modelMatrix = glm::scale(modelMatrix, glm::vec3(0.2f));
    glUniformMatrix4fv(ModelMatrixLightCube, 1, GL_FALSE, glm::value_ptr(modelMatrix));
    // View
    int ViewMatrixLightCube = glGetUniformLocation(renderingProgramLightCube, "uViewMatrixLightCube");
    glUniformMatrix4fv(ViewMatrixLightCube, 1, GL_FALSE, glm::value_ptr(viewMatrix));
    // Projection
    int ProjMatrixLightCube = glGetUniformLocation(renderingProgramLightCube, "uProjMatrixLightCube");
    glUniformMatrix4fv(ProjMatrixLightCube, 1, GL_FALSE, glm::value_ptr(projMatrix));
    // redering Cube
    glEnable(GL_DEPTH_TEST);
    glDrawArrays(GL_TRIANGLES, 0, 36);

}