#include <GL/glut.h>
#include <cmath>
//ggggggggggg
#define LINE_COUNT 1000

int curveResolution = 100;
int currentCurveIndex = -1;
float lineStartX, lineStartY, lineEndX, lineEndY, controlPointX, controlPointY;
float points[LINE_COUNT][3]; // 3 values for each point (x, y, isControlPoint)

void drawBezierCurve(float startX, float startY, float endX, float endY, float controlX, float controlY) {
    glColor3f(1.0f, 0.0f, 0.0f); // Red color
    glBegin(GL_LINE_STRIP);
    for (int i = 0; i <= curveResolution; ++i) {
        float t = static_cast<float>(i) / curveResolution;
        float x = (1 - t) * (1 - t) * startX + 2 * (1 - t) * t * controlX + t * t * endX;
        float y = (1 - t) * (1 - t) * startY + 2 * (1 - t) * t * controlY + t * t * endY;
        glVertex2f(x, y);
    }
    glEnd();
}

void mouse(int button, int state, int x, int y) {
    float x_scale = 2.0 / glutGet(GLUT_WINDOW_WIDTH);
    float y_scale = -2.0 / glutGet(GLUT_WINDOW_HEIGHT);

    if (button == GLUT_LEFT_BUTTON) {
        if (state == GLUT_DOWN) {
            lineStartX = x * x_scale - 1.0;
            lineStartY = y * y_scale + 1.0;
            controlPointX = lineStartX + 0.2; // Adjust as needed
            controlPointY = lineStartY + 0.2; // Adjust as needed
            currentCurveIndex++;
            points[currentCurveIndex][0] = lineStartX;
            points[currentCurveIndex][1] = lineStartY;
            points[currentCurveIndex][2] = 1; // 1 indicates a control point
        } else if (state == GLUT_UP) {
            lineEndX = x * x_scale - 1.0;
            lineEndY = y * y_scale + 1.0;
            currentCurveIndex++;
            points[currentCurveIndex][0] = controlPointX; // Store control point
            points[currentCurveIndex][1] = controlPointY;
            points[currentCurveIndex][2] = 1; // 1 indicates a control point
            currentCurveIndex++;
            points[currentCurveIndex][0] = lineEndX; // Store end point
            points[currentCurveIndex][1] = lineEndY;
            points[currentCurveIndex][2] = 0; // 0 indicates an end point
            glutPostRedisplay();
        }
    }
}

void motion(int x, int y) {
    float x_scale = 2.0 / glutGet(GLUT_WINDOW_WIDTH);
    float y_scale = -2.0 / glutGet(GLUT_WINDOW_HEIGHT);

    controlPointX = x * x_scale - 1.0;
    controlPointY = y * y_scale + 1.0;

    points[currentCurveIndex][0] = controlPointX;
    points[currentCurveIndex][1] = controlPointY;
    points[currentCurveIndex][2] = 1; // 1 indicates a control point

    glutPostRedisplay();
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT);

    for (int i = 0; i < currentCurveIndex; i += 3) {
        // Draw Bezier curve
        drawBezierCurve(points[i][0], points[i][1], points[i + 1][0], points[i + 1][1],
                        points[i + 2][0], points[i + 2][1]);
    }

    glPointSize(5.0);
    glBegin(GL_POINTS);
    glColor3f(0.0f, 1.0f, 0.0f); // Green color
    for (int i = 0; i <= currentCurveIndex; ++i) {
        glVertex2f(points[i][0], points[i][1]);
    }
    glEnd();
    glPointSize(1.0);

    glutSwapBuffers();
}

void init() {
    glClearColor(0.0, 0.0, 0.0, 1.0);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(-1.0, 1.0, -1.0, 1.0);
}

int main(int argc, char *argv[]) {
    glutInit(&argc, argv);
    glutInitWindowSize(500, 500);
    glutCreateWindow("car 9");
    glutDisplayFunc(display);
    glutMouseFunc(mouse);
    glutMotionFunc(motion);
    init();
    glutMainLoop();
    return 0;
}
