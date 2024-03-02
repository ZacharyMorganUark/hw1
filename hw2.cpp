#include <GL/glut.h>
//dogs
int windowWidth = 500;
int windowHeight = 500;
int pointCount = 0;
float points[500][2]; // Array to store points
int curveResolution = 100;

void init() {
    glClearColor(0.0, 0.0, 0.0, 1.0);
    glMatrixMode(GL_PROJECTION);
    gluOrtho2D(0, windowWidth, windowHeight, 0);
}

void drawLineWithCurves() {
    glColor3f(1.0, 1.0, 1.0);
    glBegin(GL_LINE_STRIP);
    for (int i = 0; i < pointCount - 1; ++i) {
        for (int j = 0; j <= curveResolution; ++j) {
            float t = static_cast<float>(j) / curveResolution;
            float x = (1 - t) * points[i][0] + t * points[i + 1][0];
            float y = (1 - t) * points[i][1] + t * points[i + 1][1];
            glVertex2f(x, y);
        }
    }
    glEnd();
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT);
    drawLineWithCurves();
    glutSwapBuffers();
}

void mouse(int button, int state, int x, int y) {
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
        points[pointCount][0] = x;
        points[pointCount][1] = y;
        ++pointCount;
        glutPostRedisplay();
    }
}

void motion(int x, int y) {
    if (pointCount > 0) {
        points[pointCount][0] = x;
        points[pointCount][1] = y;
        glutPostRedisplay();
    }
}

int main(int argc, char *argv[]) {
    glutInit(&argc, argv);
    glutInitWindowSize(windowWidth, windowHeight);
    glutInitWindowPosition(100, 100);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
    glutCreateWindow("Curved Lines");

    glutDisplayFunc(display);
    glutMouseFunc(mouse);
    glutMotionFunc(motion);

    init();

    glutMainLoop();
    return 0;
}
