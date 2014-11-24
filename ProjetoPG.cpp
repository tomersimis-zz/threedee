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
#include "Object.h"

#define _CRT_SECURE_NO_WARNINGS

#define DEBUG

#define EPS 1e-9

std::vector<Object> objects;

Point* lookAt;
Point* camera;
Vector forward;
Vector side;

bool drag;
Point* mousePosition;
Point* mouseInitialPosition;

GLfloat light_position[2][4] = { { 0, 0, 0, 0.0 }, { -1.0, -1.0, -1.0, 0.0 } };
int light = 0;
bool enable = false;

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

	glColor3d(0.0, 0.0, 0.0);

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

	//glTranslated(translateX, translateY, translateZ);

	/* Rotate Z 
	double rotateZMatrix[16] = {
		cos(rotationZ), sin(rotationZ), 0, 0,
	    -sin(rotationZ), cos(rotationZ),0, 0,
		0, 0, 1, 0,
		0, 0, 0, 1
	};
	glMultMatrixd(rotateZMatrix);*/
	
	/* Rotate Y 
	double rotateYMatrix[16] = {
		cos(rotationY), 0, sin(rotationY), 0,
		0, 1, 0, 0,
		-sin(rotationY), 0, cos(rotationY), 0,
		0, 0, 0, 1
	};
	glMultMatrixd(rotateYMatrix);
	*/
	/* Rotate X
	double rotateXMatrix[16] = {
		1, 0, 0, 0,
		0, cos(rotationX), sin(rotationX), 0,
		0, -sin(rotationX), cos(rotationX), 0,
		0, 0, 0, 1
	};
	glMultMatrixd(rotateXMatrix);
	*/
	//glTranslated(-translateX, -translateY, -translateZ);

	/* Translation
	double translationMatrix[16] = {
		1, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, 1, 0,
		translateX, translateY, translateZ, 1
	};
	glMultMatrixd(translationMatrix);
	 */
	/* Scale 
	double scaleMatrix[16] = {
		scale, 0, 0, 0,
		0, scale, 0, 0,
		0, 0, scale, 0,
		0, 0, 0, 1
	};
	glMultMatrixd(scaleMatrix);
	*/

	glBegin(GL_TRIANGLES);

	for (int i = 0; i < objects.size(); i++){
		for (int j = 0; j < objects[i].faces.size(); j++){
			glNormal3d(objects[i].faces[j]->fn->x, objects[i].faces[j]->fn->y, objects[i].faces[j]->fn->z);
			glVertex3d(objects[i].faces[j]->v1->x, objects[i].faces[j]->v1->y, objects[i].faces[j]->v1->z);
			glVertex3d(objects[i].faces[j]->v2->x, objects[i].faces[j]->v2->y, objects[i].faces[j]->v2->z);
			glVertex3d(objects[i].faces[j]->v3->x, objects[i].faces[j]->v3->y, objects[i].faces[j]->v3->z);
		}
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

	const GLfloat light0_color[4] = { .3, .3, .3, 0.0 };
	GLfloat light0_shininess[] = { 40.0 };

	GLfloat light1_ambient[] = { 0.2, 0.2, 0.2, 1.0 };
	GLfloat light1_diffuse[] = { 1.0, 1.0, 1.0, 1.0 };
	const GLfloat light1_color[4] = { .3, .3, .3, 0.0 };
	GLfloat light1_shininess[] = { 40.0 };

	glEnable(GL_LIGHT0);
	glLightfv(GL_LIGHT0, GL_POSITION, light_position[0]);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, light0_color);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, light0_shininess);

	glEnable(GL_LIGHT1);
	glLightfv(GL_LIGHT1, GL_POSITION, light_position[1]);
	glLightfv(GL_LIGHT1, GL_AMBIENT, light1_ambient);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, light1_diffuse);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, light1_color);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, light1_shininess);
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

	 std::vector<Point*> vertex;
	 std::vector<Face*> faces;
	 std::vector<Vector*> normals;

	std::ifstream file;
	file.open(path);
	std::string line;

	while (std::getline(file, line)){

		char c[2];

		double x, y, z;

		file >> c;

		if (c[0] != '#'){
			file >> x >> y >> z;

			if (c[0] == 'v' && c[1] == 'n'){
				normals.push_back(new Vector(x, y, z));
			}else if(c[0] == 'v'){
				//factor = std::max(factor, std::max(x, std::max(y, z)));
				vertex.push_back(new Point(x,y,z));
			}else if(c[0] == 'f'){
				faces.push_back(new Face(vertex[x - 1], vertex[y - 1], vertex[z - 1], new Vector(0,0,0)));
			}else if (c[0] == 's'){

			}
		}

	}

	Object newObject = Object(vertex, faces, normals);
	newObject.calculateNormals();
	objects.push_back(newObject);

	file.close();

	#ifdef DEBUG
		printf("[DEBUG] Number of points loaded: %d\n", vertex.size());
		printf("[DEBUG] Number of faces loaded: %d\n", faces.size());
	#endif


}

