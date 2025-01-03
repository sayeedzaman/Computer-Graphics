#include <bits/stdc++.h>

using namespace std;
#include <GL/glut.h>  // GLUT, include glu.h and gl.h
#include <cmath>




float px, py,pz, lx,ly,lz,rx,ry,rz,ux,uy,uz;

double angleRotationZ = M_PI / 4;

void initGL() {
    // Set "clearing" or background color
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);   // Black and opaque
    glEnable(GL_DEPTH_TEST);   // Enable depth testing for z-culling

    px = 6; py = 0; pz = 2;
    lx = -1; ly =0; lz =0;
    rx = 0; ry = 1; rz =0;
    ux =0; uy =0; uz =1;  

}



void drawTriangle()
{
    //glColor3f(1.0,0.0,0.0);
	glBegin(GL_TRIANGLES);{
		glVertex3f(1,0,0);
		glVertex3f(0,1,0);
		glVertex3f(0,0,1);
	}glEnd();
}
int numSegments = 36;


void single_cylinder(double height, double radius, int segments) {
    GLfloat points[(segments + 1) * 3];

    double offset = 70.5287794 * M_PI / 180.0;

    for (int i = 0; i < segments + 1; i++) {
        double theta = -offset / 2 + i * offset / segments;
        points[i * 3] = radius * cos(theta);
        points[i * 3 + 1] = radius * sin(theta);
        points[i * 3 + 2] = 0.0f;
    }

    glBegin(GL_QUADS);
    for (int i = 0; i < segments; i++) {
        glVertex3f(points[i * 3], points[i * 3 + 1], height / 2);
        glVertex3f(points[i * 3], points[i * 3 + 1], -height / 2);
        glVertex3f(points[(i + 1) * 3], points[(i + 1) * 3 + 1], -height / 2);
        glVertex3f(points[(i + 1) * 3], points[(i + 1) * 3 + 1], height / 2);
    }
    glEnd();
}



void drawSphereQuad(double radius, int segments) {
    double points[segments + 1][segments + 1][3];

    for (int i = 0; i <= segments; i++) {
        for (int j = 0; j <= segments; j++) {
            double x = -1 + (double)i / segments * 2;
            double y = -1 + (double)j / segments * 2;
            double z = 1;

            // Normalize the vector
            double vectorLength = sqrt(x * x + y * y + z * z);
            x /= vectorLength;
            y /= vectorLength;
            z /= vectorLength;

            // Scale the vector by the radius
            x *= radius;
            y *= radius;
            z *= radius;

            points[i][j][0] = x;
            points[i][j][1] = y;
            points[i][j][2] = z;
        }
    }

    // Draw quads using generated points
    for (int i = 0; i < segments; i++) {
        for (int j = 0; j < segments; j++) {
            glBegin(GL_QUADS);
            {
                glVertex3dv(points[i][j]);
                glVertex3dv(points[i][j + 1]);
                glVertex3dv(points[i + 1][j + 1]);
                glVertex3dv(points[i + 1][j]);
            }
            glEnd();
        }
    }
}

double maxTriangleLength = 1.6;
double triangleLength = 1.6;

double maxSphereRadius = maxTriangleLength / sqrt(3.0);
double sphereRadius = 0;
double sphereStep = maxSphereRadius/16.0;




void drawCylinders(){

    glColor3f(1.0f, 1.0f, 0.0f);

    for(int i=0;i<4;i++){

        glPushMatrix();{
            
            glRotatef(45+i*90,0,1,0);
            glTranslatef(triangleLength/sqrt(2),0,0);
            single_cylinder(triangleLength*sqrt(2),sphereRadius,100);
        }glPopMatrix();
        
    }


    for(int j = 0; j < 2; j++){
        for(int i=0;i<4;i++){

            glPushMatrix();{

            if(j == 0){
                glRotatef(90, 1, 0, 0);
            }
            else {
                glRotatef(90, 0, 0, 1);
            }
            glRotatef(45+i*90,0,1,0);
            glTranslatef(triangleLength/sqrt(2),0,0);
            single_cylinder(triangleLength*sqrt(2),sphereRadius,100);
            }glPopMatrix();

        }
    }

}



