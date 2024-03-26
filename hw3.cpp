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
float R[WIDTH][HEIGHT], G[WIDTH][HEIGHT], B[WIDTH][HEIGHT];

float x_angle = 0.0f;
float y_angle = 0.0f;
float z_angle = 0.0f;

bool useColorDisplay = false;

void init() {
    glClearColor(0.0, 0.0, 0.0, 1.0);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glEnable(GL_DEPTH_TEST);
    gluPerspective(45.0f, (GLfloat)WIDTH / (GLfloat)HEIGHT, 0.1f, 100.0f);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void readData() {
    // Read depth values
    std::ifstream depthFile("penny-depth.txt");
    if (depthFile.is_open()) {
        for (int i = 0; i < WIDTH; ++i) {
            for (int j = 0; j < HEIGHT; ++j) {
                depthFile >> depthValues[i][j];
                depthValues[i][j] *= 0.001; // Scale down depth values if needed
            }
        }
        depthFile.close();
        std::cout << "Depth data loaded successfully." << std::endl;
    } else {
        std::cerr << "Unable to open depth file." << std::endl;
    }

    // Read color values
    std::ifstream colorFile("penny-image.txt");
    if (colorFile.is_open()) {
        for (int i = 0; i < WIDTH; ++i) {
            for (int j = 0; j < HEIGHT; ++j) {
                int r, g, b;
                colorFile >> r >> g >> b;
                R[i][j] = r / 255.0; // Scale RGB values to [0, 1]
                G[i][j] = g / 255.0;
                B[i][j] = b / 255.0;
            }
        }
        colorFile.close();
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

    if (useColorDisplay) {
        for (int i = 0; i < WIDTH - 1; ++i) {
            for (int j = 0; j < HEIGHT - 1; ++j) {
                glBegin(GL_POLYGON);
                glColor3f(R[i][j], G[i][j], B[i][j]); // Set color using RGB values
                glVertex3f(i, j, depthValues[i][j]);
                glVertex3f(i + 1, j, depthValues[i + 1][j]);
                glVertex3f(i + 1, j + 1, depthValues[i + 1][j + 1]);
                glVertex3f(i, j + 1, depthValues[i][j + 1]);
                glEnd();
            }
        }
    } else {
        for (int i = 0; i < WIDTH; ++i) {
            for (int j = 0; j < HEIGHT; ++j) {
                glBegin(GL_LINE_LOOP);
                glVertex3f(i, j, depthValues[i][j]);
                glVertex3f(i + 1, j, depthValues[i + 1][j]);
                glVertex3f(i + 1, j + 1, depthValues[i + 1][j + 1]);
                glVertex3f(i, j + 1, depthValues[i][j + 1]);
                glEnd();
            }
        }
    }

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
        case '2':
            useColorDisplay = !useColorDisplay;
            glutPostRedisplay(); // Redraw with color or depth display
            break;
        default:
            break;
    }
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(WIDTH, HEIGHT);
    glutCreateWindow("Penny Model");

    init();
    readData();

    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);

    glEnable(GL_DEPTH_TEST);

    glutMainLoop();
    return 0;
}
