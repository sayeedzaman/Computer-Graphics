#include <bits/stdc++.h>

using namespace std;

#include <GL/glut.h>
#include <GL/glu.h>
#include <cmath>
#include <cstdlib>


// Global variables
GLfloat ballRadius = 0.4f;
GLfloat ballX = 0.0f, ballY = 0.0f;
GLfloat ballDirectionX = 1.0f, ballDirectionY = 1.0f;
GLfloat ballSpeed = 0.001f;
GLfloat rotationAngle = 0.0f;
// GLfloat rotate ;

int control = 1;

// Function to draw the ball

void drawBall() {
    glPushMatrix();
    glTranslatef(ballX, ballY, 0.0f);

    int slices = 30;  // Number of slices (sectors)
    int stacks = 5;  // Number of stacks

    for (int i = 0; i < stacks; ++i) {
        for (int j = 0; j < slices; ++j) {
            // Generate random RGB values
            float red = static_cast<float>(rand()) / RAND_MAX;
            float green = static_cast<float>(rand()) / RAND_MAX;
            float blue = 0.0;

            glColor3f(red, green, blue); // Random RGB color assignment
            glBegin(GL_QUADS);

            glVertex3f(ballRadius * cos(2 * M_PI * j / slices) * sin(M_PI * i / stacks),
                       ballRadius * sin(2 * M_PI * j / slices) * sin(M_PI * i / stacks),
                       ballRadius * cos(M_PI * i / stacks));

            glVertex3f(ballRadius * cos(2 * M_PI * (j + 1) / slices) * sin(M_PI * i / stacks),
                       ballRadius * sin(2 * M_PI * (j + 1) / slices) * sin(M_PI * i / stacks),
                       ballRadius * cos(M_PI * i / stacks));

            glVertex3f(ballRadius * cos(2 * M_PI * (j + 1) / slices) * sin(M_PI * (i + 1) / stacks),
                       ballRadius * sin(2 * M_PI * (j + 1) / slices) * sin(M_PI * (i + 1) / stacks),
                       ballRadius * cos(M_PI * (i + 1) / stacks));

            glVertex3f(ballRadius * cos(2 * M_PI * j / slices) * sin(M_PI * (i + 1) / stacks),
                       ballRadius * sin(2 * M_PI * j / slices) * sin(M_PI * (i + 1) / stacks),
                       ballRadius * cos(M_PI * (i + 1) / stacks));

            glEnd();
        }
    }

    glPopMatrix();
}

void drawArrow() {
    glPushMatrix();
    glTranslatef(ballX, ballY, 0.0f);

    glColor3f(0.0f, 0.0f, 1.0f); // Blue arrow color
    glBegin(GL_LINES);

    glVertex3f(0.0f, 0.0f, 0.0f);
    glVertex3f(0.5f * cos(rotationAngle * M_PI / 180.0),
               0.5f * sin(rotationAngle * M_PI / 180.0), 0.0f);

    glEnd();
        // Draw Arrowhead

    glBegin(GL_TRIANGLES);
    glVertex3f(0.5f * cos(rotationAngle * M_PI / 180.0),
               0.5f * sin(rotationAngle * M_PI / 180.0), 0.0f);
    
    glVertex3f(0.45f * cos((rotationAngle + 1.50) * M_PI / 180.0),
               0.45f * sin((rotationAngle + 1.50) * M_PI / 180.0), 0.0f);

    glVertex3f(0.45f * cos((rotationAngle - 1.50)* M_PI / 180.0),
               0.45f * sin((rotationAngle - 1.50) * M_PI / 180.0), 0.0f);

    glEnd();

    glPopMatrix();
}
void drawFloor() {
    glColor3f(0.7f, 0.7f, 0.7f); 

    const int gridSize = 8;
    const float tileSize = 2.0f / gridSize;

    glBegin(GL_QUADS);
    for (int i = 0; i < gridSize; ++i) {
        for (int j = 0; j < gridSize; ++j) {
            float x = j * tileSize - 1.0f;
            float y = i * tileSize - 1.0f;

            if ((i + j) % 2 == 0) {
                glColor3f(1.0f, 1.0f, 1.0f); // Light color
            } else {
                glColor3f(0.0f, 0.0f, 0.0f); // Dark color
            }

            glVertex3f(x, y, 0.0f);
            glVertex3f(x + tileSize, y, 0.0f);
            glVertex3f(x + tileSize, y + tileSize, 0.0f);
            glVertex3f(x, y + tileSize, 0.0f);
        }
    }
    glEnd();
}


