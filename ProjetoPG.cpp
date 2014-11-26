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
#include "Loader.h"
#include "Camera.h"

#define _CRT_SECURE_NO_WARNINGS

#define DRAW_NORMAL

#define DEBUG

#define EPS 1e-9


std::vector<Object> objects;

Camera camera;

vector<vector<Point>> MEM_VEC_POINTS;
vector<vector<Vector>> MEM_VEC_VECTOR;
vector<vector<Face>> MEM_VEC_FACES;

bool drag;
Point* mousePosition;
Point* mouseInitialPosition;

GLfloat light_position[2][4] = { { 0, 0, 0, 0.0 }, { -1.0, -1.0, -1.0, 0.0 } };
int light = 0;
bool enable = false;

float near = 0.1;

int selectedObject = 0;

void drawCoordinateSystem(){
	glLineWidth(1.0);
	glColor3d(0.2, 0.64, 0.76);

	glBegin(GL_LINES);

	glVertex3d(-10000, 0, 0);
	glVertex3d(10000, 0, 0);

	glVertex3d(0, -10000, 0);
	glVertex3d(0, 10000, 0);

	glVertex3d(0, 0, -10000);
	glVertex3d(0, 0, 10000);

	glEnd();
}

void translate(double x, double y, double z){
	double translationMatrix[16] = {
		1, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, 1, 0,
		x, y, z, 1
	};
	glMultMatrixd(translationMatrix);
}

void scale(double scale){
	double scaleMatrix[16] = {
		scale, 0, 0, 0,
		0, scale, 0, 0,
		0, 0, scale, 0,
		0, 0, 0, 1
	};
	glMultMatrixd(scaleMatrix);
}

void rotateZ(double angle){
	double rotateZMatrix[16] = {
		cos(angle), sin(angle), 0, 0,
		-sin(angle), cos(angle), 0, 0,
		0, 0, 1, 0,
		0, 0, 0, 1
	};
	glMultMatrixd(rotateZMatrix);
}

void rotateY(double angle){
	double rotateYMatrix[16] = {
		cos(angle), 0, sin(angle), 0,
		0, 1, 0, 0,
		-sin(angle), 0, cos(angle), 0,
		0, 0, 0, 1
	};
	glMultMatrixd(rotateYMatrix);
}

void rotateX(double angle){
	double rotateXMatrix[16] = {
		1, 0, 0, 0,
		0, cos(angle), sin(angle), 0,
		0, -sin(angle), cos(angle), 0,
		0, 0, 0, 1
	};
	glMultMatrixd(rotateXMatrix);
}

void drawColorPicker(){
	/*glBegin(GL_QUADS);
	glColor3f(1, 0, 0);
	glVertex3f(glutGet(GLUT_WINDOW_WIDTH) - 100, glutGet(GLUT_WINDOW_HEIGHT) - 100, 0); // top left
	glColor3f(0, 1, 0);
	glVertex3f(glutGet(GLUT_WINDOW_WIDTH), glutGet(GLUT_WINDOW_HEIGHT) - 100, 0.0); // top right
	glColor3f(0, 0, 1);
	glVertex3f(glutGet(GLUT_WINDOW_WIDTH) -100, glutGet(GLUT_WINDOW_HEIGHT), 0.0); // bot left
	glColor3f(1, 1, 1);
	glVertex3f(glutGet(GLUT_WINDOW_WIDTH), glutGet(GLUT_WINDOW_HEIGHT), 0.0); // bot right
	glEnd();*/

	glBegin(GL_QUADS);
	glColor3f(1, 0, 0);
	glVertex3f(glutGet(GLUT_WINDOW_WIDTH) - 100,0, 0); // top left
	glColor3f(0, 1, 0);
	glVertex3f(glutGet(GLUT_WINDOW_WIDTH) - 100, 100, 0.0); // top right
	glColor3f(0, 0, 1);
	glVertex3f(glutGet(GLUT_WINDOW_WIDTH), 0, 0.0); // bot left
	glColor3f(1, 1, 1);
	glVertex3f(glutGet(GLUT_WINDOW_WIDTH), 100, 0.0); // bot right
	glEnd();
}

