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
#include "Transform.h"

#define _CRT_SECURE_NO_WARNINGS

#define DRAW_NORMAL

#define DEBUG

#define EPS 1e-9


std::vector<Object> objects;

Camera camera;
Camera camera2;

vector<vector<Point>> MEM_VEC_POINTS;
vector<vector<Vector>> MEM_VEC_VECTOR;
vector<vector<Face>> MEM_VEC_FACES;

bool director = false;

bool directorClicked = false;

bool colorPicker = false;

bool picking = false;

bool drag;
Point* mousePosition;
Point* mouseInitialPosition;

GLfloat light_position[2][4] = { { 1, 1, 0, 0.0 }, { 1.0, 1.0, .5, 0.0 } };
int light = 0;
bool enable = false;

float near = 0.1;

int selectedObject = 0;

int windowWidth;
int windowHeight;

void setLightning();
void drawCoordinateSystem();
void drawSquare(); 
void drawColorPicker();
void drawObjects();
void drawCamera();
void display();
void reshape();
void mainMenu();
int rayIntersectsTriangle();
pair<Point, Point> fromScreenToWorld();
bool pointIn();
void paintFace();
void mousePress();
void mouseMove();


void drawCoordinateSystem(){

	glDisable(GL_LIGHTING);

	glPushMatrix();

	glLineWidth(2.0);
	glColor3d(0.9, 0.9, 0.9);

	glBegin(GL_LINES);

	glVertex3d(-100, 0, 0);
	glVertex3d(100, 0, 0);

	glVertex3d(0, -100, 0);
	glVertex3d(0, 100, 0);

	glVertex3d(0, 0, -100);
	glVertex3d(0, 0, 100);

	glEnd();

	glPopMatrix();

	glEnable(GL_LIGHTING);
}

void drawSquare(int x, int y, int size, double r, double g, double b){
	glBegin(GL_TRIANGLES);
	glColor3f(r, g, b);
	glVertex3f(x - size, y - size, 0); // top left
	glVertex3f(x + size, y - size, 0); // top right
	glVertex3f(x - size, y + size, 0); // bot left

	glVertex3f(x - size, y + size, 0); // bot left
	glVertex3f(x + size, y - size, 0); // top right
	glVertex3f(x + size, y + size, 0); // bot right
	glEnd();
}

void drawColorPicker(){

	glDisable(GL_LIGHTING);

	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();

	gluOrtho2D(0.0, 1600, 0.0, 800);
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();

	drawSquare(glutGet(GLUT_WINDOW_WIDTH) - 80, 110, 10, (double)229 / 255, (double)24 / 255, 0);

	drawSquare(glutGet(GLUT_WINDOW_WIDTH) - 50, 110, 10, (double)127 / 255, (double)13 / 255, 0);

	drawSquare(glutGet(GLUT_WINDOW_WIDTH) - 20, 110, 10, (double)64 / 255, (double)7 / 255, 0);


	drawSquare(glutGet(GLUT_WINDOW_WIDTH) - 80, 80, 10, (double)14 / 255, (double)229 / 255, 0);

	drawSquare(glutGet(GLUT_WINDOW_WIDTH) - 50, 80, 10, (double)8 / 255, (double)127 / 255, 0);

	drawSquare(glutGet(GLUT_WINDOW_WIDTH) - 20, 80, 10, (double)4 / 255, (double)64 / 255, 0);


	drawSquare(glutGet(GLUT_WINDOW_WIDTH) - 80, 50, 10, 0, (double)9 / 255, (double)229 / 255);

	drawSquare(glutGet(GLUT_WINDOW_WIDTH) - 50, 50, 10, 0, (double)5 / 255, (double)227 / 255);

	drawSquare(glutGet(GLUT_WINDOW_WIDTH) - 20, 50, 10, 0, (double)2 / 255, (double)64 / 255);


	drawSquare(glutGet(GLUT_WINDOW_WIDTH) - 80, 20, 10, 0, 0, 0);

	drawSquare(glutGet(GLUT_WINDOW_WIDTH) - 50, 20, 10, 0.5, 0.5, 0.5);

	drawSquare(glutGet(GLUT_WINDOW_WIDTH) - 20, 30, 10, 1, 1, 1);

	glBegin(GL_TRIANGLES);
	glColor3f(1, 1, 1);
	glVertex3f(glutGet(GLUT_WINDOW_WIDTH) - 100, 130, 0); // top left
	glVertex3f(glutGet(GLUT_WINDOW_WIDTH) - 100, 0, 0); // bot left
	glVertex3f(glutGet(GLUT_WINDOW_WIDTH), 130, 0); // top right

	glVertex3f(glutGet(GLUT_WINDOW_WIDTH), 130, 0); // top right
	glVertex3f(glutGet(GLUT_WINDOW_WIDTH), 0, 0); // bot right
	glVertex3f(glutGet(GLUT_WINDOW_WIDTH) - 100, 0, 0); // bot left

	glEnd();

	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();

	glEnable(GL_LIGHTING);
}


