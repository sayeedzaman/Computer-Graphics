#include<bits/stdc++.h>
#include <GL/glut.h>
using namespace std;

#include "1905116.h"
#include "bitmap_image.hpp"


double cameraHeight;
double cameraAngle;
int drawgrid;
int drawaxes;
double angle;

int recursionLevel;
int imageHeight,imageWidth;
bitmap_image image;

vector <Object*> objects;
vector <Light*> lights;
vector <SpotLight*> sls;

int imageCount = 1;

// position of camera
Point pos(-39.7351, -101.797, 73);

// up, right and look direction
Point up(0,0,1);
Point rightV(-1 / sqrt(2), 1 / sqrt(2), 0);
Point look(0.325568, 0.945519, 0);

double rotationAngle = 2* M_PI/180;
int numSegments;

// draw axes
void drawAxes()
{
	if(drawaxes==1)
	{
		glColor3f(1.0, 1.0, 1.0);
		glBegin(GL_LINES);
		{
			glVertex3f( 100,0,0);
			glVertex3f(-100,0,0);

			glVertex3f(0,-100,0);
			glVertex3f(0, 100,0);

			glVertex3f(0,0, 100);
			glVertex3f(0,0,-100);
		}
		glEnd();
	}
}

// draws grid
void drawGrid()
{
	int i;
	if(drawgrid==1)
	{
		glColor3f(0.6, 0.6, 0.6);	//grey
		glBegin(GL_LINES);
		{
			for(i=-8;i<=8;i++){

				if(i==0)
					continue;	//SKIP the MAIN axes

				//lines parallel to Y-axis
				glVertex3f(i*10, -90, 0);
				glVertex3f(i*10,  90, 0);

				//lines parallel to X-axis
				glVertex3f(-90, i*10, 0);
				glVertex3f( 90, i*10, 0);
			}
		}
		glEnd();
	}
}

void rotate(Point &vec,Point &axis,double ang)
{
	// vec = vec*cos(ang)+(vec*axis)*sin(ang);
	vec = vec*cos(ang)+(axis^vec)*sin(ang);
}

double windowWidth = 500, windowHeight = 500;
double viewAngle = 80;

void capture() {
    cout << "Capturing Image" << endl;

    // Initialize bitmap image and set background color to black
    for (int i = 0; i < imageWidth; i++) {
        for (int j = 0; j < imageHeight; j++) {
            image.set_pixel(i, j, 0, 0, 0); // Set background color to black
        }
    }

    // Calculate plane distance and top-left corner
    double planeDistance = (windowHeight / 2.0) / tan((M_PI * viewAngle) / (360.0));
    Point topLeft = pos + (look * planeDistance) + (up * (windowHeight / 2.0)) - (rightV * (windowWidth / 2.0));
    double du = windowWidth / (imageWidth * 1.0);
    double dv = windowHeight / (imageHeight * 1.0);
    topLeft = topLeft + (rightV * du / 2.0) - (up * dv / 2.0);

    // Cast rays and render image
    for (int i = 0; i < imageWidth; i++) {
        for (int j = 0; j < imageHeight; j++) {
            // Calculate current pixel
            Point pixel = topLeft + (rightV * du * i) - (up * dv * j);

            // Cast ray from EYE to pixel
            Ray ray(pos, pixel - pos);
            Color color;

            // Find nearest object
            double tMin = -1;
            int nearestObjectIndex = -1;
            for (int k = 0; k < objects.size(); k++) {
                double t = objects[k]->intersect(ray, color, 0);
                if (t > 0 && (nearestObjectIndex == -1 || t < tMin)) {
                    tMin = t;
                    nearestObjectIndex = k;
                }
            }

            // Shade the pixel if an object is intersected
            if (nearestObjectIndex != -1) {
                double t = objects[nearestObjectIndex]->intersect(ray, color, 1);

                // Ensure color components are within the valid range [0, 1]
                color.r = (color.r > 1) ? 1 : (color.r < 0 ? 0 : color.r);
                color.g = (color.g > 1) ? 1 : (color.g < 0 ? 0 : color.g);
                color.b = (color.b > 1) ? 1 : (color.b < 0 ? 0 : color.b);

                // Set pixel color in the image
                image.set_pixel(i, j, static_cast<int>(255 * color.r), static_cast<int>(255 * color.g), static_cast<int>(255 * color.b));
            }
        }
    }

    // Save the image
    image.save_image("Output_" + to_string(imageCount) + ".bmp");
    imageCount++;
    cout << "Saving Image" << endl;
}

void keyboardListener(unsigned char key, int x,int y){
	switch(key){
		case '0':
			capture();
			break;
		case '1':
			// rotate LEFT 
			rotate(rightV,up,rotationAngle);
			rotate(look,up,rotationAngle);
			break;
		case '2':
			// rotate right
			rotate(rightV,up,-rotationAngle);
			rotate(look,up,-rotationAngle);
			break;
		case '3':
			// rotate UP
			rotate(up,rightV,rotationAngle);
			rotate(look,rightV,rotationAngle);
			break;
		case '4':
			// rotate DOWN
			rotate(up,rightV,-rotationAngle);
			rotate(look,rightV,-rotationAngle);
			break;
		case '5':
			// tilt CLKWISE
			rotate(rightV,look,rotationAngle);
			rotate(up,look,rotationAngle);
			break;
		case '6':
			// tilt COUNTER CLKWISE
			rotate(rightV,look,-rotationAngle);
			rotate(up,look,-rotationAngle);
			break;
		default:
			break;
	}
}


