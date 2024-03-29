#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <GL/glut.h>

#include "shading.cpp"

#define ROWS 500
#define COLS 500
#define SCALE_FACTOR 0.0002

float Depth[ROWS][COLS];
float R[ROWS][COLS], G[ROWS][COLS], B[ROWS][COLS];
float x_angle = 0, y_angle = 0, z_angle = 0;
bool color_display_mode = false;
bool phong_display_mode = false;

void read_depth_data(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        fprintf(stderr, "Error opening file %s\n", filename);
        exit(1);
    }

    for (int i = ROWS - 1; i >= 0; i--) {  // Read rows in reverse order
        for (int j = 0; j < COLS; j++) {
            if (fscanf(file, "%f", &Depth[i][j]) != 1) {
                fprintf(stderr, "Error reading depth data from file\n");
                fclose(file);
                exit(1);
            }
        }
    }
    fclose(file);
}

void read_color_data(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        fprintf(stderr, "Error opening file %s\n", filename);
        exit(1);
    }

    for (int i = ROWS - 1; i >= 0; i--) {  // Read rows in reverse order
        for (int j = 0; j < COLS; j++) {
            if (fscanf(file, "%f %f %f", &R[i][j], &G[i][j], &B[i][j]) != 3) {
                fprintf(stderr, "Error reading color data from file\n");
                fclose(file);
                exit(1);
            }
        }
    }
    fclose(file);
}

void get_surface_normal(int i, int j, float& normal_x, float& normal_y, float& normal_z) {
    // Calculate the surface normal using cross product of vectors formed by adjacent vertices
    float vertex1[3] = {(float)j / COLS - 0.5, (float)i / ROWS - 0.5, Depth[i][j] * SCALE_FACTOR};
    float vertex2[3] = {(float)(j + 1) / COLS - 0.5, (float)i / ROWS - 0.5, Depth[i][j + 1] * SCALE_FACTOR};
    float vertex3[3] = {(float)(j + 1) / COLS - 0.5, (float)(i + 1) / ROWS - 0.5, Depth[i + 1][j + 1] * SCALE_FACTOR};

    // Calculate vectors from vertex1 to vertex2 and vertex3
    float vector1[3] = {vertex2[0] - vertex1[0], vertex2[1] - vertex1[1], vertex2[2] - vertex1[2]};
    float vector2[3] = {vertex3[0] - vertex1[0], vertex3[1] - vertex1[1], vertex3[2] - vertex1[2]};

    // Calculate the cross product of the two vectors to get the surface normal
    normal_x = vector1[1] * vector2[2] - vector1[2] * vector2[1];
    normal_y = vector1[2] * vector2[0] - vector1[0] * vector2[2];
    normal_z = vector1[0] * vector2[1] - vector1[1] * vector2[0];

    // Normalize the surface normal
    float length = sqrt(normal_x * normal_x + normal_y * normal_y + normal_z * normal_z);
    normal_x /= length;
    normal_y /= length;
    normal_z /= length;
} 

void init() {
    glClearColor(0.0, 0.0, 0.0, 1.0);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-1.0, 1.0, -1.0, 1.0, -1.0, 1.0);
    glEnable(GL_DEPTH_TEST);
    glShadeModel(GL_SMOOTH);
    glEnable(GL_NORMALIZE);
    init_light(GL_LIGHT0, 0, 1, 1, 0.5, 0.5, 0.5);
    init_light(GL_LIGHT1, 0, 0, 1, 0.5, 0.5, 0.5);
    init_light(GL_LIGHT2, 0, 1, 0, 0.5, 0.5, 0.5);

    read_depth_data("penny-depth.txt");
    read_color_data("penny-image.txt");
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    init_material(Ka, Kd, Ks, 100 * Kp, 0.8, 0.6, 0.4);
    glRotatef(x_angle, 1, 0, 0);
    glRotatef(y_angle, 0, 1, 0);
    glRotatef(z_angle, 0, 0, 1);

    glColor3f(1.0, 1.0, 1.0);
    for (int i = 0; i < ROWS - 1; i++) {
        glBegin(GL_LINE_LOOP);
        for (int j = 0; j < COLS - 1; j++) {
            glVertex3f((float) j / COLS - 0.5, (float) i / ROWS - 0.5, Depth[i][j] * SCALE_FACTOR);
            glVertex3f((float) (j + 1) / COLS - 0.5, (float) i / ROWS - 0.5, Depth[i][j + 1] * SCALE_FACTOR);
            glVertex3f((float) (j + 1) / COLS - 0.5, (float) (i + 1) / ROWS - 0.5, Depth[i + 1][j + 1] * SCALE_FACTOR);
            glVertex3f((float) j / COLS - 0.5, (float) (i + 1) / ROWS - 0.5, Depth[i + 1][j] * SCALE_FACTOR);
        }
        glEnd();
    }

    glFlush();
    glutSwapBuffers();
}