// Function to handle all drawings
void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Draw the ball
    drawArrow();
    drawBall();
    drawFloor();

    glutSwapBuffers();
}

// Function to update the ball position
void simulation(int value) {
    if(control == 0){
        cout<<"herer i come"<<endl;
        // Update ball position based on direction and speed
        ballDirectionX = cos(rotationAngle * M_PI / 180.0);
        ballDirectionY = sin(rotationAngle * M_PI / 180.0);    
        ballX += ballSpeed * ballDirectionX;
        ballY += ballSpeed * ballDirectionY;

        // Check for collision with window boundaries
        if (ballX + ballRadius > 1.0f || ballX - ballRadius < -1.0f ) {
            // cout<<"hit"<<endl;
            rotationAngle  = 180 - rotationAngle;
            // cout<<ballDirectionX<<endl;
            // cout<<rotationAngle<<endl;
        }
        if (ballY + ballRadius > 1.0f || ballY - ballRadius < -1.0f) {
            // ballDirectionY = sin((rotationAngle + 180.0) * M_PI / 180.0);
            if(rotationAngle > 0.0){
                cout<<"hit"<<endl;
                cout << rotationAngle<< endl;
                GLfloat rotate = 2 * rotationAngle; 
                cout<<rotate<<endl;
                rotationAngle = rotationAngle - 180.0 + rotate;
                cout<< rotationAngle<< endl;
            }
            else {
                GLfloat rot = 2*rotationAngle;
                rotationAngle = rotationAngle + 180.0 - rot;
            }
        }

        glutPostRedisplay();
        glutTimerFunc(16, simulation, 0); 
    }
}



void handleKeyboard(unsigned char key, int x, int y) {
    
    if(control == 1){
        cout<<"control: "<<control<< endl;
        switch (key) {
            case 'j': // Rotate counterclockwise
                rotationAngle += 5.0f;
                break;
            case 'l': // Rotate clockwise
                rotationAngle -= 5.0f;
                break;
            case 'i': // Go forward
                ballDirectionX = cos(rotationAngle * M_PI / 180.0);
                ballDirectionY = sin(rotationAngle * M_PI / 180.0);
                ballX += ballSpeed * ballDirectionX;
                ballY += ballSpeed * ballDirectionY;
                break;
            case 'k': // Go backward
                ballDirectionX = cos(rotationAngle * M_PI / 180.0);
                ballDirectionY = sin(rotationAngle * M_PI / 180.0);
                ballX -= ballSpeed * ballDirectionX;
                ballY -= ballSpeed * ballDirectionY;
                break;
            case ' ':
                control = 0;
                glutTimerFunc(100,simulation, 0);
                break;
        }
    }
    else if( control == 0){
        cout<<"control: "<<control<< endl;
        
        switch (key)
        {
        case 'j':
            rotationAngle += 5.0f;
            break;

        case 'l':
            rotationAngle -= 5.0f;
            break;
        
        case ' ':
            control = 1;
            break;
        }
    }

    glutPostRedisplay();
}


int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);

    glutInitWindowPosition(200, 200);
    glutInitWindowSize(600, 600);
    glutCreateWindow("rolling_ball");

    glClearColor(1.0f, 1.0f, 1.0f, 1.0f); // White background
    glEnable(GL_DEPTH_TEST);

    glMatrixMode(GL_PROJECTION);
    gluOrtho2D(-1.0, 1.0, -1.0, 1.0);

    glutDisplayFunc(display);

    //glutKeyboardFunc(keyboard);
    glutKeyboardFunc(handleKeyboard);

    glutMainLoop();
    return 0;
}
