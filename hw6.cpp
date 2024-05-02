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
   Point3D camera;
   camera.set(0, 0, position);

   // Define light sources
   const int NUM_LIGHTS = 2;
   Point3D light_positions[NUM_LIGHTS] = {Point3D(-5, 5, 5), Point3D(5, 5, -5)};
   ColorRGB light_colors[NUM_LIGHTS] = {ColorRGB(250, 250, 250), ColorRGB(250, 250, 250)};

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
         Point3D point;
         point.set(xpos, ypos, 0);

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
                  // Check if in shadow
                  if (in_shadow(closest_p, (light_positions[l] - closest_p).normalize(), closest, sphere, SPHERES))
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
}

//---------------------------------------
// Init function for OpenGL
//---------------------------------------
void init()
{
   // Initialize OpenGL
   glClearColor(0.0, 0.0, 0.0, 1.0);

   // Print command menu
   cout << "Program commands:\n"
        << "   '+' - increase camera distance\n"
        << "   '-' - decrease camera distance\n"
        << "   'p' - show Phong shading\n"
        << "   'n' - show surface normals\n"
        << "   'q' - quit program\n";

   // Define static sphere
   sphere[0].set(Point3D(0.0f, 0.0f, RADIUS / 2.0f), Vector3D(0.0f, 0.0f, 0.0f), RADIUS / 4.0f);

   // Perform ray tracing
   cout << "camera: 0,0," << position << endl;
   ray_trace();
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
// Keyboard callback for OpenGL
//---------------------------------------
void keyboard(unsigned char key, int x, int y)
{
   // End program
   if (key == 'q')
      exit(0);

   // Move camera position
   else if (key == '+' && position > -10)
   {
      position = position - 0.5;
      cout << "camera: 0,0," << position << endl;
   }
   else if (key == '-' && position < -5)
   {
      position = position + 0.5;
      cout << "camera: 0,0," << position << endl;
   }

   // Change display mode
   else if (key == 'n')
      mode = "normal";
   else if (key == 'p')
      mode = "phong";

   // Perform ray tracing
   ray_trace();
   glutPostRedisplay();
}

//---------------------------------------
// Main program
//---------------------------------------
int main(int argc, char *argv[])
{
   // Create OpenGL window
   glutInit(&argc, argv);
   glutInitWindowSize(XDIM, YDIM);
   glutInitWindowPosition(0, 0);
   glutInitDisplayMode(GLUT_RGB | GLUT_SINGLE);
   glutCreateWindow("Ray Trace");
   init();

   // Specify callback function
   glutDisplayFunc(display);
   glutKeyboardFunc(keyboard);
   glutMainLoop();
   return 0;
}
