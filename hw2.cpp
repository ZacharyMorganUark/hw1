#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif
//sam

#define LINE_COUNT 1000

int curveResolution = 100;
float lineStartX, lineStartY, lineEndX, lineEndY, controlPointX, controlPointY;

void drawBezierCurve() {
    glColor3f(1.0f, 0.0f, 0.0f); // Red color
    glBegin(GL_LINE_STRIP);
    for (int i = 0; i <= curveResolution; ++i) {
        float t = static_cast<float>(i) / curveResolution;
        float x = (1 - t) * (1 - t) * lineStartX + 2 * (1 - t) * t * controlPointX + t * t * lineEndX;
        float y = (1 - t) * (1 - t) * lineStartY + 2 * (1 - t) * t * controlPointY + t * t * lineEndY;
        glVertex2f(x, y);
    }
    glEnd();
}

void mouse(int button, int state, int x, int y) {
    float x_scale = 2.0 / glutGet(GLUT_WINDOW_WIDTH);
    float y_scale = -2.0 / glutGet(GLUT_WINDOW_HEIGHT);

    if (state == GLUT_DOWN) {
        lineStartX = x * x_scale - 1.0;
        lineStartY = y * y_scale + 1.0;
        controlPointX = lineStartX + 0.2; // Adjust as needed
        controlPointY = lineStartY + 0.2; // Adjust as needed
    } else if (state == GLUT_UP) {
        lineEndX = x * x_scale - 1.0;
        lineEndY = y * y_scale + 1.0;
        glutPostRedisplay();
    }
}

void motion(int x, int y) {
    float x_scale = 2.0 / glutGet(GLUT_WINDOW_WIDTH);
    float y_scale = -2.0 / glutGet(GLUT_WINDOW_HEIGHT);

    controlPointX = x * x_scale - 1.0;
    controlPointY = y * y_scale + 1.0;

    glutPostRedisplay();
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT);
    drawBezierCurve();
    glutSwapBuffers();
}

void init() {
    glClearColor(0.0, 0.0, 0.0, 1.0);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(-1.0, 1.0, -1.0, 1.0);
}

int main(int argc, char *argv[]) {
    glutInit(&argc, argv);
    glutInitWindowSize(500, 500);
    glutCreateWindow("Bezier Curve");
    glutDisplayFunc(display);
    glutMouseFunc(mouse);
    glutMotionFunc(motion);
    init();
    glutMainLoop();
    return 0;
}
