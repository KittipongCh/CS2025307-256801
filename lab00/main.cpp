#include <GL/glut.h>

void init(void){
    glClearColor(1.0, 1.0, 1.0, 1.0);
    glLineWidt(4.0);
    glMatrixMode(GL_PROJECTION);
    gluOrtho2D(0.0, 300.0, 0.0, 300.0);
}

void LineAndRectangle(void){
    glClear(GL_COLOR_BUFFER_BIT);
    glColor3f(0.0, 0.0, 1.0);
    glRectf(180.0, 180.0, 280.0, 280.0);
    glBegin(GL_LINES);
        glColor3f(1.0, 0.0, 0.0);
        glVertex2i(20, 20);
        glVertex2i(150, 150);
    glEnd();

    glFlush();
}

void main(int argc, char** argv){
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowPosition(50, 100);
    glutInitWindowSize(400, 400);
    glutCreateWindow("An Example OpenGL Program");
    init();
    glutDisplayFunc(LineAndRectangle);
    glutMainLoop();
}