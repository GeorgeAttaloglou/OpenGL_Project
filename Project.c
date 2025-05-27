#include <GL/freeglut.h>
#include <math.h>
#include <stdio.h>
#include <stdbool.h>

#define PI 3.1415926

float rotate_x, rotate_y = 0;
bool isIridescent = false;
bool isSpinning = false;
float scale = 1.0;
float translate_x, translate_y = 0.0f;

enum ShapeType {
    RECTANGLE,
    HEXAGON,
    CIRCLE,
    CUBE,
    PYRAMID,
    SPHERE
};


enum ShapeType currentShape = CUBE; // Default shape: CUBE

GLfloat currentColor[3] = {0.0f, 1.0f, 1.0f}; // Initial color: cyan

void drawSimplePolygon(int n, float radius) {
    glColor3fv(currentColor); 
    glBegin(GL_POLYGON);
    for (int i = 0; i < n; ++i) {
        float theta = 2.0 * PI * i / n; // Calculate the angle for each vertex
        float x = cos(theta) * radius; // Calculate the x-coordinate
        float y = sin(theta) * radius; // Calculate the y-coordinate
        glVertex2f(x, y); // Draw the vertex
    }
    glEnd();

    glColor3f(0.0f, 0.0f, 0.0f);
    glLineWidth(2.0f); // Optional: Set line thickness
    glBegin(GL_LINE_LOOP);
    for (int i = 0; i < n; ++i) {
        float theta = 2.0 * PI * i / n; // Calculate the angle for each vertex
        float x = cos(theta) * radius; // Calculate the x-coordinate
        float y = sin(theta) * radius; // Calculate the y-coordinate
        glVertex2f(x, y); // Draw the vertex
    }
    glEnd();
}

void drawCircle(float radius) {
    drawSimplePolygon(50000, radius); 
}


void drawCube3d(float size) {
    float halfSize = size / 2.0f;

    GLfloat vertices[][3] = {
        // Front face
        {-halfSize, -halfSize, halfSize}, // 0
        {halfSize, -halfSize, halfSize},  // 1
        {halfSize, halfSize, halfSize},   // 2
        {-halfSize, halfSize, halfSize},  // 3

        // Back face
        {-halfSize, -halfSize, -halfSize}, // 4
        {halfSize, -halfSize, -halfSize},  // 5
        {halfSize, halfSize, -halfSize},   // 6
        {-halfSize, halfSize, -halfSize}   // 7
    };

    GLuint faces[][4] = {
        {0, 1, 2, 3}, // Front face
        {7, 6, 5, 4}, // Back face
        {0, 3, 7, 4}, // Left face
        {1, 5, 6, 2}, // Right face
        {3, 2, 6, 7}, // Top face
        {0, 4, 5, 1}  // Bottom face
    };

    // Draw the cube with colors
    for(int i = 0; i < 6; i++) {
        glBegin(GL_POLYGON);
        glColor3fv(currentColor);
        for(int j = 0; j < 4; j++) {
            glVertex3fv(vertices[faces[i][j]]); // Draw each vertex of the face
        }
        glEnd();
    }

    // Draw the edges of the cube in black
    glColor3f(0.0f, 0.0f, 0.0f);
    glBegin(GL_LINES);
    for(int i = 0; i < 4; i++) {
        glVertex3fv(vertices[i]);
        glVertex3fv(vertices[(i + 1) % 4]);
        glVertex3fv(vertices[i + 4]);
        glVertex3fv(vertices[((i + 1) % 4) + 4]);
        glVertex3fv(vertices[i]);
        glVertex3fv(vertices[i + 4]);
    }
    glEnd();
}


