//---------------------------------------
// Program: hw3.cpp using balls.cpp as skeleton
// Purpose: 
// Author: Zachary Morgan
// Date: 10 April 2024
//---------------------------------------
#include <iostream>
#include <fstream>
#include <vector>
#include <GL/glut.h>
#include <SOIL/SOIL.h>

// Define maze dimensions
int R, C;
int SR, SC; // Starting row and column

// Define the maze array
std::vector<std::vector<char>> maze;

// Define texture IDs
GLuint rockTexture, brickTexture, woodTexture, floorTexture;

// Define rotation angles
float xRot = 0.0, yRot = 0.0, zRot = 0.0;

// Function to read the maze file
void readMazeFile(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error opening file: " << filename << std::endl;
        exit(1);
    }

    // Read maze dimensions and starting location
    file >> R >> C >> SR >> SC;

    // Resize maze array
    maze.resize(R, std::vector<char>(C));

    // Read maze layout
    for (int i = 0; i < R; ++i) {
        for (int j = 0; j < C; ++j) {
            file >> maze[i][j];
        }
    }

    file.close();
}

// Function to load texture images
GLuint loadTexture(const char* filename) {
    GLuint textureID;
    textureID = SOIL_load_OGL_texture(filename, SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_INVERT_Y);
    if (textureID == 0) {
        std::cerr << "Error loading texture: " << SOIL_last_result() << std::endl;
    }
    return textureID;
}

// Function to initialize OpenGL
void initOpenGL() {
    glEnable(GL_TEXTURE_2D);
    glEnable(GL_DEPTH_TEST);

    // Load texture images
    rockTexture = loadTexture("rock.jpg");
    brickTexture = loadTexture("brick.jpg");
    woodTexture = loadTexture("wood.jpg");
    floorTexture = loadTexture("grass.jpg");
}

// Function to draw the maze
void drawMaze() {
    glPushMatrix();

    // Apply rotations
    glRotatef(xRot, 1.0, 0.0, 0.0);
    glRotatef(yRot, 0.0, 1.0, 0.0);
    glRotatef(zRot, 0.0, 0.0, 1.0);

    // Draw floor
    glBindTexture(GL_TEXTURE_2D, floorTexture);
    glBegin(GL_QUADS);
    glTexCoord2f(0.0, 0.0); glVertex3f(-0.5 * C, 0.0, -0.5 * R);
    glTexCoord2f(1.0, 0.0); glVertex3f(0.5 * C, 0.0, -0.5 * R);
    glTexCoord2f(1.0, 1.0); glVertex3f(0.5 * C, 0.0, 0.5 * R);
    glTexCoord2f(0.0, 1.0); glVertex3f(-0.5 * C, 0.0, 0.5 * R);
    glEnd();

    // Draw walls
    for (int i = 0; i < R; ++i) {
        for (int j = 0; j < C; ++j) {
            if (maze[i][j] != ' ') {
                glPushMatrix();
                glTranslatef(j - 0.5 * C, 0.5, i - 0.5 * R);

                switch (maze[i][j]) {
                    case 'r':
                        glBindTexture(GL_TEXTURE_2D, rockTexture);
                        break;
                    case 'b':
                        glBindTexture(GL_TEXTURE_2D, brickTexture);
                        break;
                    case 'w':
                        glBindTexture(GL_TEXTURE_2D, woodTexture);
                        break;
                }

                glBegin(GL_QUADS);
                glTexCoord2f(0.0, 0.0); glVertex3f(-0.5, 0.0, -0.5);
                glTexCoord2f(1.0, 0.0); glVertex3f(0.5, 0.0, -0.5);
                glTexCoord2f(1.0, 1.0); glVertex3f(0.5, 1.0, -0.5);
                glTexCoord2f(0.0, 1.0); glVertex3f(-0.5, 1.0, -0.5);

                glTexCoord2f(0.0, 0.0); glVertex3f(-0.5, 0.0, 0.5);
                glTexCoord2f(1.0, 0.0); glVertex3f(0.5, 0.0, 0.5);
                glTexCoord2f(1.0, 1.0); glVertex3f(0.5, 1.0, 0.5);
                glTexCoord2f(0.0, 1.0); glVertex3f(-0.5, 1.0, 0.5);

                glTexCoord2f(0.0, 0.0); glVertex3f(-0.5, 0.0, -0.5);
                glTexCoord2f(1.0, 0.0); glVertex3f(-0.5, 0.0, 0.5);
                glTexCoord2f(1.0, 1.0); glVertex3f(-0.5, 1.0, 0.5);
                glTexCoord2f(0.0, 1.0); glVertex3f(-0.5, 1.0, -0.5);

                glTexCoord2f(0.0, 0.0); glVertex3f(0.5, 0.0, -0.5);
                glTexCoord2f(1.0, 0.0); glVertex3f(0.5, 0.0, 0.5);
                glTexCoord2f(1.0, 1.0); glVertex3f(0.5, 1.0, 0.5);
                glTexCoord2f(0.0, 1.0); glVertex3f(0.5, 1.0, -0.5);

                glTexCoord2f(0.0, 0.0); glVertex3f(-0.5, 0.0, -0.5);
                glTexCoord2f(1.0, 0.0); glVertex3f(-0.5, 0.0, 0.5);
                glTexCoord2f(1.0, 1.0); glVertex3f(0.5, 0.0, 0.5);
                glTexCoord2f(0.0, 1.0); glVertex3f(0.5, 0.0, -0.5);
                glEnd();

                glPopMatrix();
            }
        }
    }

    glPopMatrix();
}

// Function to display
void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    gluLookAt(0.0, 3.0, 5.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);

    drawMaze();

    glutSwapBuffers();
}

// Function to handle keyboard input
void keyboard(unsigned char key, int x, int y) {
    switch (key) {
        case 'x':
            xRot -= 5.0;
            break;
        case 'X':
            xRot += 5.0;
            break;
        case 'y':
            yRot -= 5.0;
            break;
        case 'Y':
            yRot += 5.0;
            break;
        case 'z':
            zRot -= 5.0;
            break;
        case 'Z':
            zRot += 5.0;
            break;
    }
    glutPostRedisplay();
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(800, 600);
    glutCreateWindow("Maze Viewer");

    initOpenGL();
    readMazeFile("maze.txt");

    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);

    glutMainLoop();
    return 0;
}
