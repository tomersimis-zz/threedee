#include "Vector.h";
#include "Point.h";
#include<cmath>;

Vector::Vector(double x, double y, double z){
	this->x = x;
	this->y = y;
	this->z = z;
}

Vector::Vector(Point* from, Point* to){
	this->x = to->x - from->x;
	this->y = to->y - from->y;
	this->z = to->z - from->z;
}

double Vector::getLength(){
	return sqrt(this->x*this->x + this->y*this->y + this->z*this->z);
}

void Vector::normalize(){
	double length = this->getLength();
	this->x = this->x / length;
	this->y = this->y / length;
	this->z = this->z / length;
}

// a × b = [a2b3 − a3b2, a3b1 − a1b3, a1b2 − a2b1]
Vector Vector::cross(Vector v1, Vector v2){
	return Vector(v1.y*v2.z - v1.z*v2.y, v1.z*v2.x - v1.x*v2.z, v1.x*v2.y - v1.y*v2.x);
}