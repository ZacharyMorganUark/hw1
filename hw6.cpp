//---------------------------------------
// Program: hw6.cpp
// Purpose: Demonstrate ray tracing with multiple light sources and two spheres.
// Author: [Your Name]
// Date: [Current Date]
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
const float BOUNCE = -1;
const float RADIUS = 2.0;
const int SPHERES = 2; // Only two spheres
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

void ray_trace()
{
    // Initialize camera
    Point3D camera(0, 0, -500);
    LightModel.SetCamera(camera);

    // Loop over image rows
    for (int y = -250; y <= 250; y++)
    {
        // Loop over image columns
        for (int x = -250; x <= 250; x++)
        {
            // Create ray from camera to pixel
            Point3D origin(0, 0, 0);
            Vector3D direction(x - origin.px, y - origin.py, 500 - origin.pz);
            direction.normalize();
            Ray3D ray;
            ray.set(origin, direction);

            // Initialize pixel color
            ColorRGB pixel_color(0, 0, 0);

            // Loop over spheres
            for (int s = 0; s < SPHERES; s++) // Change NUM_SPHERES to SPHERES
            {
                // Find intersection with sphere
                Point3D point;
                Vector3D normal;
                bool hit = sphere[s].get_intersection(ray, point, normal); // Change Spheres to sphere

                // If intersection, compute shading
                if (hit)
                {
                    ColorRGB color;
                    LightModel.GetShade(point, normal, color);
                    pixel_color.add(color);
                }
            }

            // Store pixel color in image array
            image[y + 250][x + 250][0] = pixel_color.R; // Red component
            image[y + 250][x + 250][1] = pixel_color.G; // Green component
            image[y + 250][x + 250][2] = pixel_color.B; // Blue component
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

    // Define properties for the first sphere
    Point3D center1(0, 0, 4); // Center of the first sphere
    Vector3D motion1(0.01, 0.02, 0.03); // Motion of the first sphere
    float radius1 = 1.5; // Radius of the first sphere
    sphere[0].set(center1, motion1, radius1);
    color[0].set(255, 0, 0); // Red color for the first sphere

    // Define properties for the second sphere
    Point3D center2(-1.5, 1.5, 6); // Center of the second sphere
    Vector3D motion2(-0.01, -0.02, -0.03); // Motion of the second sphere
    float radius2 = 1.0; // Radius of the second sphere
    sphere[1].set(center2, motion2, radius2);
    color[1].set(0, 0, 255); // Blue color for the second sphere

    // Perform ray tracing
    cout << "camera: 0,0," << position << endl;
    ray_trace();
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
    glutInitDisplayMode(GLUT_RGB | GLUT_SINGLE);
    glutInitWindowSize(XDIM, YDIM);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Ray Tracing");

    // Set OpenGL callback functions
    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);
    glutTimerFunc(1000 / 60, timer, 0); // Added timer callback

    // Initialize scene
    init();

    // Enter main OpenGL loop
    glutMainLoop();
    return 0;
}
