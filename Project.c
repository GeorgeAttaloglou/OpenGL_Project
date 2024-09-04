#include <GL/freeglut.h>
#include <math.h>
#include <stdio.h>
#include <stdbool.h>

#define PI 3.1415926

float rotate_x, rotate_y = 0;
bool isIridescent = false;
bool isSpinning = false; // Έλεγχος αν ο κύβος περιστρέφεται
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


enum ShapeType currentShape = CUBE; // Αρχική μορφή: κύβος

GLfloat currentColor[3] = {0.0f, 1.0f, 1.0f}; // Αρχικό χρώμα: κυανό

void drawSimplePolygon(int n, float radius) {
    glColor3fv(currentColor); 
    glBegin(GL_POLYGON);
    for (int i = 0; i < n; ++i) {
        float theta = 2.0 * PI * i / n; // υπολογίζουμε τη γωνία για κάθε κορυφή
        float x = cos(theta) * radius; // υπολογίζουμε την x-συντεταγμένη
        float y = sin(theta) * radius; // υπολογίζουμε την y-συντεταγμένη
        glVertex2f(x, y); // Σχεδιάζουμε την κορυφή
    }
    glEnd();

    glColor3f(0.0f, 0.0f, 0.0f);
    glLineWidth(2.0f); // Προαιρετικό: Ρυθμίστε το πάχος της γραμμής
    glBegin(GL_LINE_LOOP);
    for (int i = 0; i < n; ++i) {
        float theta = 2.0 * PI * i / n; // υπολογίζουμε τη γωνία για κάθε κορυφή
        float x = cos(theta) * radius; // υπολογίζουμε την x-συντεταγμένη
        float y = sin(theta) * radius; // υπολογίζουμε την y-συντεταγμένη
        glVertex2f(x, y); // Σχεδιάζουμε την κορυφή
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
        glColor3fv(currentColor); // Χρησιμοποιούμε το τρέχον χρώμα
        for(int j = 0; j < 4; j++) {
            glVertex3fv(vertices[faces[i][j]]);
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
    glColor3fv(currentColor); // Χρησιμοποιούμε το τρέχον χρώμα
    glVertex3f(0.0f, size, 0.0f);
    glVertex3f(-size, -size, size);
    glVertex3f(size, -size, size);
    glEnd();

    // Right
    glBegin(GL_TRIANGLES);
    glColor3fv(currentColor); // Χρησιμοποιούμε το τρέχον χρώμα
    glVertex3f(0.0f, size, 0.0f);
    glVertex3f(size, -size, size);
    glVertex3f(size, -size, -size);
    glEnd();

    // Back
    glBegin(GL_TRIANGLES);
    glColor3fv(currentColor); // Χρησιμοποιούμε το τρέχον χρώμα
    glVertex3f(0.0f, size, 0.0f);
    glVertex3f(size, -size, -size);
    glVertex3f(-size, -size, -size);
    glEnd();

    // Left
    glBegin(GL_TRIANGLES);
    glColor3fv(currentColor); // Χρησιμοποιούμε το τρέχον χρώμα
    glVertex3f(0.0f, size, 0.0f);
    glVertex3f(-size, -size, -size);
    glVertex3f(-size, -size, size);
    glEnd();

    // Bottom edges
    glBegin(GL_LINES);
    glColor3f(0.0f, 0.0f, 0.0f); // Μαύρο χρώμα για τις ακμές

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
    glColor3fv(currentColor); // Χρησιμοποιούμε το τρέχον χρώμα
    glutSolidSphere(radius, 50, 50);
}


void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glColor3f(0.0f,0.0f,0.0f);
    glRasterPos2f(-50, 50);
    glutBitmapString(GLUT_BITMAP_TIMES_ROMAN_24, (void *)"AM: 2397");

    glMatrixMode(GL_MODELVIEW);

    glPushMatrix();
    glTranslatef(translate_x, translate_y, 0.0f); // Μετατόπιση του αντικειμένου
    glScalef(scale, scale, scale); // Κλιμάκωση του αντικειμένου
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
            translate_x -= 0.5; // Μετατόπιση προς τα αριστερά
            break;
        case GLUT_KEY_RIGHT:
            translate_x += 0.5; // Μετατόπιση προς τα δεξιά
            break;
        case GLUT_KEY_UP:
            translate_y += 0.5; // Μετατόπιση προς τα πάνω
            break;
        case GLUT_KEY_DOWN:
            translate_y -= 0.5; // Μετατόπιση προς τα κάτω
            break;
        case GLUT_KEY_F1:
            currentShape = RECTANGLE; // Εμφάνιση ορθογωνίου
            break;
        case GLUT_KEY_F2:
            currentShape = HEXAGON; // Εμφάνιση εξαγώνου
            break;
        case GLUT_KEY_F3:
            currentShape = CIRCLE; // Εμφάνιση κύκλου
            break;
        case GLUT_KEY_F4:
            currentShape = CUBE; // Εμφάνιση κύβου
            break;
        case GLUT_KEY_F5:
            currentShape = PYRAMID; // Εμφάνιση πυραμίδας
            break;
        case GLUT_KEY_F6:
            currentShape = SPHERE; // Εμφάνιση σφαίρας
            break;
    }

    glutPostRedisplay();
}


void keyboard (unsigned char key, int x, int y) {
    switch(key) {
        case '1': // κυανό
            currentColor[0] = 0.0f;
            currentColor[1] = 1.0f;
            currentColor[2] = 1.0f;
            break;
        case '2': // κόκκινο
            currentColor[0] = 1.0f;
            currentColor[1] = 0.0f;
            currentColor[2] = 0.0f;
            break;
        case '3': // πράσινο
            currentColor[0] = 0.0f;
            currentColor[1] = 1.0f;
            currentColor[2] = 0.0f;
            break;
        case '4': // πορτοκαλί
            currentColor[0] = 1.0f;
            currentColor[1] = 0.5f;
            currentColor[2] = 0.0f;
            break;
         case '5': // ιριδίζον (αλλαγή χρώματος αυτόματα)
            isIridescent = !isIridescent;
            break;       
        case '+': // Αύξηση κλίμακας
            if (scale < 3.0){
                scale += 0.1;
            } else {
                scale = 3.0;
            }
            break;
        case '-': // Μείωση κλίμακας
            if (scale > 0.2) {
                scale -= 0.1;
            } else {
                scale = 0.2;
            }
            break;
        case 27: // ESC
            exit(0);
            break;
        case 32: // Παύση/Συνέχιση περιστροφής
            isSpinning = !isSpinning;
            break;
    }

    glutPostRedisplay();
}


void changeColor(){
    if (isIridescent){
        // Για να μην ξεκινάει απο άσπρο και να καταλήγει σε μαύρο αρχικοποιούμε τα χρώματα με διαφορετικές τιμές

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
    glutInit(&argc,argv);
    glutInitWindowPosition(100,100);
    glutInitWindowSize(800,800);
    glutSetOption(GLUT_MULTISAMPLE, 8);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH | GLUT_MULTISAMPLE);
    glutCreateWindow("Atomikh Askhsh Grafika");
    glMatrixMode(GL_PROJECTION);

    glOrtho(-5,105,-5,105, -100, 100);
    glTranslatef(50,50,0);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_MULTISAMPLE);
    glClearColor(1.0f,1.0f,1.0f,0);

    glutTimerFunc(10, changeColor, 10);
    glutSpecialFunc(specialInput);
    glutKeyboardFunc(keyboard);
    glutDisplayFunc(display);
    glutIdleFunc(spinShape);
    glutMainLoop();
    return 0;
}
