#include <GL/glut.h>
#include <cmath>
#include <chrono>
#include <thread>

GLfloat squareSize = 50.0f;
GLfloat squarePositionX = 0.0f;
GLfloat squarePositionY = 0.0f;
bool drawLine = false;
GLfloat lineStartX, lineStartY, controlPointX, controlPointY, lineEndX, lineEndY;
int curveResolution = 100; // Number of points to approximate the curve
const float animationDuration = 5.0f; // 5 seconds

// Timer variables
std::chrono::time_point<std::chrono::high_resolution_clock> animationStartTime;

void drawSquare() {
    glColor3f(0.0f, 0.0f, 1.0f); // Blue color
    glBegin(GL_QUADS);
    glVertex2f(squarePositionX, squarePositionY);
    glVertex2f(squarePositionX + squareSize, squarePositionY);
    glVertex2f(squarePositionX + squareSize, squarePositionY + squareSize);
    glVertex2f(squarePositionX, squarePositionY + squareSize);
    glEnd();
}

void drawBezierCurve() {
    glColor3f(1.0f, 0.0f, 0.0f); // Red color
    glBegin(GL_LINE_STRIP);
    for (int i = 0; i <= curveResolution; ++i) {
        float t = static_cast<float>(i) / curveResolution;
        float x = (1 - t) * (1 - t) * lineStartX + 2 * (1 - t) * t * controlPointX + t * t * lineEndX;
        float y = (1 - t) * (1 - t) * lineStartY + 2 * (1 - t) * t * controlPointY + t * t * lineEndY;
        glVertex2f(x, y);
    }
    glEnd();
}

void animateSquare() {
    auto currentTime = std::chrono::high_resolution_clock::now();
    float elapsedTime = std::chrono::duration<float>(currentTime - animationStartTime).count();

    if (elapsedTime < animationDuration) {
        float t = elapsedTime / animationDuration;
        squarePositionX = (1 - t) * lineStartX + t * lineEndX;
        squarePositionY = (1 - t) * lineStartY + t * lineEndY;
        glutPostRedisplay();
    } else {
        // Animation completed, reset square position
        squarePositionX = lineEndX;
        squarePositionY = lineEndY;
    }
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT);

    drawSquare();

    if (drawLine) {
        drawBezierCurve();
        animateSquare();
    }

    glutSwapBuffers();
}

void keyboard(unsigned char key, int x, int y) {
    if (key == 'M' || key == 'm') {
        // Move the square along the curve
        drawLine = false; // Stop drawing the curve
        animationStartTime = std::chrono::high_resolution_clock::now(); // Start the animation timer
        glutPostRedisplay();
    }
}

void mouse(int button, int state, int x, int y) {
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
        if (!drawLine) {
            // Record the starting point of the line
            lineStartX = x;
            lineStartY = glutGet(GLUT_WINDOW_HEIGHT) - y;
            drawLine = true;
        } else {
            // Record the control point and ending point of the curve
            controlPointX = x;
            controlPointY = glutGet(GLUT_WINDOW_HEIGHT) - y;
            drawLine = false;
            // Uncomment the following line if you want to reset the square position after drawing each curve
            // squarePositionX = squarePositionY = 0.0f;
        }
    }
    glutPostRedisplay();
}

void reshape(int width, int height) {
    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, width, 0, height);
    glMatrixMode(GL_MODELVIEW);
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutCreateWindow("OpenGL Square Animation");
    glutReshapeWindow(800, 600);

    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);
    glutMouseFunc(mouse);
    glutReshapeFunc(reshape);

    glutMainLoop();
    return 0;
}
