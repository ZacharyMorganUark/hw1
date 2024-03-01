#include <cmath>
#include <ctime>
#include <cstdio>
#include <cstdlib>

#ifdef MAC
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

//4545

const int MAX_VERTICES = 4;
const int LINE_COUNT = 100;
const int MOVEMENT_DURATION = 5000;

float squareVertices[MAX_VERTICES][2] = {
    {-0.1, -0.1},
    {0.1, -0.1},
    {0.1, 0.1},
    {-0.1, 0.1}
};

float pathVertices[MAX_VERTICES * LINE_COUNT][2];
int vertexCount = 0;
bool drawing = false;
std::clock_t moveStartTime;

void drawSquare() {
    glBegin(GL_POLYGON);
    for (int i = 0; i < MAX_VERTICES; ++i) {
        glVertex2f(squareVertices[i][0], squareVertices[i][1]);
    }
    glEnd();
}

void drawPath() {
    glBegin(GL_LINE_STRIP);
    for (int i = 0; i < vertexCount; ++i) {
        glVertex2f(pathVertices[i][0], pathVertices[i][1]);
    }
    glEnd();
}

void mouseClick(int button, int state, int x, int y) {
    if (button == GLUT_LEFT_BUTTON) {
        if (state == GLUT_DOWN) {
            drawing = true;
            vertexCount = 0;
        } else if (state == GLUT_UP) {
            drawing = false;
            moveStartTime = std::clock();  // Start animation
        }
    }
}

void mouseMotion(int x, int y) {
    if (drawing && vertexCount < MAX_VERTICES * LINE_COUNT) {
        float xCoord = (float)x / glutGet(GLUT_WINDOW_WIDTH) * 2 - 1;
        float yCoord = 1 - (float)y / glutGet(GLUT_WINDOW_HEIGHT) * 2;
        pathVertices[vertexCount][0] = xCoord;
        pathVertices[vertexCount][1] = yCoord;
        ++vertexCount;
        glutPostRedisplay();
    }
}

void moveSquare() {
    if (vertexCount > 1) {
        std::clock_t currentTime = std::clock();
        double elapsedTime = (double)(currentTime - moveStartTime) / CLOCKS_PER_SEC;

        float t = fmin(1.0, elapsedTime / (MOVEMENT_DURATION / 1000.0));
        float currentX = (1 - t) * pathVertices[0][0] + t * pathVertices[vertexCount - 1][0];
        float currentY = (1 - t) * pathVertices[0][1] + t * pathVertices[vertexCount - 1][1];

        glPushMatrix();
        glTranslatef(currentX, currentY, 0.0);
        drawSquare();
        glPopMatrix();
    }
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT);
    glColor3f(1.0, 1.0, 1.0);

    if (drawing) {
        drawPath();
    } else {
        drawSquare();
        moveSquare();
    }

    glutSwapBuffers();
}

void init() {
    glClearColor(0.0, 0.0, 0.0, 1.0); // Set clear color to black
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(-1.0, 1.0, -1.0, 1.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

int main(int argc, char *argv[]) {
    glutInit(&argc, argv);
    glutInitWindowSize(500, 500);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutCreateWindow("Drawing and Moving Square");
    glutDisplayFunc(display);
    glutMouseFunc(mouseClick);
    glutMotionFunc(mouseMotion);

    init();
    glutMainLoop();

    return 0;
}
