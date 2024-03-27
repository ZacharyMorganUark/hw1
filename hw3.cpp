
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <GL/glut.h>

#define ROWS 500
#define COLS 500
#define SCALE_FACTOR 0.001

float Depth[ROWS][COLS];
float x_angle = 0, y_angle = 0, z_angle = 0;

void read_depth_data(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        fprintf(stderr, "Error opening file %s\n", filename);
        exit(1);
    }

    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) {
            if (fscanf(file, "%f", &Depth[i][j]) != 1) {
                fprintf(stderr, "Error reading data from file\n");
                fclose(file);
                exit(1);
            }
        }
    }
    fclose(file);
}

void init() {
    glClearColor(0.0, 0.0, 0.0, 1.0);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-1.0, 1.0, -1.0, 1.0, -1.0, 1.0);
    glEnable(GL_DEPTH_TEST);

    read_depth_data("penny-depth.txt");
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
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
    }
    glutPostRedisplay();
}

int main(int argc, char *argv[]) {
    glutInit(&argc, argv);
    glutInitWindowSize(500, 500);
    glutInitWindowPosition(250, 250);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
    glutCreateWindow("Penny Depth Model");
    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);
    init();
    glutMainLoop();
    return 0;
}
