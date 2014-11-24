#include "Face.h"

Face::Face(Point* v1, Point* v2, Point* v3){
	this->v1 = v1;
	this->v2 = v2;
	this->v3 = v3;
}

Face::Face(Point* v1, Point* v2, Point* v3, Vector* n1, Vector* n2, Vector* n3){
	Face(v1, v2, v3);
	this->n1 = n1;
	this->n2 = n2;
	this->n3 = n3;
}

Face::Face(){
	v1 = (  Point*) malloc(1*sizeof(Point));
	v2 = (  Point*) malloc(1*sizeof(Point));
	v3 = (  Point*) malloc(1*sizeof(Point));
	
}

Face::Face(Point a, Point b, Point c){
	v1 = (  Point*) malloc(1*sizeof(Point));
	v2 = (  Point*) malloc(1*sizeof(Point));
	v3 = (  Point*) malloc(1*sizeof(Point));
	
	n1 = (  Vector*) malloc(1*sizeof(Vector));
	n2 = (  Vector*) malloc(1*sizeof(Vector));
	n3 = (  Vector*) malloc(1*sizeof(Vector));
	
	v1->x = a.x;
	v1->y = a.y;
	v1->z = a.z;
	
	v2->x = b.x;
	v2->y = b.y;
	v2->z = b.z;
	
	v3->x = c.x;
	v3->y = c.y;
	v3->z = c.z;
}


Face::Face(Point a, Point b, Point c, Vector d, Vector e, Vector f){
	v1 = (  Point*) malloc(1*sizeof(Point));
	v2 = (  Point*) malloc(1*sizeof(Point));
	v3 = (  Point*) malloc(1*sizeof(Point));
	
	n1 = (  Vector*) malloc(1*sizeof(Vector));
	n2 = (  Vector*) malloc(1*sizeof(Vector));
	n3 = (  Vector*) malloc(1*sizeof(Vector));	
	
	v1->x = a.x; n1->x = d.x;
	v1->y = a.y; n1->y = d.y;
	v1->z = a.z; n1->z = d.z;
	
	v2->x = b.x; n2->x = e.x;
	v2->y = b.y; n2->y = e.y;
	v2->z = b.z; n2->z = e.z;
	
	v3->x = c.x; n3->x = f.x;
	v3->y = c.y; n3->y = f.y;
	v3->z = c.z; n3->z = f.z;
	

}

Face::Face(Point* v1, Point* v2, Point* v3, Vector* fn){
	this->v1 = v1;
	this->v2 = v2;
	this->v3 = v3;
	this->fn = fn;
}

void Face::calculateNormal(){
	Point *x1 = new Point(0, 0, 0), *x2 = new Point(0, 0, 0);
	double len;

	x1->x = v2->x - v1->x;
	x1->y = v2->y - v1->y;
	x1->z = v2->z - v1->z;

	x2->x = v3->x - v1->x;
	x2->y = v3->y - v1->y;
	x2->z = v3->z - v1->z;

	/*cross product between v1 and v2*/
	fn->x = (x1->y * x2->z) - (x1->z * x2->y);
	fn->y = (x1->z * x2->x) - (x1->x * x2->z);
	fn->z = (x1->x * x2->y) - (x1->y * x2->x);

	len = sqrt(fn->x*fn->x + fn->y*fn->y + fn->z*fn->z);
	fn->x /= len;
	fn->y /= len;
	fn->z /= len;
}

/*
g++ -o foo ProjetoPG.cpp -lglut -lGLU -lGL -lm
*/
