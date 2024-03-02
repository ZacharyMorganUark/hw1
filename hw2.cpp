//---------------------------------------
// Program: building3.cpp used
// Purpose: Demonstrate use of mouse callbacks to draw lines.
// Author:  Zachary Morgan
// Date:    3/1/2024
//---------------------------------------
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#ifdef MAC
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif
//t
int windowWidth = 500;
int windowHeight = 500;
int pointCount = 0;
float points[500][2]; // Array to store points
bool isDrawing = false;
bool moveSquare = false;
float squarePosition[2] = {10.0f, 10.0f}; // Initial position of the red square
float speed = 1.0f; // Adjust the speed of the square

void init() {
    glClearColor(0.0, 0.0, 0.0, 1.0);
    glMatrixMode(GL_PROJECTION);
    gluOrtho2D(0, windowWidth, windowHeight, 0);
}

void drawLine() {
    glColor3f(1.0, 1.0, 1.0);
    glBegin(GL_LINE_STRIP);
    for (int i = 0; i < pointCount - 1; ++i) {
        for (int j = 0; j <= 100; ++j) {
            float t = static_cast<float>(j) / 100;
            float x = (1 - t) * points[i][0] + t * points[i + 1][0];
            float y = (1 - t) * points[i][1] + t * points[i + 1][1];
            glVertex2f(x, y);
        }
    }
    glEnd();
}

void drawSquare() {
    glColor3f(1.0, 0.0, 0.0);
    glBegin(GL_QUADS);
    glVertex2f(squarePosition[0], squarePosition[1]);
    glVertex2f(squarePosition[0] + 20.0f, squarePosition[1]);
    glVertex2f(squarePosition[0] + 20.0f, squarePosition[1] + 20.0f);
    glVertex2f(squarePosition[0], squarePosition[1] + 20.0f);
    glEnd();
}

void moveSquare() {
    static int currentPoint = 0;

    if (currentPoint < pointCount) {
        float dx = points[currentPoint][0] - squarePosition[0];
        float dy = points[currentPoint][1] - squarePosition[1];
        float distance = std::sqrt(dx * dx + dy * dy);

        if (distance > speed) {
            float ratio = speed / distance;
            squarePosition[0] += ratio * dx;
            squarePosition[1] += ratio * dy;
        } else {
            // Move to the next point
            squarePosition[0] = points[currentPoint][0];
            squarePosition[1] = points[currentPoint][1];
            currentPoint++;
        }

        glutPostRedisplay();
    }
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT);
    drawLine();

    if (moveSquare) {
        moveSquare();
        drawSquare();
    }

    glutSwapBuffers();
}

void mouse(int button, int state, int x, int y) {
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
        points[pointCount][0] = x;
        points[pointCount][1] = y;
        ++pointCount;
        isDrawing = true;
        glutPostRedisplay();
    }
}

void motion(int x, int y) {
    if (isDrawing && pointCount > 0) {
        points[pointCount][0] = x;
        points[pointCount][1] = y;
        glutPostRedisplay();
    }
}

void keyboard(unsigned char key, int x, int y) {
    if (key == 27) { 
        isDrawing = false;
        pointCount = 0;
        moveSquare = false;
        glutPostRedisplay();
    } else if (key == 'm' && pointCount > 0) {
        moveSquare = true;
        glutPostRedisplay();
    }
}

int main(int argc, char *argv[]) {
    glutInit(&argc, argv);
    glutInitWindowSize(windowWidth, windowHeight);
    glutInitWindowPosition(100, 100);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
    glutCreateWindow("Homework 2");

    glutDisplayFunc(display);
    glutMouseFunc(mouse);
    glutMotionFunc(motion);
    glutKeyboardFunc(keyboard); // Register keyboard callback

    init();
    printf("   'mouse down' - sets a point and the next mouse down action will connect the points\n");
    printf("   press 'm' - draw the sqaure and make it begin moving along the path\n");
    glutMainLoop();
    return 0;
}
