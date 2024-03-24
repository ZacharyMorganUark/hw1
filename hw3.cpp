#include <GL/glut.h>
#include <fstream>

const int WIDTH = 500;
const int HEIGHT = 500;
float depthValues[WIDTH][HEIGHT];

void init() {
    glClearColor(0.0, 0.0, 0.0, 1.0);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, WIDTH, 0, HEIGHT, -1, 1);
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
    glClear(GL_COLOR_BUFFER_BIT);
    glLoadIdentity();

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

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(WIDTH, HEIGHT);
    glutCreateWindow("Penny Model");

    init();
    readDepthData();

    glutDisplayFunc(display);
    glutMainLoop();
    return 0;
}