void display(){
	glClearColor(0.0, 0.0, 0.0, 1.0);

	glClear(GL_COLOR_BUFFER_BIT);

	glColor3d(0.0, 0.0, 0.0);

	glPointSize(1.0);

	glMatrixMode(GL_MODELVIEW);

	double m[16] = {
		camera.getMatrix()[0], camera.getMatrix()[1], camera.getMatrix()[2], camera.getMatrix()[3],
		camera.getMatrix()[4], camera.getMatrix()[5], camera.getMatrix()[6], camera.getMatrix()[7],
		camera.getMatrix()[8], camera.getMatrix()[9], camera.getMatrix()[10], camera.getMatrix()[11],
		camera.getMatrix()[12], camera.getMatrix()[13], camera.getMatrix()[14], camera.getMatrix()[15],
	};

	glLoadMatrixd(m);

	for (int i = 0; i < objects.size(); i++){
		glPushMatrix();

		translate(objects[i].translationX, objects[i].translationY, objects[i].translationZ);


		rotateX(objects[i].rotationX);
		rotateY(objects[i].rotationY);
		rotateZ(objects[i].rotationZ);

		scale(objects[i].scale);

		glBegin(GL_TRIANGLES);

		for (int j = 0; j < objects[i].faces.size(); j++){
			//printf("\nChegou 2\n");
#ifdef DRAW_NORMAL
			glNormal3d(objects[i].faces[j]->fn->x, objects[i].faces[j]->fn->y, objects[i].faces[j]->fn->z);
#endif
			//printf("Chegou 1\n");
			glVertex3d(objects[i].faces[j]->v1->x, objects[i].faces[j]->v1->y, objects[i].faces[j]->v1->z);
			glVertex3d(objects[i].faces[j]->v2->x, objects[i].faces[j]->v2->y, objects[i].faces[j]->v2->z);
			glVertex3d(objects[i].faces[j]->v3->x, objects[i].faces[j]->v3->y, objects[i].faces[j]->v3->z);
		}
		glEnd();

		glPopMatrix();

	}



	/* Coordinate system drawing */
	glPushMatrix();

	drawCoordinateSystem();

	glPopMatrix();



	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();

	gluOrtho2D(0.0, 1600, 0.0, 800);
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();

	drawColorPicker();

	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();
	glMatrixMode(GL_PROJECTION);
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
	printf("[DEBUG] Reshaping window, new size: %d x %d\n", w, h);
#endif

	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glFrustum(-0.041421, 0.041421, -0.041421, 0.041421, near, 500);

	setLightning();
}