void color_display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glDisable(GL_LIGHTING);
    glRotatef(x_angle, 1, 0, 0);
    glRotatef(y_angle, 0, 1, 0);
    glRotatef(z_angle, 0, 0, 1);

    for (int i = 0; i < ROWS - 1; i++) {
        glBegin(GL_POLYGON);
        for (int j = 0; j < COLS - 1; j++) {
            glColor3f(R[i][j] / 255.0, G[i][j] / 255.0, B[i][j] / 255.0);
            glVertex3f((float) j / COLS - 0.5, (float) i / ROWS - 0.5, Depth[i][j] * SCALE_FACTOR);
            glColor3f(R[i][j + 1] / 255.0, G[i][j + 1] / 255.0, B[i][j + 1] / 255.0);
            glVertex3f((float) (j + 1) / COLS - 0.5, (float) i / ROWS - 0.5, Depth[i][j + 1] * SCALE_FACTOR);
            glColor3f(R[i + 1][j + 1] / 255.0, G[i + 1][j + 1] / 255.0, B[i + 1][j + 1] / 255.0);
            glVertex3f((float) (j + 1) / COLS - 0.5, (float) (i + 1) / ROWS - 0.5, Depth[i + 1][j + 1] * SCALE_FACTOR);
            glColor3f(R[i + 1][j] / 255.0, G[i + 1][j] / 255.0, B[i + 1][j] / 255.0);
            glVertex3f((float) j / COLS - 0.5, (float) (i + 1) / ROWS - 0.5, Depth[i + 1][j] * SCALE_FACTOR);
        }
        glEnd();
    }

    glFlush();
    glutSwapBuffers();
}

void phong_display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glEnable(GL_LIGHTING);
    
    //init_material(Ka, Kd, Ks, 100 * Kp, 0.8, 0.6, 0.4);
    glRotatef(x_angle, 1, 0, 0);
    glRotatef(y_angle, 0, 1, 0);
    glRotatef(z_angle, 0, 0, 1);

    for (int i = 0; i < ROWS - 1; i++) {
        glBegin(GL_POLYGON);
        for (int j = 0; j < COLS - 1; j++) {
            float normal_x, normal_y, normal_z;
            get_surface_normal(i, j, normal_x, normal_y, normal_z);
            glNormal3f(normal_x, normal_y, normal_z);

            // Set vertex colors
            glColor3f(R[i][j] / 255.0, G[i][j] / 255.0, B[i][j] / 255.0);

            // Specify vertex positions
            glVertex3f((float) j / COLS - 0.5, (float) i / ROWS - 0.5, Depth[i][j] * SCALE_FACTOR);
            glVertex3f((float) (j + 1) / COLS - 0.5, (float) i / ROWS - 0.5, Depth[i][j + 1] * SCALE_FACTOR);
            glVertex3f((float) (j + 1) / COLS - 0.5, (float) (i + 1) / ROWS - 0.5, Depth[i + 1][j + 1] * SCALE_FACTOR);
            glVertex3f((float) j / COLS - 0.5, (float) (i + 1) / ROWS - 0.5, Depth[i + 1][j] * SCALE_FACTOR);
        }
        glEnd();
    }

    glFlush();
    glutSwapBuffers();
} */

void keyboard(unsigned char key, int x, int y) {
    switch (key) {
        case 'x':
            x_angle += 5;
            break;
        case 'X':
            x_angle -= 5;
            break;
        case 'y':
            y_angle += 5;
            break;
        case 'Y':
            y_angle -= 5;
            break;
        case 'z':
            z_angle += 5;
            break;
        case 'Z':
            z_angle -= 5;
            break;
        case '2':
            color_display_mode = !color_display_mode;
            if (color_display_mode)
                glutDisplayFunc(color_display);
            else
                glutDisplayFunc(display);
            break;
        case '3':
            phong_display_mode = !phong_display_mode;
            glutDisplayFunc(phong_display);
            break;
    }
    glutPostRedisplay();
}

int main(int argc, char *argv[]) {
    glutInit(&argc, argv);
    glutInitWindowSize(500, 500);
    glutInitWindowPosition(250, 250);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
    glutCreateWindow("Penny Model");
    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);
    init();
    glutMainLoop();
    return 0;
}
