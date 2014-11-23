// ProjetoPG.cpp : Defines the entry point for the console application.
//

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <gl/glut.h>
#include <string>
#include "Point.h"
#include "Face.h"
#include "Vector.h"
#include <vector>
#include <fstream>
#include <algorithm>

#define _CRT_SECURE_NO_WARNINGS

#define DEBUG

#define EPS 1e-9

std::vector<Point*> vertex;
std::vector<Face*> faces;
std::vector<Normal*> normals;

double rotationX;
double rotationY;
double rotationZ;

double translateX = 0;
double translateY = 0;
double translateZ = 0;

double scale = 1;

double factor;

Point* lookAt;
Point* camera;
Vector forward;
Vector side;

bool drag;
Point* mousePosition;
Point* mouseInitialPosition;

GLfloat light_position[] = { -1.0, -1.0, -1.0, 0.0 };

void drawCoordinateSystem(){
	glLineWidth(1.0);
	glColor3d(0.2, 0.64, 0.76);

	glBegin(GL_LINES);

	glVertex3d(-10000,0,0);
	glVertex3d(10000, 0, 0);

	glVertex3d(0, -10000, 0);
	glVertex3d(0, 10000, 0);

	glVertex3d(0, 0, -10000);
	glVertex3d(0, 0, 10000);

	glEnd();
}

void display(){
	glClearColor(0.0, 0.0, 0.0, 1.0);

	glClear(GL_COLOR_BUFFER_BIT);

	glColor3d(1.0, 1.0, 1.0);

	glPointSize(1.0);

	glMatrixMode(GL_MODELVIEW);

	/* View Matrix */
	forward = Vector(camera,lookAt);
	forward.normalize();

	side = Vector::cross(forward, Vector(0, 1, 0));
	
	Vector up = Vector::cross(forward, side);
	side.normalize();
	up.normalize();

		
	double viewMatrix[16] = {
		side.x, up.x, -forward.x, 0,
		side.y, up.y, -forward.y, 0,
		side.z, up.z, -forward.z, 0,
		0,0,0 , 1
	};

	

	glPushMatrix();
	glLoadIdentity();
	
	glMultMatrixd(viewMatrix);

	glTranslated(-camera->x, -camera->y, -camera->z);

	glTranslated(translateX, translateY, translateZ);

	/* Rotate Z */
	double rotateZMatrix[16] = {
		cos(rotationZ), sin(rotationZ), 0, 0,
	    -sin(rotationZ), cos(rotationZ),0, 0,
		0, 0, 1, 0,
		0, 0, 0, 1
	};
	glMultMatrixd(rotateZMatrix);
	
	/* Rotate Y */
	double rotateYMatrix[16] = {
		cos(rotationY), 0, sin(rotationY), 0,
		0, 1, 0, 0,
		-sin(rotationY), 0, cos(rotationY), 0,
		0, 0, 0, 1
	};
	glMultMatrixd(rotateYMatrix);

	/* Rotate X */
	double rotateXMatrix[16] = {
		1, 0, 0, 0,
		0, cos(rotationX), sin(rotationX), 0,
		0, -sin(rotationX), cos(rotationX), 0,
		0, 0, 0, 1
	};
	glMultMatrixd(rotateXMatrix);

	glTranslated(-translateX, -translateY, -translateZ);

	/* Translation */
	double translationMatrix[16] = {
		1, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, 1, 0,
		translateX, translateY, translateZ, 1
	};
	glMultMatrixd(translationMatrix);
	
	/* Scale */
	double scaleMatrix[16] = {
		scale, 0, 0, 0,
		0, scale, 0, 0,
		0, 0, scale, 0,
		0, 0, 0, 1
	};
	glMultMatrixd(scaleMatrix);

	glBegin(GL_TRIANGLES);

	for(int i=0; i < faces.size(); i++){
		glNormal3d(faces[i]->fn->x, faces[i]->fn->y, faces[i]->fn->z);
		glVertex3d(faces[i]->v1->x, faces[i]->v1->y, faces[i]->v1->z);
		glVertex3d(faces[i]->v2->x, faces[i]->v2->y, faces[i]->v2->z);
		glVertex3d(faces[i]->v3->x, faces[i]->v3->y, faces[i]->v3->z);

	}

	//for(int i = 0; i < vertex.size(); i++){

	//	glVertex3d(vertex[i]->x, vertex[i]->y, vertex[i]->z);

	//}

	glEnd();

	glPopMatrix();

	/* Coordinate system drawing */
	glPushMatrix();

	glLoadIdentity();

	glMultMatrixd(viewMatrix);

	drawCoordinateSystem();

	glPopMatrix();


	glutSwapBuffers();

	glutPostRedisplay();

}

