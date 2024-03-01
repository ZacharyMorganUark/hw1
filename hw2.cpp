#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#ifdef MAC
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

const int MAX_VERTICES = 100;

struct Point {
    float x, y;
};

Point vertices[MAX_VERTICES];
int vertexCount = 0;
bool drawing = false;
bool moving = false;
clock_t moveStartTime;
const int MOVEMENT_DURATION = 5000;

void drawPolygon() {
    glBegin(GL_POLYGON);
    for (int i = 0; i < vertexCount; ++i) {
        glVertex2f(vertices[i].x, vertices[i].y);
    }
    glEnd();
}

void mouseClick(int button, int state, int x, int y) {
    float x_scale = 2.0 / glutGet(GLUT_WINDOW_WIDTH);
    float y_scale = -2.0 / glutGet(GLUT_WINDOW_HEIGHT);

    if (button == GLUT_LEFT_BUTTON) {
        if (state == GLUT_DOWN) {
            if (!moving) {
                drawing = true;
                vertexCount = 0;
            }
        } else if (state == GLUT_UP) {
            drawing = false;
            if (!moving) {
                vertices[vertexCount].x = (x - glutGet(GLUT_WINDOW_WIDTH) / 2) * x_scale;
                vertices[vertexCount].y = (glutGet(GLUT_WINDOW_HEIGHT) / 2 - y) * y_scale;
                vertexCount++;
            } else {
                moving = false;
                moveStartTime = clock();
            }
        }
    }
}

void mouseMotion(int x, int y) {
    if (drawing && vertexCount < MAX_VERTICES) {
        float x_scale = 2.0 / glutGet(GLUT_WINDOW_WIDTH);
        float y_scale = -2.0 / glutGet(GLUT_WINDOW_HEIGHT);

        vertices[vertexCount].x = (x - glutGet(GLUT_WINDOW_WIDTH) / 2) * x_scale;
        vertices[vertexCount].y = (glutGet(GLUT_WINDOW_HEIGHT) / 2 - y) * y_scale;
        ++vertexCount;
        glutPostRedisplay();
    }
}

void movePolygon() {
    if (vertexCount > 1) {
        clock_t currentTime = clock();
        double elapsedTime = (double)(currentTime - moveStartTime) / CLOCKS_PER_SEC;

        float t = fmin(1.0, elapsedTime / (MOVEMENT_DURATION / 1000.0));
        float currentX = (1 - t) * vertices[0].x + t * vertices[vertexCount - 1].x;
        float currentY = (1 - t) * vertices[0].y + t * vertices[vertexCount - 1].y;

        glTranslatef(currentX, currentY, 0.0);
        drawPolygon();

        if (t >= 1.0) {
            drawing = false;
            moving = true;
        }
    }
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT);
    glColor3f(1.0, 1.0, 1.0);

    if (drawing) {
        drawPolygon();
    } else {
        movePolygon();
    }

    glutSwapBuffers();
}

void init() {
    glClearColor(0.0, 0.0, 0.0, 1.0);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(-1.0, 1.0, -1.0, 1.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

int main(int argc, char *argv[]) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutCreateWindow("Drawing and Moving Polygon");
    glutDisplayFunc(display);
    glutMouseFunc(mouseClick);
    glutMotionFunc(mouseMotion);
    init();

    glutMainLoop();

    return 0;
}