void drawSpheres(){

    for(int i=0;i<4;i++){
        
        glPushMatrix();{
            glColor3f(0, i%2, (i+1)%2);     // blue / green
            glRotatef(90*i,0,1,0);
            glTranslatef(0,0,triangleLength);
            drawSphereQuad(sphereRadius,100);

        }glPopMatrix();

    }

    for(int i=0;i<2;i++){
        
        glPushMatrix();{
            glColor3f(1.0f, 0.0f, 0.0f);     // red
            glRotatef(90+180*i,1,0,0);
            glTranslatef(0,0,triangleLength);
            drawSphereQuad(sphereRadius,100);
        }glPopMatrix();

    }

}


void drawPyramids(){
    double diff = (maxTriangleLength - triangleLength) / 3.0;
    // diff = diff/3.0;
    // cout<<diff<<endl;

    for(int j =0; j < 2; j++){
        for(int i=0;i<4;i++){

            glPushMatrix();{
                glColor3f((i+1-j)%2, (i+j)%2, 1.0f);  // cyan / pruple
                glRotatef(90*i,0,1,0);
                if(j != 0){
                    glRotatef(180,1,0,1);
                }
                glTranslatef(diff,diff,diff);
                glScaled(triangleLength,triangleLength,triangleLength);
                drawTriangle();
            }glPopMatrix();

        }
    }

}




void display() {
    
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);             
    glLoadIdentity();                       
   
    gluLookAt(px,py,pz,px +lx, py + ly, pz+lz, ux,uy,uz);
    // draw
    glRotatef(angleRotationZ * 180 / M_PI, 0,0 , 1);
    drawPyramids();
    drawSpheres();
    drawCylinders();
    // drawAll();

    glutSwapBuffers();  
}

/* Handler for window re-size event. Called back when the window first appears and
   whenever the window is re-sized with its new width and height */
void reshape(GLsizei width, GLsizei height) {  // GLsizei for non-negative integer
    // Compute aspect ratio of the new window
    if (height == 0) height = 1;                // To prevent divide by 0
    GLfloat aspect = (GLfloat)width / (GLfloat)height;

    // Set the viewport to cover the new window
    glViewport(0, 0, width, height);

    // Set the aspect ratio of the clipping area to match the viewport
    glMatrixMode(GL_PROJECTION);  // To operate on the Projection matrix
    glLoadIdentity();             // Reset the projection matrix

   gluPerspective(80,	aspect,	1, 100.0);
}