void drawPyramid(float size) {
    // Front
    glBegin(GL_TRIANGLES);
    glColor3fv(currentColor);
    glVertex3f(0.0f, size, 0.0f);
    glVertex3f(-size, -size, size);
    glVertex3f(size, -size, size);
    glEnd();

    // Right
    glBegin(GL_TRIANGLES);
    glColor3fv(currentColor);
    glVertex3f(0.0f, size, 0.0f);
    glVertex3f(size, -size, size);
    glVertex3f(size, -size, -size);
    glEnd();

    // Back
    glBegin(GL_TRIANGLES);
    glColor3fv(currentColor);
    glVertex3f(0.0f, size, 0.0f);
    glVertex3f(size, -size, -size);
    glVertex3f(-size, -size, -size);
    glEnd();

    // Left
    glBegin(GL_TRIANGLES);
    glColor3fv(currentColor);
    glVertex3f(0.0f, size, 0.0f);
    glVertex3f(-size, -size, -size);
    glVertex3f(-size, -size, size);
    glEnd();

    // Bottom edges
    glBegin(GL_LINES);
    glColor3f(0.0f, 0.0f, 0.0f); // Black color for edges

    // Base edges
    glVertex3f(-size, -size, size);
    glVertex3f(size, -size, size);

    glVertex3f(size, -size, size);
    glVertex3f(size, -size, -size);

    glVertex3f(size, -size, -size);
    glVertex3f(-size, -size, -size);

    glVertex3f(-size, -size, -size);
    glVertex3f(-size, -size, size);

    // Connecting edges
    glVertex3f(0.0f, size, 0.0f);
    glVertex3f(-size, -size, size);

    glVertex3f(0.0f, size, 0.0f);
    glVertex3f(size, -size, size);

    glVertex3f(0.0f, size, 0.0f);
    glVertex3f(size, -size, -size);

    glVertex3f(0.0f, size, 0.0f);
    glVertex3f(-size, -size, -size);

    glEnd();
}


void drawSphere(float radius) {
    glColor3fv(currentColor);
    glutSolidSphere(radius, 50, 50);
}


void display() {
    // Clear the screen and set up the view
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    // Set the camera position
    glColor3f(0.0f,0.0f,0.0f);
    glRasterPos2f(-50, 50);
    glutBitmapString(GLUT_BITMAP_TIMES_ROMAN_24, (void *)"AM: 2397");

    // Draw the current shape
    glMatrixMode(GL_MODELVIEW);

    // Apply transformations
    glPushMatrix();
    glTranslatef(translate_x, translate_y, 0.0f); // Translate the object
    glScalef(scale, scale, scale); // Scale the object
    glRotatef(45,1.0,1.0,0.0);
    glRotatef(rotate_x,1.0,0.0,0.0);
    glRotatef(rotate_y,0.0,1.0,0.0);

    if (currentShape == RECTANGLE) {
        glRotatef(45, 0.0, 0.0, 1.0);
        drawSimplePolygon(4, 20);
    } else if (currentShape == HEXAGON) {
        glRotatef(45, 0.0, 0.0, 1.0);
        drawSimplePolygon(6, 20);
    } else if (currentShape == CIRCLE) {
        glRotatef(45, 0.0, 0.0, 1.0);
        drawCircle(20);
    } else if (currentShape == CUBE) {
        drawCube3d(20);
    } else if (currentShape == PYRAMID) {
        drawPyramid(20);
    } else if (currentShape == SPHERE) {
        drawSphere(20);
    }

    glPopMatrix();

    glutSwapBuffers();
}


void specialInput(int key, int x, int y) {
    switch(key) {
        case GLUT_KEY_LEFT:
            translate_x -= 0.5; // Translate to the left
            break;
        case GLUT_KEY_RIGHT:
            translate_x += 0.5; // Translate to the right
            break;
        case GLUT_KEY_UP:
            translate_y += 0.5; // Translate up
            break;
        case GLUT_KEY_DOWN:
            translate_y -= 0.5; // Translate down
            break;
        case GLUT_KEY_F1:
            currentShape = RECTANGLE; // Show rectangle
            break;
        case GLUT_KEY_F2:
            currentShape = HEXAGON; // Show hexagon
            break;
        case GLUT_KEY_F3:
            currentShape = CIRCLE; // Show circle
            break;
        case GLUT_KEY_F4:
            currentShape = CUBE; // Show cube
            break;
        case GLUT_KEY_F5:
            currentShape = PYRAMID; // Show pyramid
            break;
        case GLUT_KEY_F6:
            currentShape = SPHERE; // Show sphere
            break;
    }

    glutPostRedisplay();
}