void drawObjects(){
	for (int i = 0; i < objects.size(); i++){
		glPushMatrix();

		double translationMatrix[16];
		Transform::translate(objects[i].translationX, objects[i].translationY, objects[i].translationZ, translationMatrix);
		glMultMatrixd(translationMatrix);

		double rotationXMatrix[16];
		Transform::rotateX(objects[i].rotationX, rotationXMatrix);
		glMultMatrixd(rotationXMatrix);

		double rotationYMatrix[16];
		Transform::rotateY(objects[i].rotationY, rotationYMatrix);
		glMultMatrixd(rotationYMatrix);

		double rotationZMatrix[16];
		Transform::rotateZ(objects[i].rotationZ, rotationZMatrix);
		glMultMatrixd(rotationZMatrix);

		double scaleMatrix[16];
		Transform::scale(objects[i].scale, scaleMatrix);
		glMultMatrixd(scaleMatrix);

		double R, G, B;

		int index;
		int contador = 0;
		for (int j = 0; j < objects[i].faces.size(); j++){
			R = objects[i].faces[j]->R;
			G = objects[i].faces[j]->G;
			B = objects[i].faces[j]->B;
			glColor3f(R, G, B);
			glBegin(GL_TRIANGLES);
			
			contador = (contador + 1) % 10;
			if (contador == 8){
				//glMaterial(0,1,0);
				GLfloat light1_ambient[] = { .5, 0.0, 0.0, 1.0 };
				GLfloat light1_diffuse[] = { .5, 0.0, 0.0, 1.0 };
				glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, light1_ambient);
				glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, light1_diffuse);
			}
			else{
				GLfloat light1_ambient[] = { 0.0, 0.5, 0.0, 1.0 };
				GLfloat light1_diffuse[] = { 0.0, 0.5, 0.0, 1.0 };
				glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, light1_ambient);
				glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, light1_diffuse);
			}

			#ifdef DRAW_NORMAL
			index = objects[i].faces[j]->v1->index;
			glNormal3d(objects[i].normals[index]->x, objects[i].normals[index]->y, objects[i].normals[index]->z);
			glVertex3d(objects[i].faces[j]->v1->x, objects[i].faces[j]->v1->y, objects[i].faces[j]->v1->z);
			#endif

			index = objects[i].faces[j]->v2->index;
			glNormal3d(objects[i].normals[index]->x, objects[i].normals[index]->y, objects[i].normals[index]->z);
			glVertex3d(objects[i].faces[j]->v2->x, objects[i].faces[j]->v2->y, objects[i].faces[j]->v2->z);

			index = objects[i].faces[j]->v3->index;
			glNormal3d(objects[i].normals[index]->x, objects[i].normals[index]->y, objects[i].normals[index]->z);
			glVertex3d(objects[i].faces[j]->v3->x, objects[i].faces[j]->v3->y, objects[i].faces[j]->v3->z);
		
			glEnd();
		}
		

		glPopMatrix();

	}
}

