//---------------------------------------
// Program: hw6.cpp
// Purpose: Demonstrate ray tracing with multiple light sources.
// Author:  [Your Name]
// Date:    [Current Date]
//---------------------------------------
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <ctime>
#ifdef MAC
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif
using namespace std;

// Include ray tracing and phong shading classes
#include "ray_classes.h"

// Global variables
#define XDIM 600
#define YDIM 600
#define ZDIM 600
unsigned char image[YDIM][XDIM][3];
float position = -5;
string mode = "phong";
float Bounce = -1;
const float RADIUS = 2.0;
const int SPHERES = 2; // Only two spheres - one static, one rotating
Sphere3D sphere[SPHERES];
ColorRGB color[SPHERES];

// Parameters for rotating sphere
const float ROTATION_RADIUS = 4.0;
const float ANGULAR_SPEED = 0.05; // Adjust as needed
float rotation_angle = 0.0;

//---------------------------------------
// Calculate random value between [min..max]
//---------------------------------------
float myrand(float min, float max)
{
   return rand() * (max - min) / RAND_MAX + min;
}

//---------------------------------------
// Check to see if point is in shadow
//---------------------------------------
bool in_shadow(Point3D pt, Vector3D dir, int current, Sphere3D sphere[], int count)
{
   // Define ray to light source
   Ray3D shadow_ray;
   shadow_ray.set(pt, dir);

   // Check to see if ray intersects any sphere
   Point3D point;
   Vector3D normal;
   for (int index = 0; index < count; index++)
      if ((index != current) && 
         (sphere[index].get_intersection(shadow_ray, point, normal)))
         return true;
   return false;
}

//---------------------------------------
// Perform ray tracing of scene
//---------------------------------------
void ray_trace()
{
   // Define camera point
   Point3D camera(0, 0, position);

   // Define light sources
   const int NUM_LIGHTS = 2;
   Point3D light_positions[NUM_LIGHTS] = {Point3D(-7.0f, 7.0f, 7.0f), Point3D(7.0f, 7.0f, -7.0f)};
   ColorRGB light_colors[NUM_LIGHTS] = {ColorRGB(), ColorRGB()};

   // Define shader
   Phong shader;
   shader.SetCamera(camera);
   
   // Perform ray tracing
   for (int y = 0; y < YDIM; y++)
      for (int x = 0; x < XDIM; x++)
      {
         // Clear image
         image[y][x][0] = 0;
         image[y][x][1] = 0;
         image[y][x][2] = 0;

         // Define sample point on image plane
         float xpos = (x - XDIM / 2) * 2.0 / XDIM;
         float ypos = (y - YDIM / 2) * 2.0 / YDIM;
         Point3D point(xpos, ypos, 0);

         // Define ray from camera through image
         Ray3D ray;
         ray.set(camera, point);

         // Perform sphere intersection
         int closest = -1;
         Point3D p, closest_p;
         Vector3D n, closest_n;
         closest_p.set(0, 0, ZDIM);
         for (int s = 0; s < SPHERES; s++)
         {
            if ((sphere[s].get_intersection(ray, p, n)) && (p.pz < closest_p.pz))
            {
               closest = s;
               closest_p = p;
               closest_n = n;
            }
         }

         // Calculate pixel color
if (closest >= 0)
{
    // Display surface normal
    if (mode == "normal")
    {
        image[y][x][0] = 127 + closest_n.vx * 127;
        image[y][x][1] = 127 + closest_n.vy * 127;
        image[y][x][2] = 127 + closest_n.vz * 127;
    }

    // Calculate Phong shading
    if (mode == "phong")
    {
        // Accumulate color from each light source
        ColorRGB pixel;
        for (int l = 0; l < NUM_LIGHTS; l++)
        {
            // Calculate light direction vector
            Vector3D light_dir;
            light_dir.set(light_positions[l].px - closest_p.px,
                          light_positions[l].py - closest_p.py,
                          light_positions[l].pz - closest_p.pz);
            light_dir.normalize();

            // Check if in shadow
            if (in_shadow(closest_p, light_dir, closest, sphere, SPHERES))
                shader.SetObject(color[closest], 0.4, 0.0, 0.0, 1);
            else
                shader.SetObject(color[closest], 0.4, 0.4, 0.4, 10);

            // Calculate pixel color from this light source
            ColorRGB light_contribution;
            shader.GetShade(closest_p, closest_n, light_contribution);

            // Add light contribution to pixel color
            pixel.add(light_contribution);
        }
        image[y][x][0] = pixel.R;
        image[y][x][1] = pixel.G;
        image[y][x][2] = pixel.B;
    }
}
      }

   // Update display
   glutPostRedisplay();
}

