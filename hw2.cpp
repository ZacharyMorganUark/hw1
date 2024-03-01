#include <GL/glut.h>
#include <iostream>
#include <vector>

// Triangle properties
GLfloat triangleVertices[] = { 0.0f, 0.0f, -25.0f, 50.0f, 25.0f, 50.0f };

// Line properties
std::vector<std::pair<float, float>> linePoints;
bool drawingLine = false;

void drawTriangle() {
    glColor3f(1.0f, 0.0f, 0.0f); // Red color
    glBegin(GL_TRIANGLES);
    for (int i = 0; i < 6; i += 2) {
        glVertex2f(triangleVertices[i], triangleVertices[i + 1]);
    }
    glEnd();
}

void drawFreeformLine() {
    if (!linePoints.empty()) {
        glColor3f(0.0f, 0.0f, 1.0f); // Blue color
        glBegin(GL_LINE_STRIP);
        for (const auto& point : linePoints) {
            glVertex2f(point.first, point.second);
        }
        glEnd();
    }
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT);

    drawTriangle();

    if (drawingLine) {
        drawFreeformLine();
    }

    glutSwapBuffers();
}

void mouse(int button, int state, int x, int y) {
    if (button == GLUT_LEFT_BUTTON) {
        if (state == GLUT_DOWN) {
            // When the left mouse button is pressed, start drawing the freeform line
            linePoints.clear();
            drawingLine = true;
        } else if (state == GLUT_UP) {
            // When the left mouse button is released, end drawing the freeform line
            drawingLine = false;
        }
    }

    // Convert screen coordinates to OpenGL coordinates
    float mouseX = static_cast<float>(x) / glutGet(GLUT_WINDOW_WIDTH) * 2.0f - 1.0f;
    float mouseY = 1.0f - static_cast<float>(y) / glutGet(GLUT_WINDOW_HEIGHT) * 2.0f;

    // Add the current mouse position to the line points
    if (drawingLine) {
        linePoints.push_back(std::make_pair(mouseX, mouseY));
    }

    glutPostRedisplay();
}

void reshape(int width, int height) {
    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(-1.0, 1.0, -1.0, 1.0);
    glMatrixMode(GL_MODELVIEW);
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutCreateWindow("Draw Triangle and Freeform Line");
    glutReshapeWindow(800, 600);

    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

    glutDisplayFunc(display);
    glutMouseFunc(mouse);
    glutReshapeFunc(reshape);

    glutMainLoop();
    return 0;
}
