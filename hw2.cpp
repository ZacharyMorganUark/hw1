#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#ifdef MAC
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

const int MAX_VERTICES = 100;

// Structure to represent a point
struct Point {
    float x, y;
};

Point vertices[MAX_VERTICES]; // Array to store the vertices of the polygon
int vertexCount = 0;          // Counter to keep track of the number of vertices
bool drawing = false;         // Flag to indicate whether drawing is in progress

// Function to draw a polygon
void drawPolygon() {
    glBegin(GL_POLYGON);
    for (int i = 0; i < vertexCount; ++i) {
        glVertex2f(vertices[i].x, vertices[i].y);
    }
    glEnd();
}

// Function to handle mouse click events
void mouseClick(int button, int state, int x, int y) {
    if (button == GLUT_LEFT_BUTTON) {
        if (state == GLUT_DOWN) {
            // Start drawing when the left mouse button is pressed
            drawing = true;
            vertexCount = 0;
        } else if (state == GLUT_UP) {
            // Stop drawing when the left mouse button is released
            drawing = false;
        }
    }
}

// Function to handle mouse motion events
void mouseMotion(int x, int y) {
    if (drawing && vertexCount < MAX_VERTICES) {
        // Record the mouse position while drawing
        vertices[vertexCount].x = (float)x / glutGet(GLUT_WINDOW_WIDTH) * 2 - 1;
        vertices[vertexCount].y = 1 - (float)y / glutGet(GLUT_WINDOW_HEIGHT) * 2;
        ++vertexCount;
        glutPostRedisplay(); // Trigger a redraw to update the display
    }
}

// Function to animate the polygon along the recorded path
void movePolygon() {
    // Animation logic goes here
}

// Function to display the scene
void display() {
    glClear(GL_COLOR_BUFFER_BIT);
    glColor3f(1.0, 2.0, 1.0); // Set color to white

    if (drawing) {
        drawPolygon();
    } else {
        movePolygon();
    }

    glutSwapBuffers();
}

// Function to initialize OpenGL settings
void init() {
    glClearColor(0.0, 0.0, 0.0, 1.0); // Set clear color to black
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(-1.0, 1.0, -1.0, 1.0); // Set up an orthographic view
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

// Main function
int main(int argc, char *argv[]) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutCreateWindow("Homework 2");

    // Register callback functions
    glutDisplayFunc(display);
    glutMouseFunc(mouseClick);
    glutMotionFunc(mouseMotion);

    init();

    // Enter the GLUT event loop
    glutMainLoop();

    return 0;
}