void drawGrid(){
	glDisable(GL_LIGHTING);

	glPushMatrix();

	glColor3f(.2, .2, .2);

	glLineWidth(1.0);


	glBegin(GL_LINES);
	for (int i = -50; i <= 50; i++) {

		glVertex3f(-100, 0, i * 2);
		glVertex3f(100, 0, i * 2);
		glVertex3f(i * 2, 0, -100);
		glVertex3f(i * 2, 0, 100);

	};
	glEnd();


	glPopMatrix();
	glEnable(GL_LIGHTING);

}

void drawCamera(float x, float y, float z)
{

	glColor3d(1.0, 1.0, 0.0);

	glPushMatrix();

	glTranslatef(x, y, z);
	glScaled(-1, -1, -1);
	glRotated(camera.yawAngle, 0, 1, 0);
	glRotated(-camera.pitchAngle, 1, 0, 0);

	GLUquadricObj *quadratic = gluNewQuadric();
	gluCylinder(quadratic, 1, 1, 3, 32, 32);

	glPopMatrix();

	glPushMatrix();
	glTranslatef(x, y, z);
	glRotated(camera.yawAngle, 0, 1, 0);
	glRotated(-camera.pitchAngle, 1, 0, 0);

	glScaled(3, 3, 3);

	glutSolidCube(1);

	glPopMatrix();
}

void display(){

	float aspect;
	if (director){
		aspect = (float)(windowWidth / 2) / (float)(windowHeight);
		glViewport(0, 0, glutGet(GLUT_WINDOW_WIDTH) / 2, glutGet(GLUT_WINDOW_HEIGHT));
	}else{
		aspect = (float)windowWidth / (float)(windowHeight);
		glViewport(0, 0, glutGet(GLUT_WINDOW_WIDTH), glutGet(GLUT_WINDOW_HEIGHT));
	}

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glFrustum(-0.041421*aspect, 0.041421*aspect, -0.041421, 0.041421, near, 500);
	
	glClearColor(0.0, 0.0, 0.0, 1.0);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

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

	drawObjects();

	setLightning();
	/* Coordinate system drawing */

	drawCoordinateSystem();

	drawGrid();

	if (colorPicker)
		drawColorPicker();

	if (director){
		glViewport(glutGet(GLUT_WINDOW_WIDTH) / 2, 0, glutGet(GLUT_WINDOW_WIDTH) / 2, glutGet(GLUT_WINDOW_HEIGHT));

		glColor3d(0.0, 0.0, 0.0);

		glPointSize(1.0);

		glMatrixMode(GL_MODELVIEW);

		double m2[16] = {
			camera2.getMatrix()[0], camera2.getMatrix()[1], camera2.getMatrix()[2], camera2.getMatrix()[3],
			camera2.getMatrix()[4], camera2.getMatrix()[5], camera2.getMatrix()[6], camera2.getMatrix()[7],
			camera2.getMatrix()[8], camera2.getMatrix()[9], camera2.getMatrix()[10], camera2.getMatrix()[11],
			camera2.getMatrix()[12], camera2.getMatrix()[13], camera2.getMatrix()[14], camera2.getMatrix()[15],
		};

		glLoadMatrixd(m2);

		drawObjects();

		drawGrid();

		glColor3d(1.0, 0.0, 0.0);
		drawCamera(camera.position.x, camera.position.y, camera.position.z);

	}
	glutSwapBuffers();

	glutPostRedisplay();

}

