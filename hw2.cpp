#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif
//dfdf
#define LINE_COUNT 1000

int curveResolution = 100;
int pointCount = 0;
float points[LINE_COUNT][2]; // x, y

void drawBezierCurve(float startX, float startY, float endX, float endY, float controlX, float controlY) {
    glColor3f(1.0f, 0.0f, 0.0f); // Red color
    glBegin(GL_LINE_STRIP);
    for (int i = 0; i <= curveResolution; ++i) {
        float t = static_cast<float>(i) / curveResolution;
        float x = (1 - t) * (1 - t) * startX + 2 * (1 - t) * t * controlX + t * t * endX;
        float y = (1 - t) * (1 - t) * startY + 2 * (1 - t) * t * controlY + t * t * endY;
        glVertex2f(x, y);
    }
    glEnd();
}

void mouse(int button, int state, int x, int y) {
    float x_scale = 2.0 / glutGet(GLUT_WINDOW_WIDTH);
    float y_scale = -2.0 / glutGet(GLUT_WINDOW_HEIGHT);

    if (button == GLUT_LEFT_BUTTON) {
        if (state == GLUT_DOWN) {
            points[pointCount][0] = x * x_scale - 1.0;
            points[pointCount][1] = y * y_scale + 1.0;
            pointCount++;
            glutPostRedisplay();
        }
    }
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT);

    // Draw the existing points
    glColor3f(0.0f, 1.0f, 0.0f); // Green color
    glPointSize(5.0);

    glBegin(GL_POINTS);
    for (int i = 0; i < pointCount; ++i) {
        glVertex2f(points[i][0], points[i][1]);
    }
    glEnd();

    // Draw Bezier curves between consecutive points
    glColor3f(1.0f, 0.0f, 0.0f); // Red color
    for (int i = 1; i < pointCount; i += 2) {
        float controlX = (points[i - 1][0] + points[i + 1][0]) / 2.0;
        float controlY = (points[i - 1][1] + points[i + 1][1]) / 2.0;
        drawBezierCurve(points[i - 1][0], points[i - 1][1], points[i][0], points[i][1], controlX, controlY);
    }

    glPointSize(1.0);

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
    glutCreateWindow("Bed");
    glutDisplayFunc(display);
    glutMouseFunc(mouse);
    init();
    glutMainLoop();
    return 0;
}
