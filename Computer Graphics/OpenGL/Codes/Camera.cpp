#include <bits/stdc++.h>
using namespace std;

#include <GL/glut.h>
#include <cmath>


float cameraRadius = 5.0f;
float cameraAngleX = 45.0f;
float cameraAngleY = 45.0f;

float eyeX = 0.0f;
float eyeY = 0.0f;
float eyeZ = 5.0f;

float lateralMovement = 0.0f;

float kx = 0.0f;
float ky = 0.0f;
float kz = 0.0f;

float cx,cy,cz;

float ux = 0.0f;
float uy = 1.0f;
float uz = 0.0f;

int counter = 96;

void updateCameraPosition() {
    float rightX = sin((cameraAngleY + 90.0f) * M_PI / 180.0);
    float rightZ = cos((cameraAngleY + 90.0f) * M_PI / 180.0);

    eyeX = cameraRadius * sin(cameraAngleY * M_PI / 180.0) * cos(cameraAngleX * M_PI / 180.0);
    eyeY = cameraRadius * sin(cameraAngleX * M_PI / 180.0);
    eyeZ = cameraRadius * cos(cameraAngleY * M_PI / 180.0) * cos(cameraAngleX * M_PI / 180.0);

    eyeX += rightX * lateralMovement;
    eyeZ += rightZ * lateralMovement;

    cx = kx;
    cy = ky;
    cz = kz;


}

void tilt(int counter){
    if(counter % 6 == 0){    
        ux = 0.0f;
        uy = 1.0f;
        uz = 0.0f;
    }
    else if(counter % 6 == 1){
        ux = 0.0f;
        uy = 1.0f;
        uz = 1.0f;
    }
    else if(counter % 6 == 2){
        ux = 0.0f;
        uy = 0.0f;
        uz = 1.0f;
    }
    else if(counter % 6 == 3){
        ux = 1.0f;
        uy = 0.0f;
        uz = 1.0f;
    }
    else if(counter % 6 == 4){
        ux = 1.0f;
        uy = 0.0f;
        uz = 0.0f;
    }
    else if(counter % 6 == 5){
        ux = 1.0f;
        uy = 1.0f;
        uz = 0.0f;
    }
}

void axes(){
    glBegin(GL_LINES);
    // X-axis (red)
    glColor3f(1.0, 0.0, 0.0);
    glVertex3f(0.0, 0.0, 0.0);
    glVertex3f(1.0, 0.0, 0.0);

    // Y-axis (green)
    glColor3f(0.0, 1.0, 0.0);
    glVertex3f(0.0, 0.0, 0.0);
    glVertex3f(0.0, 1.0, 0.0);

    // Z-axis (blue)
    glColor3f(0.0, 0.0, 1.0);
    glVertex3f(0.0, 0.0, 0.0);
    glVertex3f(0.0, 0.0, 1.0);

    glEnd();

}

void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    updateCameraPosition();

    glLoadIdentity();
    gluLookAt(eyeX, eyeY, eyeZ, cx, cy, cz, ux, uy, uz);
    
    axes();

    glutSwapBuffers();
}

void reshape(int width, int height) {
    glViewport(0, 0, width, height);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0, (double)width / height, 0.1, 100.0);

    glMatrixMode(GL_MODELVIEW);
}

void keyboard(unsigned char key, int x, int y) {
    switch (key) {
        case 'q':
            exit(0);
            break;
        case '1': 
            cameraAngleY += 0.5f; // Look Left
            break;
        case '2': 
            cameraAngleY += 0.5f; // Look right
            break;
        case '3': // Look up
            if (cameraAngleX < 90.0f) {
                cameraAngleX += 5.0f;
            }
            break;
        case '4': // Look down
            if (cameraAngleX > -90.0f) {
                cameraAngleX -= 5.0f;
            }
            break;
        case '5': // Tilt anticlockwise
            counter++;
            tilt(counter);
            break;
        case '6': // Tilt clockwise
            counter--;
            tilt(counter);
            break;
    }

    glutPostRedisplay();
}

void specialKeys(int key, int x, int y) {
    switch (key) {
        case GLUT_KEY_UP:
            // cameraAngleX += 5.0f;
            cameraRadius -= 0.1f; // Move Forward
            break;
        case GLUT_KEY_DOWN:
            // cameraAngleX -= 5.0f;
            cameraRadius += 0.1f; // Move Backward
            break;
        case GLUT_KEY_LEFT:
            // cameraAngleY += 5.0f;
            kx -= 0.1f; // Move left
            break;
        case GLUT_KEY_RIGHT:
            // cameraAngleY -= 5.0f;
            kx += 0.1f; // Move right
            break;
        case GLUT_KEY_PAGE_UP:
            ky += 0.1f; // Move Up
            break;
        case GLUT_KEY_PAGE_DOWN:
            ky -= 0.1f; // Move down
            break;

    }

    glutPostRedisplay();
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutCreateWindow("OpenGL Camera Movement");

    glEnable(GL_DEPTH_TEST);

    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);
    glutSpecialFunc(specialKeys);

    glutMainLoop();

    return 0;
}
    