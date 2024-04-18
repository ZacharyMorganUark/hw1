
//---------------------------------------
// Program: texture3.cpp used as skelton
// Purpose: Maze for Assignment 4
// Author: Zachary Morgan 
// Date: 10 April 2024
//---------------------------------------
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#ifdef MAC
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif
#include "libim/im_color.h"
// Globals
#define ROTATE 1
#define TRANSLATE 2
int xangle = 10;
int yangle = 10;
int zangle = 10;
int xpos = 0;
int ypos = 0;
int zpos = 0;
int mode = ROTATE;
int ROWS, COLS;
char **maze;
GLuint textures[4]; // Texture IDs for walls and floor
//---------------------------------------
// Initialize texture images
//---------------------------------------
void init_textures() {
 // Read and initialize textures for walls and floor
 const char* textureFiles[4] = {"textures/rock.jpg", "textures/brick.jpg", "textures/wood.jpg", "textures/grass.jpg"};
 glGenTextures(4, textures); 
 for (int i = 0; i < 4; ++i) {
 im_color image;
 image.ReadJpg(textureFiles[i]);
 int xdim = 1, ydim = 1;
 while (xdim < image.R.Xdim) xdim *= 2;
 while (ydim < image.R.Ydim) ydim *= 2;
 image.Interpolate(xdim, ydim);
 
 unsigned char *texture = (unsigned char *)malloc(xdim * ydim * 3);
 int index = 0;
 for (int y = 0; y < ydim; ++y) {
 for (int x = 0; x < xdim; ++x) {
 texture[index++] = (unsigned char)(image.R.Data2D[y][x]);
 texture[index++] = (unsigned char)(image.G.Data2D[y][x]);
 texture[index++] = (unsigned char)(image.B.Data2D[y][x]);
 }
 }
 
 glBindTexture(GL_TEXTURE_2D, textures[i]);
 glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
 glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
 glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
 glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
 glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, xdim, ydim, 0, GL_RGB, GL_UNSIGNED_BYTE, texture);
 free(texture);
 }
}
//---------------------------------------
// Read the maze file
//---------------------------------------
void read_maze(const char *filename) {
 FILE *file = fopen(filename, "r");
 if (!file) {
 fprintf(stderr, "Error opening file %s\n", filename);
 exit(1);
 }
 int result = fscanf(file, "%d %d", &ROWS, &COLS); // Read number of rows and columns
 if (result != 2) {
 fprintf(stderr, "Error reading maze dimensions from file %s\n", filename);
 exit(1);
 }
 maze = (char **)malloc(ROWS * sizeof(char *));
 for (int i = 0; i < ROWS; ++i) {
 maze[i] = (char *)malloc((COLS + 1) * sizeof(char)); // Allocate memory for the row
 result = fscanf(file, "%s", maze[i]);
 if (result != 1) {
 fprintf(stderr, "Error reading maze row %d from file %s\n", i, filename);
 exit(1);
 }
 maze[i][COLS] = '\0'; // Null-terminate the string
 }
 fclose(file);
}
//---------------------------------------
// Function to draw textured cube
//---------------------------------------
void draw_textured_cube(float xmin, float ymin, float zmin,
 float xmax, float ymax, float zmax, int textureIndex) {
 glBindTexture(GL_TEXTURE_2D, textures[textureIndex]);
 glBegin(GL_QUADS);
 // Front face
 glTexCoord2f(0.0, 0.0); glVertex3f(xmin, ymin, zmax);
 glTexCoord2f(1.0, 0.0); glVertex3f(xmax, ymin, zmax);
 glTexCoord2f(1.0, 1.0); glVertex3f(xmax, ymax, zmax);
 glTexCoord2f(0.0, 1.0); glVertex3f(xmin, ymax, zmax);
 // Back face
 glTexCoord2f(0.0, 0.0); glVertex3f(xmin, ymin, zmin);
 glTexCoord2f(1.0, 0.0); glVertex3f(xmin, ymax, zmin);
 glTexCoord2f(1.0, 1.0); glVertex3f(xmax, ymax, zmin);
 glTexCoord2f(0.0, 1.0); glVertex3f(xmax, ymin, zmin);
 // Top face
 glTexCoord2f(0.0, 0.0); glVertex3f(xmin, ymax, zmin);
 glTexCoord2f(1.0, 0.0); glVertex3f(xmin, ymax, zmax);
 glTexCoord2f(1.0, 1.0); glVertex3f(xmax, ymax, zmax);
 glTexCoord2f(0.0, 1.0); glVertex3f(xmax, ymax, zmin);
 // Bottom face
 glTexCoord2f(0.0, 0.0); glVertex3f(xmin, ymin, zmin);
 glTexCoord2f(1.0, 0.0); glVertex3f(xmax, ymin, zmin);
 glTexCoord2f(1.0, 1.0); glVertex3f(xmax, ymin, zmax);
 glTexCoord2f(0.0, 1.0); glVertex3f(xmin, ymin, zmax);
 // Right face
 glTexCoord2f(0.0, 0.0); glVertex3f(xmax, ymin, zmin);
 glTexCoord2f(1.0, 0.0); glVertex3f(xmax, ymax, zmin);
 glTexCoord2f(1.0, 1.0); glVertex3f(xmax, ymax, zmax);
 glTexCoord2f(0.0, 1.0); glVertex3f(xmax, ymin, zmax);
 // Left face
 glTexCoord2f(0.0, 0.0); glVertex3f(xmin, ymin, zmin);
 glTexCoord2f(1.0, 0.0); glVertex3f(xmin, ymin, zmax);
 glTexCoord2f(1.0, 1.0); glVertex3f(xmin, ymax, zmax);
 glTexCoord2f(0.0, 1.0); glVertex3f(xmin, ymax, zmin);
 glEnd();
}
//---------------------------------------
// Init function for OpenGL
//---------------------------------------
void init()
{
 // Init view
 glClearColor(0.0, 0.0, 0.0, 1.0);
 glMatrixMode(GL_PROJECTION);
 glLoadIdentity();
 glOrtho(-10.0, 10.0, -10.0, 10.0, -10.0, 10.0);
 glEnable(GL_DEPTH_TEST);
 // Init texture
 init_textures();
 glEnable(GL_TEXTURE_2D);
}
//---------------------------------------
// Display callback for OpenGL
//---------------------------------------
void display() {
 glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
 glMatrixMode(GL_MODELVIEW);
 glLoadIdentity();
 glTranslatef(xpos / 500.0, ypos / 500.0, zpos / 500.0);
 glRotatef(xangle, 1.0, 0.0, 0.0);
 glRotatef(yangle, 0.0, 1.0, 0.0);
 glRotatef(zangle, 0.0, 0.0, 1.0);
 // Draw floor
 draw_textured_cube(-1, -1, -1, 1, 1, -1.01, 3);
 // Draw walls based on maze data
 for (int r = 0; r < ROWS; ++r) {
 for (int c = 0; c < COLS; ++c) {
 float xmin = c - 0.5, xmax = c + 0.5;
 float ymin = r - 0.5, ymax = r + 0.5;
 float zmin = -1.01, zmax = 1.01;
 int textureIndex;
 switch (maze[r][c]) {
 case 'r':
 textureIndex = 0; break;
 case 'b':
 textureIndex = 1; break;
 case 'w':
 textureIndex = 2; break;
 case 'g':
 textureIndex = 3; break;
 default:
 continue;
 }
 draw_textured_cube(xmin, ymin, zmin, xmax, ymax, zmax, textureIndex);
 }
 }
 glutSwapBuffers();
}
//---------------------------------------
// Keyboard callback for OpenGL
//---------------------------------------
void keyboard(unsigned char key, int x, int y) {
 // Check mode
 if ((key == 'r') || (key == 'R')) {
 printf("Type x y z to decrease or X Y Z to increase ROTATION angles.\n");
 mode = ROTATE;
 } else if ((key == 't') || (key == 'T')) {
 printf("Type x y z to decrease or X Y Z to increase TRANSLATION distance.\n");
 mode = TRANSLATE;
 }
 // Handle R
 if (mode == ROTATE) {
 if (key == 'x')
 xangle -= 5;
 else if (key == 'y')
 yangle -= 5;
 else if (key == 'z')
 zangle -= 5;
 else if (key == 'X')
 xangle += 5;
 else if (key == 'Y')
 yangle += 5;
 else if (key == 'Z')
 zangle += 5;
 glutPostRedisplay();
 }
 // Handle T
 if (mode == TRANSLATE) {
 if (key == 'x')
 xpos -= 5;
 else if (key == 'y')
 ypos -= 5;
 else if (key == 'z')
 zpos -= 5;
 else if (key == 'X')
 xpos += 5;
 else if (key == 'Y')
 ypos += 5;
 else if (key == 'Z')
 zpos += 5;
 glutPostRedisplay();
 }
}
//---------------------------------------
// Mouse callback for OpenGL
//---------------------------------------
void mouse(int button, int state, int x, int y) {
 // Handle mouse down
 static int xdown, ydown;
 if (state == GLUT_DOWN) {
 xdown = x;
 ydown = y;
 }
 // Handle R
 if ((mode == ROTATE) && (state == GLUT_UP)) {
 xangle += (y - ydown);
 yangle -= (x - xdown);
 zangle = 0;
 glutPostRedisplay();
 }
 // Handle T
 if ((mode == TRANSLATE) && (state == GLUT_UP)) {
 xpos += (x - xdown);
 ypos -= (y - ydown);
 glutPostRedisplay();
 }
}
//---------------------------------------
// Main program
//---------------------------------------
int main(int argc, char *argv[]) {
 // Read maze file
 if (argc != 2) {
 printf("Usage: %s maze.txt\n", argv[0]);
 exit(1);
 }
 read_maze(argv[1]);
 // Initialize OpenGL window
 glutInit(&argc, argv);
 glutInitWindowSize(500, 500);
 glutInitWindowPosition(250, 250);
 glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
 glutCreateWindow("HW4 Maze");
 init();
 glutDisplayFunc(display);
 glutKeyboardFunc(keyboard);
 glutMouseFunc(mouse);
 glutMainLoop();
 // Free memory because of malloc use
 for (int i = 0; i < ROWS; ++i)
 free(maze[i]);
 free(maze);
 return 0;
}