/* Callback handler for normal-key event */
void keyboardListener(unsigned char key, int x, int y) {
    // double v = 0.1;
    double rotationangle = 5.0f;
    double rate = (rotationangle * M_PI) / 180.0;
    //Point oldEye = eye;
   

    // double s;
    //float v = 0.1;
    switch (key) {

    case 'a':
        angleRotationZ += 0.01f;
        break;
    case 'd':
        angleRotationZ -= 0.01f;
        break;
    case 'w':
        pz += 0.1;
        break;
    case 's':
        pz -= 0.1;
        break;
    // Control eye (location of the eye)
    // control eye.x
   case '1':

            rx = rx *cos(rate) + lx *sin(rate);
            ry = ry *cos(rate) + ly *sin(rate);
            rx = rz *cos(rate) + lz *sin(rate);

            lx = lx*cos(rate)-rx*sin(rate);
            ly = ly*cos(rate)-ry*sin(rate);
            lz = lz*cos(rate)-rz*sin(rate);
			break;

        case '2':

            rx = rx *cos(-rate) + lx *sin(-rate);
            ry = ry *cos(-rate) + ly *sin(-rate);
            rx = rz *cos(-rate) + lz *sin(-rate);

            lx = lx*cos(-rate)-rx*sin(-rate);
            ly = ly*cos(rate)-ry*sin(-rate);
            lz = lz*cos(-rate)-rz*sin(-rate);
			break;

        case '3':

            lx = lx *cos(rate) + ux *sin(rate);
            ly = ly *cos(rate) + uy *sin(rate);
            lz = lz *cos(rate) + uz *sin(rate);

            ux = ux*cos(rate)-lx*sin(rate);
            uy = uy*cos(rate)-ly*sin(rate);
            uz = uz*cos(rate)-lz*sin(rate);
			break;

        case '4':

            lx = lx *cos(-rate) + ux *sin(-rate);
            ly = ly *cos(-rate) + uy *sin(-rate);
            lz = lz *cos(-rate) + uz *sin(-rate);

            ux = ux*cos(-rate)-lx*sin(-rate);
            uy = uy*cos(-rate)-ly*sin(-rate);
            uz = uz*cos(-rate)-lz*sin(-rate);
			break;

        case '5':

            ux = ux *cos(rate) + rx *sin(rate);
            uy = uy *cos(rate) + ry *sin(rate);
            uz = uz *cos(rate) + rz *sin(rate);

            rx = rx*cos(rate)-ux*sin(rate);
            ry = ry*cos(rate)-uy*sin(rate);
            rz = rz*cos(rate)-uz*sin(rate);
			break;

        case '6':

            ux = ux *cos(-rate) + rx *sin(-rate);
            uy = uy *cos(-rate) + ry *sin(-rate);
            uz = uz *cos(-rate) + rz *sin(-rate);

            rx = rx*cos(-rate)-ux*sin(-rate);
            ry = ry*cos(-rate)-uy*sin(-rate);
            rz = rz*cos(-rate)-uz*sin(-rate);
			
			break;


   
    case ',':
        if(triangleLength > 0){
            triangleLength -= 0.1;
            sphereRadius += sphereStep;
        }
        break;
    case '.':
        if(triangleLength < maxTriangleLength ){
            triangleLength += 0.1;
            sphereRadius -= sphereStep;
        }
        break;
    // Control exit
    case 27:    // ESC key
        exit(0);    // Exit window
        break;
    }

   
   


    glutPostRedisplay();    
}

/* Callback handler for special-key event */
void specialKeyListener(int key, int x,int y)
{
	switch(key){
		case GLUT_KEY_UP:		//down arrow key
			px += lx;
            py += ly;
            pz += lz;
			break;
		case GLUT_KEY_DOWN:		// up arrow key
			px -= lx;
            py -= ly;
            pz -= lz;
			break;

		case GLUT_KEY_RIGHT:
			// pos=pos+r;
            px += rx;
            py += ry;
            pz += rz;
			break;
		case GLUT_KEY_LEFT :
			// pos=pos-r;
            px -= rx;
            py -= ry;
            pz -= rz;
			break;

		case GLUT_KEY_PAGE_UP:
		    // pos=pos+u;
            px += ux;
            py += uy;
            pz += uz;
			break;

		case GLUT_KEY_PAGE_DOWN:
            // pos=pos-u;
            px -= ux;
            py -= uy;
            pz -= uz;
			break;

		default:
			break;
	}
	glutPostRedisplay();
}

int main(int argc, char** argv) {


    glutInit(&argc, argv);                      
    glutInitWindowSize(600,600);               
    glutInitWindowPosition(50, 50);             
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGB);	
    glutCreateWindow("Magic Cube");             
    glutDisplayFunc(display);                   
    glutReshapeFunc(reshape);                   
    glutKeyboardFunc(keyboardListener);         
    glutSpecialFunc(specialKeyListener);     
    initGL();                                 
    glutMainLoop();                           
    return 0;
}