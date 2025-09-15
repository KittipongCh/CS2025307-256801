#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <string>
#include <fstream>

#define numVAOs 1

using namespace std;

GLuint renderingProgram;
GLuint VAO;
GLuint VBO_vertrices;
GLuint VBO_colors;

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
    float vertrices[] = {
         0.5,  0.5, 0.0,
        -0.5,  0.5, 0.0,
        -0.5, -0.5, 0.0,
         0.5, -0.5, 0.0,
         0.0,  0.0, 0.0
    };

    float vertexColors[] = {
        1.0, 0.0, 0.0,
        0.0, 1.0, 0.0,
        0.0, 0.0, 1.0,
        1.0, 1.0, 0.0,
        0.0, 1.0, 1.0
    };

    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    // vertrices
    glGenBuffers(1, &VBO_vertrices);
    glBindBuffer(GL_ARRAY_BUFFER, VBO_vertrices);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertrices), vertrices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

    // colors
    glGenBuffers(1, &VBO_colors);
    glBindBuffer(GL_ARRAY_BUFFER, VBO_colors);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertexColors), vertexColors, GL_STATIC_DRAW);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glBindVertexArray(0);
}

void display(GLFWwindow* window, double currentTime) {
    glUseProgram(renderingProgram);
    glBindVertexArray(VAO);
    glClearColor(0.0, 0.0, 0.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT);
    glPointSize(10.0);
    glDrawArrays(GL_POINTS, 0, 5);
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
    GLFWwindow* window = glfwCreateWindow(640, 480, "Data, Buffer, Vertex Attribute", NULL, NULL);
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