void setLightning(){
	glEnable(GL_LIGHTING);
	glClearDepth(1.0f);
	glDepthFunc(GL_LESS);
	glEnable(GL_DEPTH_TEST);

	GLfloat light1_ambient[] = { .5, 0.0, 0.0, 1.0 };
	GLfloat light1_diffuse[] = { .5, 0.0, 0.0, 1.0 };

	glEnable(GL_LIGHT0);
	glLightfv(GL_LIGHT0, GL_POSITION, light_position[0]);

	glEnable(GL_LIGHT1);
	glLightfv(GL_LIGHT1, GL_POSITION, light_position[1]);
	glLightfv(GL_LIGHT1, GL_AMBIENT, light1_ambient);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, light1_diffuse);
}

void reshape(int w, int h){

	windowWidth = w;
	windowHeight = h;

	#ifdef DEBUG
		printf("[DEBUG] Reshaping window, new size: %d x %d\n", w, h);
	#endif

	float aspect;
	if (director) {
		aspect = (float)(w/2) / (float)(h/2);
	}else{
		aspect = (float)w / (float)h ;
	}
	
	
	glViewport(w/2, 0, w/2, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glFrustum(-0.041421*aspect, 0.041421*aspect, -0.041421, 0.041421, near, 500);

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
			objects[selectedObject].translationZ += 0.1;
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
		if (director && x > windowWidth / 2){
			camera2.slide(0, 0, -1);
		}
		else{
			camera.slide(0, 0, -1);
		}
		break;
	case 's':
	case 'S':
		if (director && x > windowWidth / 2){
			camera2.slide(0, 0, 1);
		}
		else{
			camera.slide(0, 0, 1);
		}
		break;
	case 'd':
	case 'D':
		if (director && x > windowWidth / 2){
			camera2.slide(1, 0, 0);
		}
		else{
			camera.slide(1, 0, 0);
		}
		break;
	case 'a':
	case 'A':
		if (director && x > windowWidth / 2){
			camera2.slide(-1, 0, 0);
		}
		else{
			camera.slide(-1, 0, 0);
		}
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
	case 'C':
	case 'c':
		colorPicker = !colorPicker;
		break;
	case 'V':
	case 'v':
		director = !director;
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
	newObject.calculateVertexNormals();
	printf("Calculando as normais . . ./n");
	objects.push_back(newObject);
	printf("Inserindo o objeto . . ./n");
	printf(" READY! \n");
}


int rayIntersectsTriangle(Point p, Vector d,
	Point v0, Point v1, Point v2) {
	float e1[3], e2[3], h[3], s[3], q[3];
	Vector E1 = Vector(v1, v0);
	Vector E2 = Vector(v2, v0);
	Vector H = E2.cross(d, E2);
	double A = E1.dotProduct(H);
	if (A > -0.00001 && A < 0.00001) return 0;
	double F = 1.0 / A;
	Vector S = Vector(p, v0);
	double U = F * (S.dotProduct(H));
	if (U < 0.0 || U > 1.0) return 0;
	Vector Q = S.cross(S, E1);
	double V = F * d.dotProduct(Q);
	if (V < 0.0 || U + V > 1.0) return 0;
	// achando t 
	double T = F * E2.dotProduct(Q);
	if (T > 0.00001) return 1; // intereseção de raio
	else return 0; // intereseção de linha mas não de raio
}

pair<Point, Point> fromScreenToWorld(int winX, int winY){
	double winZ;
	GLdouble modelview[16];
	GLdouble projection[16];
	GLint viewport[4];
	glGetDoublev(GL_MODELVIEW_MATRIX, modelview);
	glGetDoublev(GL_PROJECTION_MATRIX, projection);
	glGetIntegerv(GL_VIEWPORT, viewport);
	double resX, resY, resZ, res2X, res2Y, res2Z;
	winY = viewport[3] - winY;
	winZ = near;
	gluUnProject(winX, winY, winZ, modelview, projection, viewport, &resX, &resY, &resZ);
	printf("Resultado: %lf %lf %lf\n", resX, resY, resZ);
	winZ = 500;
	gluUnProject(winX, winY, winZ, modelview, projection, viewport, &res2X, &res2Y, &res2Z);
	printf("Resultado: %lf %lf %lf\n", res2X, res2Y, res2Z);
	return (make_pair(Point(resX, resY, resZ), Point(res2X, res2Y, res2Z)));
}

bool pointIn(pair<Point, Point> par, Face f){
	Point pa = par.first, pb = par.second;
	Vector dir = Vector(pa, pb);
	Point v0 = *(f.v1);
	Point v1 = *(f.v2);
	Point v2 = *(f.v3);
	return rayIntersectsTriangle(pa, dir, v0, v1, v2);
}


void paintFace(pair<Point, Point> par){
	printf("Camera---> %lf %lf %lf\n", camera.position.x, camera.position.y, camera.position.z);
	Face f, fobj;
	double R = 0.5, G = 0.5, B = 0.5;
	bool ok;
	pair<int, int> index; // first == object, second == face;
	bool found = 0;
	Point aux1 = Point(par.first.x, par.first.y, par.first.z);
	Point aux2 = Point(par.second.x, par.second.y, par.second.z);

	double minDist = 0x3f3f3f, tx, ty, tz, dx, dy, dz, angleX, angleY, angleZ, scale;
	for (int i = 0; i < objects.size(); i++){
		par.first = Point(aux1.x, aux1.y, aux1.z);
		par.second = Point(aux2.x, aux2.y, aux2.z);

		dx = objects[i].translationX;
		dy = objects[i].translationY;
		dz = objects[i].translationZ;
		angleX = objects[i].rotationX;
		angleY = objects[i].rotationY;
		angleZ = (objects[i].rotationZ);
		scale = objects[i].scale;

		angleX *= -1;
		angleY *= 1;
		angleZ *= -1;

		// translate
		par.first.x -= dx;
		par.first.y -= dy;
		par.first.z -= dz;

		par.second.x -= dx;
		par.second.y -= dy;
		par.second.z -= dz;

		//scale
		par.first = par.first.multiplyScalar(1.0 / scale);
		par.second = par.second.multiplyScalar(1.0 / scale);

		par.first = par.first.rotate(angleX, 0);
		par.first = par.first.rotate(angleY, 1);
		par.first = par.first.rotate(angleZ, 2);

		par.second = par.second.rotate(angleX, 0);
		par.second = par.second.rotate(angleY, 1);
		par.second = par.second.rotate(angleZ, 2);



		//printf("Dx:%lf | DY: %lf  | DZ: %lf | scale: %lf | angle: %lf %lf %lf\n", dx, dy, dz, scale, angleX, angleY, angleZ);

		for (int j = 0; j < objects[i].faces.size(); j++){
			fobj = *(objects[i].faces[j]);
			Point pa, pb, pc;


			pa = *(fobj.v1);
			pb = *(fobj.v2);
			pc = *(fobj.v3);

			pa = Point(pa.x, pa.y, pa.z);
			pb = Point(pb.x, pb.y, pb.z);
			pc = Point(pc.x, pc.y, pc.z);

			/*
			// Rotate
			pa = pa.rotate(angleX, 0);
			pa = pa.rotate( angleY, 1);
			pa = pa.rotate( angleZ, 2);

			pb = pb.rotate( angleX, 0);
			pb = pb.rotate(angleY, 1);
			pb = pb.rotate(angleZ, 2);

			pc = pc.rotate( angleX, 0);
			pc = pc.rotate( angleY, 1);
			pc = pc.rotate( angleZ, 2);

			//Translate
			pa.x += dx;
			pb.x += dx;
			pc.x += dx;


			pa.y += dy;
			pb.y += dy;
			pc.y += dy;

			pa.z += dz;
			pb.z += dz;
			pc.z += dz;

			//Scale
			pa = pa.multiplyScalar(scale);
			pb = pb.multiplyScalar(scale);
			pc = pc.multiplyScalar(scale);

			*/
			/*
			pa = Point(fobj.v1->x + dx, fobj.v1->y + dy, fobj.v1->z + dz);
			pb = Point(fobj.v2->x + dx, fobj.v2->y + dy, fobj.v2->z + dz);
			pc = Point(fobj.v3->x + dx, fobj.v3->y + dy, fobj.v3->z + dz);
			*/
			f = Face(pa, pb, pc);

			ok = pointIn(par, f);
			if (ok){
				found = 1;

				Point centro = f.centroid();


				double distCentroid = camera.position.distanceFrom(centro);
				//double distCentroid = camera.position.distanceFrom( *(f.v1)  );
				printf("Encontrou! Centroid: %lf %lf %lf   || Distance: %lf e minDist: %lf  \n", centro.x, centro.y, centro.z, distCentroid, minDist);
				if (distCentroid < minDist) {
					minDist = distCentroid;
					index.first = i, index.second = j;
					printf("Melhor: (%d, %d) -- Dist: %lf\n", i, j, minDist);
				}
			}
		}
	}
	if (found){

		int i = index.first, j = index.second;
		printf("Face index: %d %d\n", i, j);
		objects[i].faces[j]->R = R;
		objects[i].faces[j]->G = G;
		objects[i].faces[j]->B = B;
	}


}

bool CLICKED = 0;

void mousePress(int btn, int state, int x, int y){
	if (btn == GLUT_LEFT_BUTTON){

		if (state == GLUT_UP){
			drag = false;
			picking = false;
		}
		else{

			if (!CLICKED){
				printf("Em tela: %d %d\n", x, y);
				double winX = x, winY = y, winZ;
				pair<Point, Point> par = fromScreenToWorld(x, y);
				paintFace(par);
				CLICKED = 0;
			}
			else CLICKED = 1;

			if (director && x > windowWidth / 2){
				directorClicked = true;
			}
			else{
				directorClicked = false;
			}

			drag = false;
			if (colorPicker && x > glutGet(GLUT_WINDOW_WIDTH) - 120 && y > glutGet(GLUT_WINDOW_HEIGHT) - 120){ // picking a color
				picking = true;

				GLint viewport[4];
				GLubyte pixel[3];

				glGetIntegerv(GL_VIEWPORT, viewport);

				glReadPixels(x, viewport[3] - y, 1, 1,
					GL_RGB, GL_UNSIGNED_BYTE, (void *)pixel);

				printf(">>>>>%d %d %d\n", pixel[0], pixel[1], pixel[2]);
			}
			else{
				
				mouseInitialPosition->x = x;
				mouseInitialPosition->y = y;
			}
		}
	}
}

void mouseMove(int x, int y){
	if (!picking){
		if (directorClicked){
			camera2.yawAngle += (x - mouseInitialPosition->x)*0.5;
			camera2.pitchAngle += (y - mouseInitialPosition->y)*0.5;
		}
		else{
			camera.yawAngle += (x - mouseInitialPosition->x)*0.5;
			camera.pitchAngle += (y - mouseInitialPosition->y)*0.5;
		}
		mouseInitialPosition->x = x;
		mouseInitialPosition->y = y;
	}
}

int main(int argc, char** argv)
{
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);

	mainMenu(1);
	//mainMenu(4);

	glutCreateWindow("Hello World");
	glutInitWindowSize(800, 600);

	//glutFullScreen();

	glutDisplayFunc(display);

	glutReshapeFunc(reshape);

	glutKeyboardFunc(keyboard);

	glutMouseFunc(mousePress);

	glutMotionFunc(mouseMove);

	mousePosition = new Point(0, 0, 0);
	mouseInitialPosition = new Point(0, 0, 0);

	camera = Camera(0, 0, 50);
	camera2 = Camera(100, 100, 100);

	glutMainLoop();
	return 0;


}

