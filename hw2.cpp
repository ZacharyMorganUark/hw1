#include <GL/glut.h>
#include <cmath>
//sammy
int windowWidth = 500;
int windowHeight = 500;
int pointCount = 0;
float points[500][2]; // Array to store points
int curveResolution = 100;
bool isDrawing = false;
bool moveSquare = false;
float redSquarePosition[2] = {10.0f, 10.0f}; // Initial position of the red square

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

void drawRedSquare() {
    glColor3f(1.0, 0.0, 0.0);
    glBegin(GL_QUADS);
    glVertex2f(redSquarePosition[0], redSquarePosition[1]);
    glVertex2f(redSquarePosition[0] + 20.0f, redSquarePosition[1]);
    glVertex2f(redSquarePosition[0] + 20.0f, redSquarePosition[1] + 20.0f);
    glVertex2f(redSquarePosition[0], redSquarePosition[1] + 20.0f);
    glEnd();
}

void moveSquareAlongPath() {
    static int currentPoint = 0;
    static float speed = 2.0f; // Adjust the speed of the square

    if (currentPoint < pointCount) {
        float dx = points[currentPoint][0] - redSquarePosition[0];
        float dy = points[currentPoint][1] - redSquarePosition[1];
        float distance = std::sqrt(dx * dx + dy * dy);

        if (distance > speed) {
            float ratio = speed / distance;
            redSquarePosition[0] += ratio * dx;
            redSquarePosition[1] += ratio * dy;
        } else {
            // Move to the next point
            redSquarePosition[0] = points[currentPoint][0];
            redSquarePosition[1] = points[currentPoint][1];
            currentPoint++;
        }
    }
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT);
    drawLineWithCurves();

    if (moveSquare) {
        moveSquareAlongPath();
        drawRedSquare();
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
    if (key == 27) { // ASCII code for the 'Esc' key
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
    glutCreateWindow("Tacos");

    glutDisplayFunc(display);
    glutMouseFunc(mouse);
    glutMotionFunc(motion);
    glutKeyboardFunc(keyboard); // Register keyboard callback

    init();

    glutMainLoop();
    return 0;
}