void specialKeyListener(int key, int x,int y){
	switch(key){
		case GLUT_KEY_DOWN:		//down arrow key
			pos = pos - look * 3;
			break;
		case GLUT_KEY_UP:		// up arrow key
			pos = pos + look * 3;
			break;
		case GLUT_KEY_RIGHT:
			pos = pos + rightV * 3;
			break;
		case GLUT_KEY_LEFT:
			pos = pos - rightV * 3;
			break;
		case GLUT_KEY_PAGE_UP:
			pos = pos + up * 3;
			break;
		case GLUT_KEY_PAGE_DOWN:
			pos = pos - up * 3;
			break;
		case GLUT_KEY_INSERT:
			break;
		case GLUT_KEY_HOME:
			break;
		case GLUT_KEY_END:
			break;
		default:
			break;
	}
}


void mouseListener(int button, int state, int x, int y){	//x, y is the x-y of the screen (2D)
	switch(button){
		case GLUT_LEFT_BUTTON:
			if(state == GLUT_DOWN){		// 2 times?? in ONE click? -- solution is checking DOWN or UP
				drawaxes=1-drawaxes;
			}
			break;

		case GLUT_RIGHT_BUTTON:
			//........
			break;

		case GLUT_MIDDLE_BUTTON:
			//........
			break;

		default:
			break;
	}
}



void display(){

	//clear the display
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0,0,0,0);	//color black
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	/********************
	/ set-up camera here
	********************/
	//load the correct matrix -- MODEL-VIEW matrix
	glMatrixMode(GL_MODELVIEW);

	//initialize the matrix
	glLoadIdentity();


	gluLookAt(pos.x, pos.y, pos.z, 
			pos.x + look.x, pos.y + look.y, pos.z + look.z, 
			up.x, up.y, up.z);

	glMatrixMode(GL_MODELVIEW);

	drawAxes();
	drawGrid();

    for (int i=0; i<objects.size(); i++){
		Object *object = objects[i];
		object->draw();
	}

	for (int i=0; i<lights.size(); i++){
		lights[i]->draw();
	}

	for(int i=0;i<sls.size();i++){
		SpotLight* l = sls[i];
		sls[i]->draw();
	}

	glutSwapBuffers();
}


void animate(){
	glutPostRedisplay();
}

void loadData()
{
	ifstream in("scene.txt");
	in >> recursionLevel >> imageHeight;

	imageWidth = imageHeight;

	int objCount;
	in >> objCount;

	for(int i=0;i<objCount;i++)
	{
		string objType;
		in >> objType;

		Object *obj;

		if(objType == "sphere"){
			obj = new Sphere();
			in >> *((Sphere *)obj);
		}
		else if(objType == "triangle"){
			obj = new Triangle();
			in >> *((Triangle *)obj);
		}
		else if(objType == "general"){
			// obj = new Plane();
			obj = new General();
			in >> *((General *)obj);
		}
		else{
			cout<<objType<<" is not a valid object type"<<endl;
		}
		objects.push_back(obj);
	}

	int lightCount;
	in >> lightCount;

	for(int i=0;i<lightCount;i++){
		Light *light = new Light();
		in >> *light;
		lights.push_back(light);
	}

	int spotlightCount;
	in >> spotlightCount;

	for(int i=0;i<spotlightCount;i++){
		SpotLight *spotlight = new SpotLight();
		in >> *spotlight;
		sls.push_back(spotlight);
	}


	Object *floor;
	floor = new Floor(400, 10);
	floor->setColor(Color(0.5, 0.5, 0.5));
	vector <double> coefficients = {0.4, 0.2, 0.2, 0.2};
	floor->setCoefficients(coefficients);
	objects.push_back(floor);
	
	
}

void init(){
	//codes for initialization
	drawgrid=1;
	drawaxes=1;
	cameraHeight=150.0;
	cameraAngle=1.0;
	angle=0;
	numSegments = 36;

	loadData();
	image = bitmap_image(imageWidth, imageHeight);

	//clear the screen
	glClearColor(0,0,0,0);

	glMatrixMode(GL_PROJECTION);

	glLoadIdentity();

	gluPerspective(80,	1,	1,	1000.0);

}

int main(int argc, char **argv){

	
	glutInit(&argc,argv);
	glutInitWindowSize(500, 500);
	glutInitWindowPosition(0, 0);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGB);	//Depth, Double buffer, RGB color

	glutCreateWindow("1905116");

	init();

	glEnable(GL_DEPTH_TEST);	//enable Depth Testing

	glutDisplayFunc(display);	//display callback function
	// cout<< pos.x<< " "<< pos.y<< " "<< pos.z<< " "<< " "<< up.x<< " "<< up.y<< " "<< up.z<<" "<< look.x<< " "<< look.y<< " "<< look.z <<  endl;
	glutIdleFunc(animate);		//what you want to do in the idle time (when no drawing is occuring)

	glutKeyboardFunc(keyboardListener);
	glutSpecialFunc(specialKeyListener);
	glutMouseFunc(mouseListener);

	glutMainLoop();		//The main loop of OpenGL

	objects.clear();
	objects.shrink_to_fit();

	lights.clear();
	lights.shrink_to_fit();

	sls.clear();
	sls.shrink_to_fit();

	return 0;
}