//---------------------------------------
// Initialize the scene
//---------------------------------------
void init()
{
   // Define sphere and color
   sphere[0].set(Point3D(0.0f, 0.0f, RADIUS / 2.0f), Vector3D(0.0f, 0.0f, 0.0f), RADIUS / 4.0f);
   color[0].set(255.0f, 0.0f, 0.0f);
   sphere[1].set(Point3D(ROTATION_RADIUS * cos(rotation_angle), ROTATION_RADIUS * sin(rotation_angle), RADIUS / 2.0f), Vector3D(0.0f, 0.0f, 0.0f), RADIUS / 4.0f);
   color[1].set(0.0f, 0.0f, 255.0f);

   // Set up background color
   glClearColor(0, 0, 0, 0);
}

//---------------------------------------
// Keyboard function
//---------------------------------------
void keyboard(unsigned char key, int x, int y)
{
    if (key == 'n') // Toggle mode between normal and phong
    {
        mode = (mode == "normal") ? "phong" : "normal";
        glutPostRedisplay(); // Request redisplay
    }
}

//---------------------------------------
// Display callback for OpenGL
//---------------------------------------
void display()
{
   // Display image
   glClear(GL_COLOR_BUFFER_BIT);
   glDrawPixels(XDIM, YDIM, GL_RGB, GL_UNSIGNED_BYTE, image);
   glFlush();
}

//---------------------------------------
// Timer callback for OpenGL
//---------------------------------------
void timer(int value)
{
   // Move bouncing balls
   int i;
   for (i = 0; i < SPHERES; i++)
   {
      // Update ball position
      sphere[i].center.px += sphere[i].motion.vx;
      sphere[i].center.py += sphere[i].motion.vy;
      sphere[i].center.pz += sphere[i].motion.vz;

      // Bounce off walls
      if (sphere[i].center.px > RADIUS/2 - sphere[i].radius) 
         {sphere[i].center.px = RADIUS/2 - sphere[i].radius; 
          sphere[i].motion.vx *= Bounce; }
      if (sphere[i].center.py > RADIUS/2 - sphere[i].radius) 
         {sphere[i].center.py = RADIUS/2 - sphere[i].radius; 
          sphere[i].motion.vy *= Bounce; }
      if (sphere[i].center.pz > RADIUS/2 - sphere[i].radius) 
         {sphere[i].center.pz = RADIUS/2 - sphere[i].radius; 
          sphere[i].motion.vz *= Bounce; }
      if (sphere[i].center.px < -RADIUS/2 + sphere[i].radius) 
         {sphere[i].center.px = -RADIUS/2 + sphere[i].radius; 
          sphere[i].motion.vx *= Bounce; }
      if (sphere[i].center.py < -RADIUS/2 + sphere[i].radius) 
         {sphere[i].center.py = -RADIUS/2 + sphere[i].radius; 
          sphere[i].motion.vy *= Bounce; }
      if (sphere[i].center.pz < -RADIUS/2 + sphere[i].radius) 
         {sphere[i].center.pz = -RADIUS/2 + sphere[i].radius; 
          sphere[i].motion.vz *= Bounce; }

   }

   // Calculate and display image
   ray_trace();
   glutPostRedisplay();
   glutTimerFunc(10, timer, 0);
}

//---------------------------------------
// Main program to create window
//---------------------------------------
int main(int argc, char *argv[])
{
   // Initialize random seed
   srand(time(NULL));

   // Initialize OpenGL
   glutInit(&argc, argv);
   glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
   glutInitWindowSize(XDIM, YDIM);
   glutInitWindowPosition(100, 100);
   glutCreateWindow("Ray Tracing");

   // Set OpenGL callback functions
   glutDisplayFunc(ray_trace);
   glutIdleFunc(ray_trace);
   glutKeyboardFunc(keyboard); // Added keyboard callback
   glutTimerFunc(1000 / 60, timer, 0); // Added timer callback

   // Initialize scene
   init();

   // Enter main OpenGL loop
   glutMainLoop();
   return 0;
}
