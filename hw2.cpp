#include <GL/glut.h>
#include <iostream>

// Triangle properties
GLfloat triangleVertices[] = { 0.0f, 0.0f,  -25.0f, 50.0f,  25.0f, 50.0f };
bool followMouse = false;
float mouseX, mouseY;

// Animation properties
float animationStartTime;
float animationDuration = 5.0f;

void drawTriangle() {
    glColor3f(1.0f, 0.0f, 0.0f); // Red color
    glBegin(GL_TRIANGLES);
    for (int i = 0; i < 6; i += 2) {
        glVertex2f(triangleVertices[i], triangleVertices[i + 1]);
    }
    glEnd();
}

void animateTriangle() {
    float currentTime = glutGet(GLUT_ELAPSED_TIME) / 1000.0f;
    float elapsedTime = currentTime - animationStartTime;

    if (followMouse) {
        // If the mouse button is held down, update the triangle position
        triangleVertices[0] = mouseX;
        triangleVertices[1] = mouseY;
    } else if (elapsedTime <= animationDuration) {
        // If the mouse button is released, animate the triangle for a few seconds
        float t = elapsedTime / animationDuration;
        float x = (1 - t) * mouseX + t * 0.0f;
        float y = (1 - t) * mouseY + t * 0.0f;

        // Update the triangle position
        triangleVertices[0] = x;
        triangleVertices[1] = y;
    }
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT);

    drawTriangle();
    animateTriangle();

    glutSwapBuffers();
}

void mouse(int button, int state, int x, int y) {
    if (button == GLUT_LEFT_BUTTON) {
        if (state == GLUT_DOWN) {
            // When the mouse button is pressed, start following the mouse
            followMouse = true;
        } else if (state == GLUT_UP) {
            // When the mouse button is released, stop following the mouse and start the animation
            followMouse = false;
            animationStartTime = glutGet(GLUT_ELAPSED_TIME) / 1000.0f;
        }
    }

    // Convert screen coordinates to OpenGL coordinates
    mouseX = static_cast<float>(x) / glutGet(GLUT_WINDOW_WIDTH) * 2.0f - 1.0f;
    mouseY = 1.0f - static_cast<float>(y) / glutGet(GLUT_WINDOW_HEIGHT) * 2.0f;

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
    glutCreateWindow("Triangle Follows Mouse");
    glutReshapeWindow(800, 600);

    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

    glutDisplayFunc(display);
    glutMouseFunc(mouse);
    glutReshapeFunc(reshape);

    glutMainLoop();
    return 0;
}
