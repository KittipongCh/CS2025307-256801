
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>

#define numVAOs 1

using namespace std;

GLuint renderingProgram;
GLuint vao[numVAOs];

GLuint createShaderProgram(){
    const char *vshaderSource =
    "#version 330 \n"
    "void main(void)\n"
    "{gl_Position = vec4(0.0, 0.0, 0.0, 1.0);}";

    const char *fshaderSource =
    "#version 330 \n"
    "out vec4 color;\n"
    "void main(void){\n"
    "if (gl_FragCoord.x < 295) \n"
    "color = vec4(1.0, 0.0, 0.0, 1.0);\n"
    "else\n"
    "color = vec4(0.0, 0.0, 1.0, 1.0);\n"
    "}";

    GLuint vShader = glCreateShader(GL_VERTEX_SHADER);
    GLuint fShader = glCreateShader(GL_FRAGMENT_SHADER);

    glShaderSource(vShader, 1, &vshaderSource,NULL);
    glShaderSource(fShader, 1, &fshaderSource, NULL);
    glCompileShader(vShader);
    glCompileShader(fShader);

    GLuint vfProgram = glCreateProgram();
    glAttachShader(vfProgram, vShader);
    glAttachShader(vfProgram, fShader);
    glLinkProgram(vfProgram);

    return vfProgram;
}

void init(GLFWwindow* window) {
    renderingProgram = createShaderProgram();
    glGenVertexArrays(numVAOs, vao);
    glBindVertexArray(vao[0]);
}

void display(GLFWwindow* window, double currentTime) {
    glClearColor(0.0, 0.0, 0.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT);
    glUseProgram(renderingProgram);
    glPointSize(40.0);
    glDrawArrays(GL_POINTS, 0, 1);
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
    GLFWwindow* window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
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