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