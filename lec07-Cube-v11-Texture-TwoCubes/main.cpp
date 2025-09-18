#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <string>
#include <fstream>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

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
        // X, Y, Z, R, G, B, x, y
        // front
         1.0,  1.0,  1.0, 1.0, 1.0, 1.0, 1.0, 1.0, // V0-White
        -1.0,  1.0,  1.0, 1.0, 0.0, 1.0, 0.0, 1.0, // V1-Magenta
        -1.0, -1.0,  1.0, 1.0, 0.0, 0.0, 0.0, 0.0, // V2-Red
         1.0,  1.0,  1.0, 1.0, 1.0, 1.0, 1.0, 1.0, // V0-White
        -1.0, -1.0,  1.0, 1.0, 0.0, 0.0, 0.0, 0.0, // V2-Red
         1.0, -1.0,  1.0, 1.0, 1.0, 0.0, 1.0, 0.0, // V3-Yellow

        // right
         1.0,  1.0,  1.0, 1.0, 1.0, 1.0, 0.0, 1.0, // V0-White
         1.0, -1.0,  1.0, 1.0, 1.0, 0.0, 0.0, 0.0, // V3-Yellow
         1.0, -1.0, -1.0, 0.0, 1.0, 0.0, 1.0, 0.0, // V4-Green
         1.0,  1.0,  1.0, 1.0, 1.0, 1.0, 0.0, 1.0, // V0-White
         1.0, -1.0, -1.0, 0.0, 1.0, 0.0, 1.0, 0.0, // V4-Green
         1.0,  1.0, -1.0, 0.0, 1.0, 1.0, 1.0, 1.0, // V5-cyan
        
        // up
         1.0,  1.0,  1.0, 1.0, 1.0, 1.0, 0.0, 1.0, // V0-White
         1.0,  1.0, -1.0, 0.0, 1.0, 1.0, 0.0, 0.0, // V5-cyan
        -1.0,  1.0, -1.0, 0.0, 0.0, 1.0, 1.0, 0.0, // V6-blue
         1.0,  1.0,  1.0, 1.0, 1.0, 1.0, 0.0, 1.0, // V0-White
        -1.0,  1.0, -1.0, 0.0, 0.0, 1.0, 1.0, 0.0, // V6-blue
        -1.0,  1.0,  1.0, 1.0, 0.0, 1.0, 1.0, 1.0, // V1-Magenta

        // left
        -1.0,  1.0,  1.0, 1.0, 0.0, 1.0, 0.0, 1.0, // V1-Magenta
        -1.0,  1.0, -1.0, 0.0, 0.0, 1.0, 0.0, 0.0, // V6-blue
        -1.0, -1.0, -1.0, 0.0, 0.0, 0.0, 1.0, 0.0, // V7-Black
        -1.0,  1.0,  1.0, 1.0, 0.0, 1.0, 0.0, 1.0, // V1-Magenta
        -1.0, -1.0, -1.0, 0.0, 0.0, 0.0, 1.0, 0.0, // V7-Black
        -1.0, -1.0,  1.0, 1.0, 0.0, 0.0, 1.0, 1.0, // V2-Red

        // down
        -1.0, -1.0, -1.0, 0.0, 0.0, 0.0, 0.0, 1.0, // V7-Black
         1.0, -1.0, -1.0, 0.0, 1.0, 0.0, 0.0, 0.0, // V4-Green
         1.0, -1.0,  1.0, 1.0, 1.0, 0.0, 1.0, 0.0, // V3-Yellow
        -1.0, -1.0, -1.0, 0.0, 0.0, 0.0, 0.0, 1.0, // V7-Black
         1.0, -1.0,  1.0, 1.0, 1.0, 0.0, 1.0, 0.0, // V3-Yellow
        -1.0, -1.0,  1.0, 1.0, 0.0, 0.0, 1.0, 1.0, // V2-Red

        // back
         1.0, -1.0, -1.0, 0.0, 1.0, 0.0, 0.0, 1.0, // V4-Green
        -1.0, -1.0, -1.0, 0.0, 0.0, 0.0, 0.0, 0.0, // V7-Black
        -1.0,  1.0, -1.0, 0.0, 0.0, 1.0, 1.0, 0.0, // V6-blue
         1.0, -1.0, -1.0, 0.0, 1.0, 0.0, 0.0, 1.0, // V4-Green
        -1.0,  1.0, -1.0, 0.0, 0.0, 1.0, 1.0, 0.0, // V6-blue
         1.0,  1.0, -1.0, 0.0, 1.0, 1.0, 1.0, 1.0 // V5-cyan
    };

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    // vertices - Position (X, Y, Z)
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // vertices - color (R, G, B)
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    // vertices - Texture (x,y)
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    // load and create a texture 
    // -------------------------
    unsigned int texture1;
    glGenTextures(1, &texture1);
    // all upcoming GL_TEXTURE_2D operations now have effect on this texture object
    glBindTexture(GL_TEXTURE_2D, texture1);
    // set the texture wrapping parameters
    // set texture wrapping to GL_REPEAT (default wrapping method)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // set texture filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // load image, create texture and generate mipmaps
    int width, height, nrChannels;
    // tell stb_image.h to flip loaded texture's on the y-axis.
    stbi_set_flip_vertically_on_load(true);
    // The FileSystem::getPath(...) is part of the GitHub repository so we can find files on any IDE/platform; replace it with your own image path.
    unsigned char *data = stbi_load("Texturelabs_Glass_125S.jpg", &width, &height, &nrChannels, 0);
    if (data){
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else{
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ARRAY_BUFFER, 1);

    glBindVertexArray(0);
}

