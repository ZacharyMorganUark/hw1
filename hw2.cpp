#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#ifdef MAC
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif
//the

#define MIN_X_VIEW -50
#define MAX_X_VIEW 50
#define MIN_Y_VIEW -50
#define MAX_Y_VIEW 50
#define MIN_Z_VIEW -50
#define MAX_Z_VIEW 50
#define MIN_X_SCREEN 0
#define MAX_X_SCREEN 500
#define MIN_Y_SCREEN 0
#define MAX_Y_SCREEN 500
#define LINE_COUNT 1000

int xangle = 10;
int yangle = 15;
int mode = 2;
int count = 0;
float point[LINE_COUNT][4];
int curveResolution = 100;

void init()
{
    glClearColor(0.0, 0.0, 0.0, 1.0);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(MIN_X_VIEW, MAX_X_VIEW, MIN_Y_VIEW, MAX_Y_VIEW, MIN_Z_VIEW, MAX_Z_VIEW);
    glEnable(GL_DEPTH_TEST);
}

void drawBezierCurve(float lineStartX, float lineStartY, float lineEndX, float lineEndY, float controlPointX, float controlPointY)
{
    glColor3f(1.0f, 0.0f, 0.0f); // Red color
    glBegin(GL_LINE_STRIP);
    for (int i = 0; i <= curveResolution; ++i)
    {
        float t = static_cast<float>(i) / curveResolution;
        float x = (1 - t) * (1 - t) * lineStartX + 2 * (1 - t) * t * controlPointX + t * t * lineEndX;
        float y = (1 - t) * (1 - t) * lineStartY + 2 * (1 - t) * t * controlPointY + t * t * lineEndY;
        glVertex3f(x, y, 0.0);
    }
    glEnd();
}

void mouse(int button, int state, int x, int y)
{
    if (mode != 2)
        return;

    float x_scale = (MAX_X_VIEW - MIN_X_VIEW) / (float)(MAX_X_SCREEN - MIN_X_SCREEN);
    float y_scale = (MIN_Y_VIEW - MAX_Y_VIEW) / (float)(MAX_Y_SCREEN - MIN_Y_SCREEN);

    if (state == GLUT_DOWN)
    {
        point[count][0] = MIN_X_VIEW + (x - MIN_X_SCREEN) * x_scale;
        point[count][1] = MAX_Y_VIEW + (y - MIN_Y_SCREEN) * y_scale;
    }
    else if (state == GLUT_UP)
    {
        point[count][2] = MIN_X_VIEW + (x - MIN_X_SCREEN) * x_scale;
        point[count][3] = MAX_Y_VIEW + (y - MIN_Y_SCREEN) * y_scale;
        count++;
        glutPostRedisplay();
    }
}

void motion(int x, int y)
{
    if (mode != 2)
        return;

    float x_scale = (MAX_X_VIEW - MIN_X_VIEW) / (float)(MAX_X_SCREEN - MIN_X_SCREEN);
    float y_scale = (MIN_Y_VIEW - MAX_Y_VIEW) / (float)(MAX_Y_SCREEN - MIN_Y_SCREEN);

    point[count][2] = MIN_X_VIEW + (x - MIN_X_SCREEN) * x_scale;
    point[count][3] = MAX_Y_VIEW + (y - MIN_Y_SCREEN) * y_scale;
    glutPostRedisplay();
}

void display()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    if (mode == 3)
    {
        glRotatef(xangle, 1.0, 0.0, 0.0);
        glRotatef(yangle, 0.0, 1.0, 0.0);
    }

    for (int i = 0; i < count; i++)
    {
        if ((point[i][0] != point[i][2]) || (point[i][1] != point[i][3]))
        {
            drawBezierCurve(point[i][0], point[i][1], point[i][2], point[i][3],
                            (point[i][0] + point[i][2]) / 2, (point[i][1] + point[i][3]) / 2);
        }
    }

    glutSwapBuffers();
}

int main(int argc, char *argv[])
{
    glutInit(&argc, argv);
    glutInitWindowSize(MAX_Y_SCREEN, MAX_X_SCREEN);
    glutInitWindowPosition(MAX_Y_SCREEN / 2, MAX_X_SCREEN / 2);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
    glutCreateWindow("Building");
    glutDisplayFunc(display);
    glutMouseFunc(mouse);
    glutMotionFunc(motion);
    init();

    printf("Mouse operations:\n");
    printf("   'mouse down' - start drawing line\n");
    printf("   'mouse motion' - draw curve\n");
    printf("   'mouse up' - finish drawing line\n");
    glutMainLoop();
    return 0;
}
