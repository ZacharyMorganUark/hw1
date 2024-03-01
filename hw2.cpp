#include <GL/glut.h>
#include <vector>

// Structure to represent a point
struct Point {
    float x, y;
};

std::vector<Point> flightPath;
bool drawing = false;

// Function to draw a polygon (rectangle in this example)
void drawObject() {
    glColor3f(1.0, 0.0, 0.0);  // Red color
    glBegin(GL_POLYGON);
    glVertex2f(-0.1, -0.1);
    glVertex2f(0.1, -0.1);
    glVertex2f(0.1, 0.1);
    glVertex2f(-0.1, 0.1);
    glEnd();
}

// Function to convert screen coordinates to object coordinates
Point screenToObject(int x, int y) {
    Point objectPoint;
    objectPoint.x = (float)x / glutGet(GLUT_WINDOW_WIDTH) * 2 - 1;
    objectPoint.y = 1 - (float)y / glutGet(GLUT_WINDOW_HEIGHT) * 2;
    return objectPoint;
}

// Function to handle mouse click events
void mouseClick(int button, int state, int x, int y) {
    if (button == GLUT_LEFT_BUTTON) {
        if (state == GLUT_DOWN) {
            // Start capturing the flight path when the left mouse button is pressed
            flightPath.clear();
            drawing = true;
        } else if (state == GLUT_UP) {
            // Stop capturing the flight path when the left mouse button is released
            drawing = false;
        }
    }
}

// Function to handle mouse motion events
void mouseMotion(int x, int y) {
    if (drawing) {
        // Record the mouse position while drawing
        Point objectPoint = screenToObject(x, y);
        flightPath.push_back(objectPoint);
        glutPostRedisplay();  // Trigger a redraw to update the display
    }
}

// Function to animate the object along the captured path
void animateObject() {
    static int currentPointIndex = 0;
    if (currentPointIndex < flightPath.size()) {
        Point currentPoint = flightPath[currentPointIndex];
        glPushMatrix();
        glTranslatef(currentPoint.x, currentPoint.y, 0.0);
        drawObject();
        glPopMatrix();

        currentPointIndex++;
    }
}

// Function to display the scene
void display() {
    glClear(GL_COLOR_BUFFER_BIT);
    glColor3f(0.0, 0.0, 1.0);  // Blue color

    if (drawing) {
        drawObject();  // Draw the object during drawing phase
    } else {
        animateObject();  // Animate the object along the captured path
    }

    glutSwapBuffers();
}

// Function to initialize OpenGL settings
void init() {
    glClearColor(1.0, 1.0, 1.0, 1.0);  // Set clear color to white
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(-1.0, 1.0, -1.0, 1.0);  // Set up an orthographic view
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

// Main function
int main(int argc, char *argv[]) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutCreateWindow("Flying Object");

    // Register callback functions
    glutDisplayFunc(display);
    glutMouseFunc(mouseClick);
    glutMotionFunc(mouseMotion);

    init();

    // Enter the GLUT event loop
    glutMainLoop();

    return 0;
}
