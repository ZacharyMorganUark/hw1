#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif
//sadhg
int curveResolution = 100;
float startPoint[2]; // x, y
float endPoint[2];   // x, y
bool drawing = false;

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
            startPoint[0] = x * x_scale - 1.0;
            startPoint[1] = y * y_scale + 1.0;
            drawing = true;
        } else if (state == GLUT_UP) {
            endPoint[0] = x * x_scale - 1.0;
            endPoint[1] = y * y_scale + 1.0;
            drawing = false;
            glutPostRedisplay();
        }
    }
}

void motion(int x, int y) {
    float x_scale = 2.0 / glutGet(GLUT_WINDOW_WIDTH);
    float y_scale = -2.0 / glutGet(GLUT_WINDOW_HEIGHT);

    if (drawing) {
        endPoint[0] = x * x_scale - 1.0;
        endPoint[1] = y * y_scale + 1.0;
        glutPostRedisplay();
    }
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT);

    // Draw the existing line
    if (drawing) {
        glColor3f(0.0f, 1.0f, 0.0f); // Green color
        glLineWidth(2.0);
        glBegin(GL_LINES);
        glVertex2f(startPoint[0], startPoint[1]);
        glVertex2f(endPoint[0], endPoint[1]);
        glEnd();
        glLineWidth(1.0);
    }

    // Draw the Bezier curve
    if (!drawing) {
        float controlX = (startPoint[0] + endPoint[0]) / 2.0;
        float controlY = (startPoint[1] + endPoint[1]) / 2.0;
        drawBezierCurve(startPoint[0], startPoint[1], endPoint[0], endPoint[1], controlX, controlY);
    }

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
    glutCreateWindow("Bark");
    glutDisplayFunc(display);
    glutMouseFunc(mouse);
    glutMotionFunc(motion);
    init();
    glutMainLoop();
    return 0;
}
