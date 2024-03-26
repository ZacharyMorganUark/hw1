#include <iostream>
#include <fstream>
#include <cmath>
#ifdef MAC
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

const int WIDTH = 500;
const int HEIGHT = 500;
float depthValues[WIDTH][HEIGHT];
int R[WIDTH][HEIGHT], G[WIDTH][HEIGHT], B[WIDTH][HEIGHT];

float x_angle = 0.0f;
float y_angle = 0.0f;
float z_angle = 0.0f;

void init() {
    glClearColor(0.0, 0.0, 0.0, 1.0);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glEnable(GL_DEPTH_TEST);
    gluPerspective(45.0f, (GLfloat)WIDTH / (GLfloat)HEIGHT, 0.1f, 100.0f);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void readDepthData() {
    std::ifstream depth_file("penny-depth.txt");
    if (depth_file.is_open()) {
        for (int i = 0; i < WIDTH; ++i) {
            for (int j = 0; j < HEIGHT; ++j) {
                depth_file >> depthValues[i][j];
                depthValues[i][j] *= 0.001; // Scale down depth values if necessary
            }
        }
        depth_file.close();
        std::cout << "Depth data loaded successfully." << std::endl;
    } else {
        std::cerr << "Unable to open depth file." << std::endl;
    }
}

void readColorData() {
    std::ifstream color_file("penny-image.txt");
    if (color_file.is_open()) {
        for (int i = 0; i < WIDTH; ++i) {
            for (int j = 0; j < HEIGHT; ++j) {
                color_file >> R[i][j] >> G[i][j] >> B[i][j];
                // Scale RGB values if necessary
                R[i][j] = (int)(R[i][j] / 255.0f * 1000);
                G[i][j] = (int)(G[i][j] / 255.0f * 1000);
                B[i][j] = (int)(B[i][j] / 255.0f * 1000);
            }
        }
        color_file.close();
        std::cout << "Color data loaded successfully." << std::endl;
    } else {
        std::cerr << "Unable to open color file." << std::endl;
    }
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    glTranslatef(0.0f, 0.0f, -10.0f); // Increased translation along z-axis for zooming out
    glRotatef(x_angle, 1.0f, 0.0f, 0.0f);
    glRotatef(y_angle, 0.0f, 1.0f, 0.0f);
    glRotatef(z_angle, 0.0f, 0.0f, 1.0f);

    // Loop over polygons and display them with GL_LINE_LOOP
    glColor3f(1.0, 1.0, 1.0);
    glBegin(GL_LINE_LOOP);
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
    readColorData(); // Load color data on startup

    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);

    glEnable(GL_DEPTH_TEST);

    glutMainLoop();
    return 0;
}