void setLightning(){
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);
	const GLfloat scolor[4] = { .3, .3, .3, 0.0 };
	glMaterialfv( GL_FRONT_AND_BACK, GL_SPECULAR, scolor );
	GLfloat mat_shininess[] = { 40.0 };
	glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, mat_shininess);
}

void reshape(int w, int h){

	#ifdef DEBUG
		printf("[DEBUG] Reshaping window, new size: %d x %d\n", w,h);
	#endif
		
	glViewport(0,0,w,h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45, 1, 0.1, 500);
	setLightning();
}

 void loadObj(std::string path){

	std::ifstream file;
	file.open(path);

	std::string line;

	while (std::getline(file, line)){

		printf("%s\n", line.c_str());

		char c[2];

		double x, y, z;

		file >> c;

		if (c[0] != '#'){
			file >> x >> y >> z;

			if (c[0] == 'v' && c[1] == 'n'){
				normals.push_back(new Normal(x, y, z));
			}else if(c[0] == 'v'){
				factor = std::max(factor, std::max(x, std::max(y, z)));
				vertex.push_back(new Point(x,y,z));
			}else if(c[0] == 'f'){
				faces.push_back(new Face(vertex[x - 1], vertex[y - 1], vertex[z - 1], new Normal(0,0,0)));
			}else if (c[0] == 's'){

			}
		}

	}

	file.close();

	#ifdef DEBUG
		printf("[DEBUG] Number of points loaded: %d\n", vertex.size());
		printf("[DEBUG] Number of faces loaded: %d\n", faces.size());
	#endif


}

 void faceNormal(Point *p1, Point *p2, Point *p3, Normal *normal){

	 Point *v1 = new Point(0,0,0), *v2 = new Point(0,0,0);
	 double len;

	 v1->x = p2->x - p1->x;
	 v1->y = p2->y - p1->y;
	 v1->z = p2->z - p1->z;

	 v2->x = p3->x - p1->x;
	 v2->y = p3->y - p1->y;
	 v2->z = p3->z - p1->z;

	 /*cross product between v1 and v2*/
	 normal->x = (v1->y * v2->z) - (v1->z * v2->y);
	 normal->y = (v1->z * v2->x) - (v1->x * v2->z);
	 normal->z = (v1->x * v2->y) - (v1->y * v2->x);

	 len = sqrt(normal->x*normal->x + normal->y*normal->y + normal->z*normal->z);
	 normal->x /= len;
	 normal->y /= len;
	 normal->z /= len;
 }

 void facesNormals(){
	int size = faces.size();
	for (int i = 0; i < size; i++){
		faceNormal(faces[i]->v1, faces[i]->v2, faces[i]->v3, faces[i]->fn);
	 }
 }

