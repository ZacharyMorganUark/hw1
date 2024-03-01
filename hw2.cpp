#include <GL/glut.h>
#include <cmath>

GLfloat squareSize = 50.0f;
GLfloat squarePositionX = 0.0f;
GLfloat squarePositionY = 0.0f;
bool drawLine = false;
GLfloat lineStartX, lineStartY, lineEndX, lineEndY;

void drawSquare() {
    glColor3f(0.0f, 0.0f, 1.0f); // Blue color
    glBegin(GL_QUADS);
    glVertex2f(squarePositionX, squarePositionY);
    glVertex2f(squarePositionX + squareSize, squarePositionY);
    glVertex2f(squarePositionX + squareSize, squarePositionY + squareSize);
    glVertex2f(squarePositionX, squarePositionY + squareSize);
    glEnd();
}

void drawLineSegment(GLfloat startX, GLfloat startY, GLfloat endX, GLfloat endY) {
    glColor3f(1.0f, 0.0f, 0.0f); // Red color
    glBegin(GL_LINES);
    glVertex2f(startX, startY);
    glVertex2f(endX, endY);
    glEnd();
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT);

    drawSquare();

    if (drawLine) {
        drawLineSegment(lineStartX, lineStartY, lineEndX, lineEndY);
    }

    glutSwapBuffers();
}

void keyboard(unsigned char key, int x, int y) {
    if (key == 'M' || key == 'm') {
        // Move the square along the line
        squarePositionX = lineEndX;
        squarePositionY = lineEndY;
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
            // Record the ending point of the line
            lineEndX = x;
            lineEndY = glutGet(GLUT_WINDOW_HEIGHT) - y;
            drawLine = false;
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
    glutCreateWindow("OpenGL Square and Line");
    glutReshapeWindow(800, 600);

    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);
    glutMouseFunc(mouse);
    glutReshapeFunc(reshape);

    glutMainLoop();
    return 0;
}