void keyboard(unsigned char key, int x, int y){

	switch (key){

		case '1':
			//translateX -= 0.1;
			break;
		case '2':
			//translateX += 0.1;
			break;
		case '3':
			//translateY -= 0.1;
			break;
		case '4':
			//translateY += 0.1;
			break;
		case '5':
			//translateZ -= 0.1;
			break;
		case '6':
			//translateZ += 0.1;
			break;
		case '7':
			//rotationX += 0.1;
			break;
		case '8':
			//rotationY += 0.1;
			break;
		case '9':
			//rotationZ += 0.1;
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
			//scale-= 0.01;
			break;
		case '=':
		case '+':
			//scale += 0.01;
			break;
		case 'u':
		case 'U':
			printf("%d",light);
			light_position[light][0] += 0.05;
			if (light_position[light][0] > 1) light_position[light][0] = 1;
			if (light == 0){
				glLightfv(GL_LIGHT0, GL_POSITION, light_position[light]);
			}
			else{
				glLightfv(GL_LIGHT1, GL_POSITION, light_position[light]);
			}			
			break;
		case 'j':
		case 'J':
			light_position[light][0] -= 0.05;
			if (light_position[light][0] < -1) light_position[light][0] = -1;
			if (light == 0){
				glLightfv(GL_LIGHT0, GL_POSITION, light_position[light]);
			}
			else{
				glLightfv(GL_LIGHT1, GL_POSITION, light_position[light]);
			}
			break;
		case 'i':
		case 'I':
			light_position[light][1] += 0.05;
			if (light_position[light][1] > 1) light_position[light][1] = 1;
			if (light == 0){
				glLightfv(GL_LIGHT0, GL_POSITION, light_position[light]);
			}
			else{
				glLightfv(GL_LIGHT1, GL_POSITION, light_position[light]);
			}
			break;
		case 'k':
		case 'K':
			light_position[light][1] -= 0.05;
			if (light_position[light][1] < -1) light_position[light][1] = -1;
			if (light == 0){
				glLightfv(GL_LIGHT0, GL_POSITION, light_position[light]);
			}
			else{
				glLightfv(GL_LIGHT1, GL_POSITION, light_position[light]);
			}
			break;
		case 'o':
		case 'O':
			light_position[light][2] += 0.05;
			if (light_position[light][2] > 1) light_position[light][2] = 1;
			if (light == 0){
				glLightfv(GL_LIGHT0, GL_POSITION, light_position[light]);
			}
			else{
				glLightfv(GL_LIGHT1, GL_POSITION, light_position[light]);
			}
			break;
		case 'l':
		case 'L':
			light_position[light][2] -= 0.05;
			if (light_position[light][2] < -1) light_position[light][2] = -1;
			if (light == 0){
				glLightfv(GL_LIGHT0, GL_POSITION, light_position[light]);
			}
			else{
				glLightfv(GL_LIGHT1, GL_POSITION, light_position[light]);
			}
			break;
		case 'n':
		case 'N':
			light = (light + 1) % 2;
			break;
		case 'M':
		case 'm':
			if (enable){
				enable = false;
				glDisable(GL_LIGHTING);
			}else{
				enable = true;
				glEnable(GL_LIGHTING);
			}
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

