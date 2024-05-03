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

            // Calculate pixel color using Phong shading
            if (closest >= 0)
            {
                // Set object color for Phong shading
                ColorRGB object_color = color[closest];

                // Ambient light component (base color)
                float ambient_coefficient = 0.2;
                ColorRGB ambient_color = object_color * ambient_coefficient;

                // Diffuse and specular light components
                ColorRGB diffuse_color(0, 0, 0);
                ColorRGB specular_color(0, 0, 0);

                // Loop through each light source
                for (int l = 0; l < NUM_LIGHTS; l++)
                {
                    // Calculate light direction vector
                    Vector3D light_dir;
                    light_dir.set(light_positions[l].px - closest_p.px,
                                  light_positions[l].py - closest_p.py,
                                  light_positions[l].pz - closest_p.pz);
                    light_dir.normalize();

                    // Check if in shadow
                    if (!in_shadow(closest_p, light_dir, closest, sphere, SPHERES))
                    {
                        // Diffuse component
                        float diffuse_coefficient = 0.8;
                        float diffuse_intensity = max(0.0f, closest_n.dot(light_dir));
                        diffuse_color += object_color * light_colors[l] * diffuse_intensity * diffuse_coefficient;

                        // Specular component
                        float specular_coefficient = 0.5;
                        Vector3D reflection_dir = closest_n * 2.0 * closest_n.dot(light_dir) - light_dir;
                        float specular_intensity = max(0.0f, -ray.dir.dot(reflection_dir));
                        specular_intensity = pow(specular_intensity, 10); // Shininess factor
                        specular_color += light_colors[l] * specular_intensity * specular_coefficient;
                    }
                }

                // Final pixel color (ambient + diffuse + specular)
                ColorRGB pixel_color = ambient_color + diffuse_color + specular_color;

                // Clamp pixel color components to [0, 255]
                pixel_color.clamp();

                // Assign pixel color to image
                image[y][x][0] = pixel_color.R;
                image[y][x][1] = pixel_color.G;
                image[y][x][2] = pixel_color.B;
            }
        }

    // Update display
    glutPostRedisplay();
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