char mode = 0; // 0 = obj, 1 = light
void keyboard(unsigned char key, int x, int y){

	switch (key){

	case '1':
		if (mode == 0){
			objects[selectedObject].translationX -= 0.1;
		}
		else{
			light_position[light][0] += 0.05;
			if (light_position[light][0] > 1) light_position[light][0] = 1;
			if (light == 0){
				glLightfv(GL_LIGHT0, GL_POSITION, light_position[light]);
			}
			else{
				glLightfv(GL_LIGHT1, GL_POSITION, light_position[light]);
			}
		}
		break;
	case '2':
		if (mode == 0){
			objects[selectedObject].translationX += 0.1;
		}
		else{
			light_position[light][0] -= 0.05;
			if (light_position[light][0] < -1) light_position[light][0] = -1;
			if (light == 0){
				glLightfv(GL_LIGHT0, GL_POSITION, light_position[light]);
			}
			else{
				glLightfv(GL_LIGHT1, GL_POSITION, light_position[light]);
			}
		}

		break;
	case '3':
		if (mode == 0){
			objects[selectedObject].translationY -= 0.1;
		}
		else{
			light_position[light][1] += 0.05;
			if (light_position[light][1] > 1) light_position[light][1] = 1;
			if (light == 0){
				glLightfv(GL_LIGHT0, GL_POSITION, light_position[light]);
			}
			else{
				glLightfv(GL_LIGHT1, GL_POSITION, light_position[light]);
			}
		}

		break;
	case '4':
		if (mode == 0){
			objects[selectedObject].translationY += 0.1;
		}
		else{
			light_position[light][1] -= 0.05;
			if (light_position[light][1] < -1) light_position[light][1] = -1;
			if (light == 0){
				glLightfv(GL_LIGHT0, GL_POSITION, light_position[light]);
			}
			else{
				glLightfv(GL_LIGHT1, GL_POSITION, light_position[light]);
			}
		}
		break;
	case '5':
		if (mode == 0){
			objects[selectedObject].translationZ -= 0.1;
		}
		else{
			light_position[light][2] += 0.05;
			if (light_position[light][2] > 1) light_position[light][2] = 1;
			if (light == 0){
				glLightfv(GL_LIGHT0, GL_POSITION, light_position[light]);
			}
			else{
				glLightfv(GL_LIGHT1, GL_POSITION, light_position[light]);
			}
		}

		break;
	case '6':
		if (mode == 0){
			objects[selectedObject].translationY += 0.1;
		}
		else{
			light_position[light][2] -= 0.05;
			if (light_position[light][2] < -1) light_position[light][2] = -1;
			if (light == 0){
				glLightfv(GL_LIGHT0, GL_POSITION, light_position[light]);
			}
			else{
				glLightfv(GL_LIGHT1, GL_POSITION, light_position[light]);
			}
		}

		break;
	case '7':
		objects[selectedObject].rotationX += 0.1;
		break;
	case '8':
		objects[selectedObject].rotationY += 0.1;
		break;
	case '9':
		objects[selectedObject].rotationZ += 0.1;
		break;
	case 'w':
	case 'W':
		camera.slide(0, 0, -1);
		break;
	case 's':
	case 'S':
		camera.slide(0, 0, 1);
		break;
	case 'd':
	case 'D':
		camera.slide(1, 0, 0);
		break;
	case 'a':
	case 'A':
		camera.slide(-1, 0, 0);
		break;
	case '-':
	case '_':
		objects[selectedObject].scale -= 0.1;
		break;
	case '=':
	case '+':
		objects[selectedObject].scale += 0.1;
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
		}
		else{
			enable = true;
			glEnable(GL_LIGHTING);
		}
		break;
	case '.':
	case '>':
		selectedObject = (selectedObject + 1) % objects.size();
		break;
	case ',':
	case '<':
		selectedObject -= 1;
		if (selectedObject == -1) selectedObject = objects.size() - 1;
		break;
	case 'b':
	case 'B':
		mode = (mode + 1) % 2;
		break;
	case 'z':
	case 'Z':
		camera.slide(0, 0, -0.2);
		//camera->z += cameraForward.z / 3;
		//camera->x += cameraForward.x / 3;
		//camera->y += cameraForward.y / 3;

		//lookAt->z += cameraForward.z / 3;
		//lookAt->x += cameraForward.x / 3;
		//lookAt->y += cameraForward.y / 3;

		near -= 0.0026;

		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		glFrustum(-0.041421, 0.041421, -0.041421, 0.041421, near, 500);
		break;
	case 'x':
	case 'X':
		camera.slide(0, 0, 0.2);
		//camera->z -= cameraForward.z / 3;
		//camera->x -= cameraForward.x / 3;
		//camera->y -= cameraForward.y / 3;

		//lookAt->z -= cameraForward.z / 3;
		//lookAt->x -= cameraForward.x / 3;
		//lookAt->y -= cameraForward.y / 3;

		near += 0.0026;

		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		glFrustum(-0.041421, 0.041421, -0.041421, 0.041421, near, 500);
		break;
		break;

	}

}

