#include <GL/glut.h>
#include <iostream>
#include <vector>

struct Point {
    int x, y;
};

std::vector<Point> flightPath;
float objectPositionX = 0.0f;
float objectPositionY = 0.0f;
bool drawingPath = false;
int windowWidth = 800;
int windowHeight = 600;
float animationDuration = 5.0f;
float animationStartTime = 0.0f;

void drawObject() {
    glColor3f(1.0f, 0.0f, 0.0f); // Red color
    glBegin(GL_POLYGON);
    glVertex2f(objectPositionX - 10, objectPositionY - 10);
    glVertex2f(objectPositionX + 10, objectPositionY - 10);
    glVertex2f(objectPositionX, objectPositionY + 15);
    glEnd();
}

void drawFlightPath() {
    glColor3f(0.0f, 0.0f, 1.0f); // Blue color
    glBegin(GL_LINE_STRIP);
    for (const Point& point : flightPath) {
        glVertex2f(point.x, windowHeight - point.y);
    }
    glEnd();
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT);

    drawObject();

    if (drawingPath) {
        drawFlightPath();
    }

    glutSwapBuffers();
}

void reshape(int width, int height) {
    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, width, 0, height);
    glMatrixMode(GL_MODELVIEW);

    windowWidth = width;
    windowHeight = height;
}

void mouse(int button, int state, int x, int y) {
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
        if (!drawingPath) {
            // Start drawing the flight path
            drawingPath = true;
            flightPath.clear();
            objectPositionX = x;
            objectPositionY = windowHeight - y;
            flightPath.push_back({x, y});
        } else {
            // End drawing the flight path
            drawingPath = false;
        }
        glutPostRedisplay();
    }
}

void mouseMotion(int x, int y) {
    if (drawingPath) {
        flightPath.push_back({x, y});
        glutPostRedisplay();
    }
}

void animateObject() {
    if (!drawingPath) {
        // If not drawing, return
        return;
    }

    float currentTime = glutGet(GLUT_ELAPSED_TIME) / 1000.0f; // Convert to seconds
    float elapsedTime = currentTime - animationStartTime;

    if (elapsedTime < animationDuration) {
        float t = elapsedTime / animationDuration;

        // Interpolate object position along the flight path
        size_t pathSize = flightPath.size();
        size_t index = static_cast<size_t>(t * (pathSize - 1));
        float tInSegment = t * (pathSize - 1) - index;

        objectPositionX = (1 - tInSegment) * flightPath[index].x + tInSegment * flightPath[index + 1].x;
        objectPositionY = windowHeight - ((1 - tInSegment) * flightPath[index].y + tInSegment * flightPath[index + 1].y);

        glutPostRedisplay();
    } else {
        // Animation completed, reset object position
        objectPositionX = flightPath.back().x;
        objectPositionY = windowHeight - flightPath.back().y;
    }
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutCreateWindow("Object Flight Animation");
    glutReshapeWindow(800, 600);

    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutMouseFunc(mouse);
    glutMotionFunc(mouseMotion);
    glutIdleFunc(animateObject);

    glutMainLoop();
    return 0;
}