void keyboard(unsigned char key, int x, int y){

	switch (key){

		case '1':
			translateX -= 0.1;
			break;
		case '2':
			translateX += 0.1;
			break;
		case '3':
			translateY -= 0.1;
			break;
		case '4':
			translateY += 0.1;
			break;
		case '5':
			translateZ -= 0.1;
			break;
		case '6':
			translateZ += 0.1;
			break;
		case '7':
			rotationX += 0.1;
			break;
		case '8':
			rotationY += 0.1;
			break;
		case '9':
			rotationZ += 0.1;
			break;
		case 'w':
		case 'W':
			camera->z += forward.z / 10;
			camera->x += forward.x / 10;
			camera->y += forward.y / 10;

			lookAt->z += forward.z / 10;
			lookAt->x += forward.x / 10;
			lookAt->y += forward.y / 10;
			break;
		case 's':
		case 'S':
			camera->z -= forward.z / 10;
			camera->x -= forward.x / 10;
			camera->y -= forward.y / 10;

			lookAt->z -= forward.z / 10;
			lookAt->x -= forward.x / 10;
			lookAt->y -= forward.y / 10;
			break;
		case 'd':
		case 'D':
			camera->x += side.x/10;
			camera->y += side.y/10;
			camera->z += side.z/10;
			break;
		case 'a':
		case 'A':
			camera->x -= side.x / 10;
			camera->y -= side.y / 10;
			camera->z -= side.z / 10;
			break;
		case '-':
		case '_':
			scale-= 0.01;
			break;
		case '=':
		case '+':
			scale += 0.01;
			break;
		case 'u':
		case 'U':
			light_position[0] += 0.05;
			if (light_position[0] > 1) light_position[0] = 1;
			printf("%f\n", light_position[0]);
			glLightfv(GL_LIGHT0, GL_POSITION, light_position);
			break;
		case 'j':
		case 'J':
			light_position[0] -= 0.05;
			if (light_position[0] < -1) light_position[0] = -1;
			printf("%f\n", light_position[0]);
			glLightfv(GL_LIGHT0, GL_POSITION, light_position);
			break;
		case 'i':
		case 'I':
			light_position[1] += 0.05;
			if (light_position[1] > 1) light_position[1] = 1;
			printf("%f\n", light_position[1]);
			glLightfv(GL_LIGHT0, GL_POSITION, light_position);
			break;
		case 'k':
		case 'K':
			light_position[1] -= 0.05;
			if (light_position[1] < -1) light_position[1] = -1;
			printf("%f\n", light_position[1]);
			glLightfv(GL_LIGHT0, GL_POSITION, light_position);
			break;
		case 'o':
		case 'O':
			light_position[2] += 0.05;
			if (light_position[2] > 1) light_position[2] = 1;
			printf("%f\n", light_position[2]);
			glLightfv(GL_LIGHT0, GL_POSITION, light_position);
			break;
		case 'l':
		case 'L':
			light_position[2] -= 0.05;
			if (light_position[2] < -1) light_position[2] = -1;
			printf("%f\n", light_position[2]);
			glLightfv(GL_LIGHT0, GL_POSITION, light_position);
			break;

	}

}

void mainMenu(int item){
	if (item == 0){
		loadObj("Obj\\cube.obj");
	}
	else if (item == 1){
		loadObj("Obj\\pumpkin.obj");
	}

	facesNormals();
}

void mousePress(int btn, int state, int x, int y){
	if (btn == GLUT_LEFT_BUTTON){
		if (state == GLUT_UP){
			drag = false;
		}else{
			drag = true;
			mouseInitialPosition->x = x;
			mouseInitialPosition->y = y;
	    }
	}
}

void mouseMove(int x, int y){
	lookAt->x += (x - mouseInitialPosition->x)*0.01;
	lookAt->y += (y - mouseInitialPosition->y)*0.01;
	mouseInitialPosition->x = x;
	mouseInitialPosition->y = y;
}

int main(int argc, char** argv)
{

	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(800, 600);
	glutCreateWindow("Hello World");

	glutCreateMenu( mainMenu );

	glutAddMenuEntry("Cube", 0);
	glutAddMenuEntry("Pumpkin", 1);

	glutAttachMenu(GLUT_RIGHT_BUTTON);

	glutDisplayFunc(display);

	glutReshapeFunc(reshape);

	glutKeyboardFunc(keyboard);

	glutMouseFunc(mousePress);

	glutMotionFunc(mouseMove);

	mousePosition = new Point(0, 0, 0);
	mouseInitialPosition = new Point(0, 0, 0);
	lookAt = new Point(0, 0, 0);
	camera = new Point(0, 0, 10);

	glutMainLoop();
	return 0;


}