void keyboard (unsigned char key, int x, int y) {
    switch(key) {
        case '1': // Cyan
            currentColor[0] = 0.0f;
            currentColor[1] = 1.0f;
            currentColor[2] = 1.0f;
            break;
        case '2': // Red
            currentColor[0] = 1.0f;
            currentColor[1] = 0.0f;
            currentColor[2] = 0.0f;
            break;
        case '3': // Green
            currentColor[0] = 0.0f;
            currentColor[1] = 1.0f;
            currentColor[2] = 0.0f;
            break;
        case '4': // Orange
            currentColor[0] = 1.0f;
            currentColor[1] = 0.5f;
            currentColor[2] = 0.0f;
            break;
         case '5': // Iridescent (automatic color change)
            isIridescent = !isIridescent;
            break;
        case '+': // Increase scale
            if (scale < 3.0){
                scale += 0.1;
            } else {
                scale = 3.0;
            }
            break;
        case '-': // Decrease scale
            if (scale > 0.2) {
                scale -= 0.1;
            } else {
                scale = 0.2;
            }
            break;
        case 27: // ESC
            exit(0);
            break;
        case 32: // Pause/Resume rotation
            isSpinning = !isSpinning;
            break;
    }

    glutPostRedisplay();
}


void changeColor(int value){
    if (isIridescent){
        // We initiate the colors with different values so that they change at different rates
        // And we dont just have black to white

        currentColor[0] += 0.001;
        currentColor[1] += 0.009;
        currentColor[2] += 0.005;
        if(currentColor[0] > 1.0f){
            currentColor[0] = 0.0f;
        }
        if(currentColor[1] > 1.0f){
            currentColor[1] = 0.0f;
        }
        if(currentColor[2] > 1.0f){
            currentColor[2] = 0.0f;
        }
    }
    glutPostRedisplay();
    glutTimerFunc(10, changeColor, 10);
}


void spinShape() {
    if (isSpinning) {
        if (currentShape == RECTANGLE || currentShape == HEXAGON || currentShape == CIRCLE) {
            rotate_x += 0.1;
        } else {
            rotate_y += 0.1;
        }
        glutPostRedisplay();
    }
}


int main(int argc, char** argv) {
    glutInit(&argc,argv); // Initialize GLUT
    glutInitWindowPosition(100,100); // Set window position
    glutInitWindowSize(800,800); // Set window size
    glutSetOption(GLUT_MULTISAMPLE, 8); // Enable multisampling for anti-aliasing
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH | GLUT_MULTISAMPLE); // Set display mode with double buffering, RGB color, depth buffer, and multisampling
    glutCreateWindow("Shape Viewer"); // Create the window with a title
    glMatrixMode(GL_PROJECTION); // Set the projection matrix

    glOrtho(-5,105,-5,105, -100, 100); // Set orthographic projection
    glTranslatef(50,50,0); // Translate the scene to center it
    glEnable(GL_DEPTH_TEST); // Enable depth testing for 3D rendering
    glEnable(GL_MULTISAMPLE); // Enable multisampling for anti-aliasing
    glClearColor(1.0f,1.0f,1.0f,0); // Set the clear color to white

    glutTimerFunc(10, changeColor, 10); // Start the timer for color change
    glutSpecialFunc(specialInput); // Register the special input function for arrow keys and function keys
    glutKeyboardFunc(keyboard); // Register the keyboard input function
    glutDisplayFunc(display); // Register the display function
    glutIdleFunc(spinShape); // Register the idle function
    glutMainLoop(); // Enter the GLUT main loop
    // This line will never be reached, but it's good practice to return an int from main   
    return 0;
}
