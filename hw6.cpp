//---------------------------------------
// Program: ray_trace.cpp
// Purpose: Demonstrate ray tracing.
// Author:  John Gauch
// Date:    Spring 2019
//---------------------------------------
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <vector>
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
const int SPHERES = 10;
Sphere3D sphere[SPHERES];
ColorRGB color[SPHERES];

float animation_radius = 5.0; // Radius of circular path
float animation_speed = 0.1;  // Speed of rotation
float animation_angle = 0.0;  // Current angle of rotation

#define MAX_LIGHTS 2 // Maximum number of light sources
std::vector<ColorRGB> light_colors(MAX_LIGHTS); // Array to store light colors
std::vector<Vector3D> light_dirs(MAX_LIGHTS); // Array to store light directions

//---------------------------------------
// Calculate random value between [min..max]
//---------------------------------------
float myrand(float min, float max)
{
   return rand() * (max - min) / RAND_MAX + min;
}

// Function to initialize light sources
void init_lights() {
    // Initialize light sources here with appropriate colors and directions
    light_colors[0].set(255, 255, 255); // White light
    light_dirs[0].set(1, -1, -1); // Directional light from top-right

    light_colors[1].set(255, 255, 255); // Red light
    light_dirs[1].set(-1, 1, -1); // Directional light from bottom-left

    // Ensure to normalize the light directions
    for (int i = 0; i < MAX_LIGHTS; ++i) {
        light_dirs[i].normalize();
    }
}

//---------------------------------------
// Check to see if point is in shadow
//---------------------------------------
bool in_shadow(Point3D pt, Vector3D dir, int current, Sphere3D sphere[], int count)
{
   // Define ray to light source
   Ray3D shadow_ray;
   shadow_ray.set(pt, dir);

   // Check to see ray intersects any sphere
   Point3D point;
   Vector3D normal;
   for (int index=0; index<count; index++)
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
    camera.set(0,0,position);

    // Define light sources
    init_lights();

    // Define shader
    Phong shader;

    // Perform ray tracing
    for (int y = 0; y < YDIM; y++)
        for (int x = 0; x < XDIM; x++)
        {
            // Clear image
            image[y][x][0] = 0;
            image[y][x][1] = 0;
            image[y][x][2] = 0;

            // Define sample point on image plane
            float xpos = (x - XDIM/2) * 2.0 / XDIM;
            float ypos = (y - YDIM/2) * 2.0 / YDIM;
            Point3D point;
            point.set(xpos, ypos, 0);

            // Define ray from camera through image
            Ray3D ray;
            ray.set(camera, point);

            // Perform sphere intersection
            int closest = -1;
            Point3D p, closest_p;
            Vector3D n, closest_n;
            closest_p.set(0,0,ZDIM);
            for (int s=0; s<SPHERES; s++)
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
                // Calculate pixel color from all light sources
                ColorRGB pixel;
                int num_contributions = 0; // Track the number of contributions

                for (int i = 0; i < MAX_LIGHTS; ++i) {
                    // Check if in shadow
                    bool is_in_shadow = in_shadow(closest_p, light_dirs[i], closest, sphere, SPHERES);

                    // Set light source for shading
                    shader.SetLight(light_colors[i], light_dirs[i]);

                    if (!is_in_shadow) {
                        // Set object color and shading parameters
                        shader.SetObject(color[closest], 0.4, 0.4, 0.4, 10);

                        // Calculate Phong shading for this light source
                        ColorRGB light_contribution;
                        shader.GetShade(closest_p, closest_n, light_contribution);

                        // Add up the color contribution from this light source
                        pixel.add(light_contribution);
                        num_contributions++; // Increment the number of contributions
                    }
                }

                // Normalize the accumulated pixel color to prevent exceeding 255
                if (num_contributions > 0) {
                    pixel.R /= num_contributions;
                    pixel.G /= num_contributions;
                    pixel.B /= num_contributions;
                    pixel.normalize();
                }

                // Assign the accumulated pixel color to the image
                image[y][x][0] = pixel.R;
                image[y][x][1] = pixel.G;
                image[y][x][2] = pixel.B;
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

   // Define array of spheres
   srand(time(NULL));
   for (int s=0; s<SPHERES; s++)
   {
      float cx = myrand(-RADIUS/2, RADIUS/2);
      float cy = myrand(-RADIUS/2, RADIUS/2);
      float cz = myrand(0, RADIUS/2);
      Point3D center;
      center.set(cx,cy,cz);

      float mx = myrand(-RADIUS/100, RADIUS/200);
      float my = myrand(-RADIUS/100, RADIUS/200);
      float mz = myrand(-RADIUS/100, RADIUS/200);
      Vector3D motion;
      motion.set(mx,my,mz);
      float radius = myrand(RADIUS/20, RADIUS/10);
      sphere[s].set(center, motion, radius);
      int R = rand() % 255;
      int G = rand() % 255;
      int B = rand() % 255;
      color[s].set(R,G,B);
   }

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
// Timer callback for OpenGL
//---------------------------------------
void timer(int value)
{
   // Update animation angle for the rotating sphere
   animation_angle += animation_speed;

   // Calculate the position of the rotating sphere based on the angle
   float orbit_radius = 5.0; // Radius of the orbit
   float orbit_center_x = 0.0; // X-coordinate of the center of the orbit
   float orbit_center_y = 0.0; // Y-coordinate of the center of the orbit
   float rotating_sphere_x = orbit_center_x + orbit_radius * cos(animation_angle);
   float rotating_sphere_y = orbit_center_y + orbit_radius * sin(animation_angle);

   // Update the position of the rotating sphere
   sphere[0].center.px = rotating_sphere_x;
   sphere[0].center.py = rotating_sphere_y;

   // Move bouncing balls
   for (int i = 1; i < SPHERES; i++)
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
   glutTimerFunc(10, timer, 0);
   glutMainLoop();
   return 0;
}
