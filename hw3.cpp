#include <math.h>
#include <stdio.h>
#include <fstream>
#include <iostream>
#include <stdlib.h>
#ifdef MAC
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif
const int WIDTH = 500;
const int HEIGHT = 500;
float depthValues[WIDTH][HEIGHT];

float x_angle = 0.0f;
float y_angle = 0.0f;
float z_angle = 0.0f;

void init() {
    glClearColor(0.0, 0.0, 0.0, 1.0);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0f, (GLfloat)WIDTH / (GLfloat)HEIGHT, 0.1f, 100.0f);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void readDepthData() {
    std::ifstream file("penny-depth.txt");
    if (file.is_open()) {
        for (int i = 0; i < WIDTH; ++i) {
            for (int j = 0; j < HEIGHT; ++j) {
                file >> depthValues[i][j];
                // Scale depth values here if needed
            }
        }
        file.close();
    } else {
        std::cerr << "Unable to open file." << std::endl;
    }
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    glTranslatef(0.0f, 0.0f, -5.0f);
    glRotatef(x_angle, 1.0f, 0.0f, 0.0f);
    glRotatef(y_angle, 0.0f, 1.0f, 0.0f);
    glRotatef(z_angle, 0.0f, 0.0f, 1.0f);

    glBegin(GL_QUADS);
    for (int i = 0; i < WIDTH - 1; ++i) {
        for (int j = 0; j < HEIGHT - 1; ++j) {
            glVertex3f(i, j, depthValues[i][j]);
            glVertex3f(i + 1, j, depthValues[i + 1][j]);
            glVertex3f(i + 1, j + 1, depthValues[i + 1][j + 1]);
            glVertex3f(i, j + 1, depthValues[i][j + 1]);
        }
    }
    glEnd();

    glutSwapBuffers();
}

void keyboard(unsigned char key, int x, int y) {
    switch (key) {
        case 'x':
            x_angle += 5.0f;
            break;
        case 'X':
            x_angle -= 5.0f;
            break;
        case 'y':
            y_angle += 5.0f;
            break;
        case 'Y':
            y_angle -= 5.0f;
            break;
        case 'z':
            z_angle += 5.0f;
            break;
        case 'Z':
            z_angle -= 5.0f;
            break;
        default:
            break;
    }
    glutPostRedisplay();
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(WIDTH, HEIGHT);
    glutCreateWindow("Penny Model");

    init();
    readDepthData();

    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard); // Register keyboard callback

    glEnable(GL_DEPTH_TEST);

    glutMainLoop();
    return 0;
}
