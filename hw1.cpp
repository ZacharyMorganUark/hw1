//---------------------------------------
//hw1.cpp
//Zachary Morgan
//2-12-2024
//---------------------------------------
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#ifdef MAC
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

// Maximum number of fireworks
const int MIN_FIREWORKS = 5;
const int MAX_FIREWORKS = 10;

//---------------------------------------
// generate a random color
void set_random_color()
{
    glColor3f((rand() % 100) / 100.0, (rand() % 100) / 100.0, (rand() % 100) / 100.0);
}

//---------------------------------------
// draw random fireworks

void draw_fireworks()
{
    int numFireworks = rand() % (MAX_FIREWORKS - 4) + MIN_FIREWORKS; // Random number of fireworks [5..10]

    for (int i = 0; i < numFireworks; i++)
    {
        //float size = (rand() % 40) / 100.0 + 0.1; // Random size between 0.1 and 0.5
        float x = (rand() % 200) / 100.0 - 1.0;    // Random x between -1.0 and 1.0
        float y = (rand() % 200) / 100.0 - 1.0;    // Random y between -1.0 and 1.0

        set_random_color(); // Set a random color
        glBegin(GL_POINTS);
        glVertex2f(x, y);
        glEnd();

        glBegin(GL_LINES);
        int numLines = rand() % 41 + 30; // Random number of lines between 30-70?
        for (int j = 0; j < numLines; j++)
        {
            float angle = j * 10.0 * (M_PI / 180.0); // Convert degrees to radians
            float lineSize = (rand() % 40) / 100.0 + 0.1; // Random size between 0.1 and 0.5
            float dx = lineSize * cos(angle);
            float dy = lineSize * sin(angle);
            glVertex2f(x, y);
            glVertex2f(x + dx, y + dy);
        }
        glEnd();
    }
}

//---------------------------------------
// Display 

void display()
{
    glClear(GL_COLOR_BUFFER_BIT);
    draw_fireworks();
    glFlush();
}

//---------------------------------------
// Init function

void init()
{
    glClearColor(0.0, 0.0, 0.0, 1.0);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-1.0, 1.0, -1.0, 1.0, -1.0, 1.0);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

//---------------------------------------
// Main 

int main(int argc, char *argv[])
{
    glutInit(&argc, argv);
    glutInitWindowSize(500, 500);
    glutInitWindowPosition(250, 250);
    glutInitDisplayMode(GLUT_RGB | GLUT_SINGLE);
    glutCreateWindow("Fireworks");
    glutDisplayFunc(display);
    init();
    glutMainLoop();
    return 0;
}