void mainMenu(int item){

	std::vector<Point*> vertex;
	std::vector<Face*> faces;
	std::vector<Vector*> normals;
	string s = "";
	if (item == 0){
		//Loader("Obj\\cubo1.obj", vertex, normals, faces);

	}
	else if (item == 1){
		s = "Obj\\pumpkin.obj";
	}

	else if (item == 2){
		s = "Obj\\apple.obj";
	}
	else if (item == 3){
		s = "Obj\\chimp.obj";
	}
	else if (item == 4){
		s = "Obj\\cow.obj";
	}
	else if (item == 5){
		s = "Obj\\dog.obj";

	}
	else if (item == 6){
		s = "Obj\\eagle.obj";
	}
	else if (item == 7){
		s = "Obj\\elephant.obj";
	}
	else if (item == 8){
		s = "Obj\\lion.obj";
	}
	if (s.empty()) return;
	vector<Point> CONST_P;
	vector<Vector> CONST_vec;
	vector<Face> CONST_f;
	MEM_VEC_POINTS.push_back(CONST_P);
	MEM_VEC_VECTOR.push_back(CONST_vec);
	MEM_VEC_FACES.push_back(CONST_f);

	//Loader l = Loader(&s, &CONST_P, &CONST_vec, &CONST_f);
	Loader l = Loader(&s, &MEM_VEC_POINTS[MEM_VEC_POINTS.size() - 1], &MEM_VEC_VECTOR[MEM_VEC_VECTOR.size() - 1], &MEM_VEC_FACES[MEM_VEC_FACES.size() - 1]);
	l.load();
	for (int i = 0; i < MEM_VEC_POINTS[MEM_VEC_POINTS.size() - 1].size(); i++){
		vertex.push_back(&MEM_VEC_POINTS[MEM_VEC_POINTS.size() - 1][i]);
	}

	for (int i = 0; i <MEM_VEC_VECTOR[MEM_VEC_VECTOR.size() - 1].size(); i++){
		normals.push_back(&MEM_VEC_VECTOR[MEM_VEC_VECTOR.size() - 1][i]);
	}

	for (int i = 0; i < MEM_VEC_FACES[MEM_VEC_FACES.size() - 1].size(); i++){
		faces.push_back(&MEM_VEC_FACES[MEM_VEC_FACES.size() - 1][i]);
	}




	printf("Criando o novo objeto . . ./n");
	Object newObject = Object(vertex, faces, normals);
	newObject.calculateNormals();
	printf("Calculando as normais . . ./n");
	objects.push_back(newObject);
	printf("Inserindo o objeto . . ./n");
	printf(" READY! \n");
}

void mousePress(int btn, int state, int x, int y){
	if (btn == GLUT_LEFT_BUTTON){
		if (state == GLUT_UP){
			drag = false;
		}
		else{
			drag = true;
			mouseInitialPosition->x = x;
			mouseInitialPosition->y = y;
		}
	}
}

void mouseMove(int x, int y){
	camera.yawAngle += (x - mouseInitialPosition->x)*0.5;
	camera.pitchAngle += (y - mouseInitialPosition->y)*0.5;
	mouseInitialPosition->x = x;
	mouseInitialPosition->y = y;
}

int main(int argc, char** argv)
{
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);

	mainMenu(1);
	mainMenu(4);

	glutCreateWindow("Hello World");
	glutFullScreen();

	glutDisplayFunc(display);

	glutReshapeFunc(reshape);

	glutKeyboardFunc(keyboard);

	glutMouseFunc(mousePress);

	glutMotionFunc(mouseMove);

	mousePosition = new Point(0, 0, 0);
	mouseInitialPosition = new Point(0, 0, 0);

	camera = Camera(0, 0, 100);

	glutMainLoop();
	return 0;


}

