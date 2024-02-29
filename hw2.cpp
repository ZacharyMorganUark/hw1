#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctime>
#ifdef MAC
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

const int MAX_VERTICES = 100;
const int MOVEMENT_DURATION = 5000; // Animation duration in milliseconds
clock_t moveStartTime;              // Start time of the animation

// Structure to represent a point
struct Point {
    float x, y;
};

Point vertices[MAX_VERTICES]; // Array to store the vertices of the path
int vertexCount = 0;          // Counter to keep track of the number of vertices
bool drawing = false;         // Flag to indicate whether drawing is in progress

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
            moveStartTime = clock(); // Record the start time for animation
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
    }
}

// Function to animate the polygon along the recorded path
void movePolygon() {
    if (vertexCount > 1) {
        // Calculate elapsed time since the animation started
        clock_t currentTime = clock();
        double elapsedTime = (double)(currentTime - moveStartTime) / CLOCKS_PER_SEC;

        // Speed factor controls the pace of the movement
        float speedFactor = 0.2; // Adjust this value to control the speed

        // Calculate the current position along the path based on elapsed time and speed factor
        float t = fmin(1.0, elapsedTime / (MOVEMENT_DURATION / 1000.0 * speedFactor));
        float currentX = (1 - t) * vertices[0].x + t * vertices[vertexCount - 1].x;
        float currentY = (1 - t) * vertices[0].y + t * vertices[vertexCount - 1].y;

        // Draw the polygon at the current position
        glBegin(GL_POLYGON);
        glVertex2f(currentX - 0.05, currentY - 0.05);
        glVertex2f(currentX + 0.05, currentY - 0.05);
        glVertex2f(currentX + 0.05, currentY + 0.05);
        glVertex2f(currentX - 0.05, currentY + 0.05);
        glEnd();

        // Check if the animation is complete
        if (t >= 1.0) {
            drawing = false; // Stop drawing when animation is complete
        }
    }
}

// Function to display the scene
void display() {
    glClear(GL_COLOR_BUFFER_BIT);
    glColor3f(0.0, 0.0, 1.0); // Set color

    if (drawing) {
        // Draw the path being drawn by the user
        glBegin(GL_LINE_STRIP);
        for (int i = 0; i < vertexCount; ++i) {
            glVertex2f(vertices[i].x, vertices[i].y);
        }
        glEnd();
    } else {
        movePolygon();
    }

    glutSwapBuffers();
}

// Function to initialize OpenGL settings
void init() {
    glClearColor(1.0, 1.0, 1.0, 1.0); // Set clear color to white
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
