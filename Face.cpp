#include "Face.h"


Face::Face(Point* v1, Point* v2, Point* v3){
	this->v1 = v1;
	this->v2 = v2;
	this->v3 = v3;
}

Face::Face(Point* v1, Point* v2, Point* v3, Normal* n1, Normal* n2, Normal* n3){
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
	
	n1 = (  Normal*) malloc(1*sizeof(Normal));
	n2 = (  Normal*) malloc(1*sizeof(Normal));
	n3 = (  Normal*) malloc(1*sizeof(Normal));
	
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


Face::Face(Point a, Point b, Point c, Normal d, Normal e, Normal f){
	v1 = (  Point*) malloc(1*sizeof(Point));
	v2 = (  Point*) malloc(1*sizeof(Point));
	v3 = (  Point*) malloc(1*sizeof(Point));
	
	n1 = (  Normal*) malloc(1*sizeof(Normal));
	n2 = (  Normal*) malloc(1*sizeof(Normal));
	n3 = (  Normal*) malloc(1*sizeof(Normal));	
	
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

Face::Face(Point* v1, Point* v2, Point* v3, Normal* fn){
	this->v1 = v1;
	this->v2 = v2;
	this->v3 = v3;
	this->fn = fn;
}

/*
g++ -o foo ProjetoPG.cpp -lglut -lGLU -lGL -lm
*/