void display(GLFWwindow* window, double currentTime) {
    float cameraX = 0.0f; 
    float cameraY = 0.0f;
    float cameraZ = 8.0f;
    glClearColor(0.0, 0.0, 0.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);
    glUseProgram(renderingProgram);
    glBindVertexArray(VAO);

    // Texture
    int uniformTexture = glGetUniformLocation(renderingProgram, "utexture1");
    glUniform1i(uniformTexture, 0);

    // Model
    int uniformModelMatrix = glGetUniformLocation(renderingProgram, "uModelMatrix");
    //float ANGLE = 45;
    //float radian = glm::radians(ANGLE);
    //glm::vec3 axis = glm::vec3(0.0, 0.0, 1.0);
    glm::mat4 modelMatrix = glm::mat4(1.0);
    //modelMatrix = glm::rotate(modelMatrix, radian, axis);
    //modelMatrix = glm::translate(modelMatrix, glm::vec3(-3.0, 0.0, 0.0));
    //glUniformMatrix4fv(uniformModelMatrix, 1, GL_FALSE, glm::value_ptr(modelMatrix));

    // View
    int uniformViewMatrix = glGetUniformLocation(renderingProgram, "uViewMatrix");
    glm::mat4 viewMatrix = glm::mat4(1.0f);
    //glm::vec3 cameraPosition(3.0f, 3.0f, 7.0f); 
    //glm::vec3 targetPoint(0.0, 0.0, 0.0);
    //glm::vec3 upVector(0.0, 1.0, 0.0);
    //viewMatrix = glm::lookAt(
    //    cameraPosition, //Position of the camera
    //    targetPoint, // Position where the camera is looking at
    //    upVector // Normalized up vector, how the camera is oriented. Typically (0, 0, 1)
    //);
    viewMatrix = glm::translate(glm::mat4(1.0), glm::vec3(-cameraX, -cameraY, -cameraZ));
    //glUniformMatrix4fv(uniformViewMatrix, 1, GL_FALSE, glm::value_ptr(viewMatrix));

    // Projection
    int uniformProjMatrix = glGetUniformLocation(renderingProgram, "uProjMatrix");
    float ANGLE = 60.0f;
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
    //glUniformMatrix4fv(uniformProjMatrix, 1, GL_FALSE, glm::value_ptr(projMatrix));

    // Draw Cube 1
    modelMatrix = glm::translate(glm::mat4(1.0), glm::vec3(-4.0f, -2.0f, -1.0f));
    
    glUniformMatrix4fv(uniformViewMatrix, 1, GL_FALSE, glm::value_ptr(viewMatrix));
    glUniformMatrix4fv(uniformProjMatrix, 1, GL_FALSE, glm::value_ptr(projMatrix));
    glUniformMatrix4fv(uniformModelMatrix, 1, GL_FALSE, glm::value_ptr(modelMatrix));
    glEnable(GL_DEPTH_TEST);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    // Draw Cube 2
    modelMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(2.0f, 2.0f, -2.0f));

    glUniformMatrix4fv(uniformViewMatrix, 1, GL_FALSE, glm::value_ptr(viewMatrix));
    glUniformMatrix4fv(uniformProjMatrix, 1, GL_FALSE, glm::value_ptr(projMatrix));
    glUniformMatrix4fv(uniformModelMatrix, 1, GL_FALSE, glm::value_ptr(modelMatrix));
    glEnable(GL_DEPTH_TEST);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    // Draw Cube 3
    modelMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(-2.0f, 2.0f, 0.0f));

    glUniformMatrix4fv(uniformViewMatrix, 1, GL_FALSE, glm::value_ptr(viewMatrix));
    glUniformMatrix4fv(uniformProjMatrix, 1, GL_FALSE, glm::value_ptr(projMatrix));
    glUniformMatrix4fv(uniformModelMatrix, 1, GL_FALSE, glm::value_ptr(modelMatrix));
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
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "3D: Cube V10 - Texture", NULL, NULL);
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