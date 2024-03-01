#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#ifdef MAC
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

// Global constants
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

// Global variables 
int xangle = 10;
int yangle = 15;
int mode = 2;
int count = 0;
float point[LINE_COUNT][4];

//---------------------------------------
// Init function for OpenGL
//---------------------------------------
void init()
{
   glClearColor(0.0, 0.0, 0.0, 1.0);
   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();
   glOrtho(MIN_X_VIEW, MAX_X_VIEW, 
      MIN_Y_VIEW, MAX_Y_VIEW, 
      MIN_Z_VIEW, MAX_Z_VIEW);
   glEnable(GL_DEPTH_TEST);
}

//---------------------------------------
// Function to draw a zero thickness wall
//---------------------------------------
void wall(float x1, float y1, float x2, float y2)
{
   float z1 = 0;
   float z2 = 10;

   // Draw wall polygon
   glColor3f(1.0, 0.0, 0.0);
   glBegin(GL_POLYGON);
   glVertex3f(x1, y1, z1);
   glVertex3f(x2, y2, z1);
   glVertex3f(x2, y2, z2);
   glVertex3f(x1, y1, z2);
   glEnd();

   // Draw wall outline
   glColor3f(1.0, 1.0, 1.0);
   glBegin(GL_LINE_LOOP);
   glVertex3f(x1, y1, z1);
   glVertex3f(x2, y2, z1);
   glVertex3f(x2, y2, z2);
   glVertex3f(x1, y1, z2);
   glEnd();
}

//---------------------------------------
// Mouse callback for OpenGL
//---------------------------------------
void mouse(int button, int state, int x, int y)
{
   // Calculate scale factors
   if (mode != 2) return;
   float x_scale = (MAX_X_VIEW - MIN_X_VIEW) / 
      (float)(MAX_X_SCREEN - MIN_X_SCREEN);
   float y_scale = (MIN_Y_VIEW - MAX_Y_VIEW) / 
      (float)(MAX_Y_SCREEN - MIN_Y_SCREEN);

   // Handle mouse down
   if (state == GLUT_DOWN)
   {
      point[count][0] = MIN_X_VIEW + (x - MIN_X_SCREEN) * x_scale;
      point[count][1] = MAX_Y_VIEW + (y - MIN_Y_SCREEN) * y_scale;
   }

   // Handle mouse up
   else if (state == GLUT_UP) 
   {
      point[count][2] = MIN_X_VIEW + (x - MIN_X_SCREEN) * x_scale;
      point[count][3] = MAX_Y_VIEW + (y - MIN_Y_SCREEN) * y_scale;
      count++; 
      glutPostRedisplay();
   }
}

//---------------------------------------
// Motion callback for OpenGL
//---------------------------------------
void motion(int x, int y)
{
   // Calculate scale factors
   if (mode != 2) return;
   float x_scale = (MAX_X_VIEW - MIN_X_VIEW) / 
      (float)(MAX_X_SCREEN - MIN_X_SCREEN);
   float y_scale = (MIN_Y_VIEW - MAX_Y_VIEW) / 
      (float)(MAX_Y_SCREEN - MIN_Y_SCREEN);

   // Handle mouse motion
   point[count][2] = MIN_X_VIEW + (x - MIN_X_SCREEN) * x_scale;
   point[count][3] = MAX_Y_VIEW + (y - MIN_Y_SCREEN) * y_scale;
   glutPostRedisplay();
}

//---------------------------------------
// Display callback for OpenGL
//---------------------------------------
void display()
{
   // Incrementally rotate objects
   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
   glMatrixMode(GL_MODELVIEW);
   glLoadIdentity();
   if (mode == 3)
   {
      glRotatef(xangle, 1.0, 0.0, 0.0);
      glRotatef(yangle, 0.0, 1.0, 0.0);
   }

   // Draw all walls
   for (int i=0; i<=count; i++)
      if ((point[i][0] != point[i][2]) || (point[i][1] != point[i][3])) 
         wall(point[i][0], point[i][1], point[i][2], point[i][3]);
   glFlush();
}

//---------------------------------------
// Main program
//---------------------------------------
int main(int argc, char *argv[])
{
   glutInit(&argc, argv);
   glutInitWindowSize(MAX_Y_SCREEN, MAX_X_SCREEN);
   glutInitWindowPosition(MAX_Y_SCREEN/2, MAX_X_SCREEN/2);
   glutInitDisplayMode(GLUT_RGB | GLUT_SINGLE | GLUT_DEPTH);
   glutCreateWindow("Building");
   glutDisplayFunc(display);
   glutMouseFunc(mouse);
   glutMotionFunc(motion);
   init();
   printf("Keyboard commands:\n");
   printf("   'x' - rotate x-axis -5 degrees\n");
   printf("   'X' - rotate x-axis +5 degrees\n");
   printf("   'y' - rotate y-axis -5 degrees\n");
   printf("   'Y' - rotate y-axis +5 degrees\n");
   printf("   '2' - enter 2D display mode\n");
   printf("   '3' - enter 3D display mode\n");
   printf("   'L' - load building.txt file\n");
   printf("   'S' - save building.txt file\n");
   printf("Mouse operations:\n");
   printf("   'mouse down' - start drawing line\n");
   printf("   'mouse motion' - draw rubber-band line\n");
   printf("   'mouse up' - finish drawing line\n");
   glutMainLoop();
   return 0;
}
