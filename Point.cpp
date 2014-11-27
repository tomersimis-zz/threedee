#include "Point.h"
#include "Vector.h"
#include <cmath>
#include <cstdio>

Point::Point(double x, double y, double z){
	this->x = x;
	this->y = y;
	this->z = z;
}

Point::Point(double x, double y, double z, int index){
	this->x = x;
	this->y = y;
	this->z = z;
	this->index = index;
}


Point::Point(double x, double y){
	this->x = x;
	this->y = y;
	this->z = 0;

}

Point::Point(){
	this->x = 0; this->y = 0; this->z = 0;
}

void Point::sum(double x, double y, double z){
	this->x += x;
	this->y += y;
	this->z += z;
}



double Point::distanceFrom(Point a){
	double dx = (this->x - a.x); dx *= dx;
	double dy = (this->y - a.y); dy *= dy;
	double dz = (this->z - a.z); dz *= dz;
	printf("[Distance]: dx: %lf | dy: %lf | dz: %lf\n", dx, dy, dz);

	return sqrt(dx + dy + dz);
}

Point Point::rotate(double angle, int type){
	double rotMatrix[3][3] = {
		{ 1, 0, 0 },
		{ 0, 1, 0 },
		{ 0, 0, 1 }
	};
	if (type == 0){
		rotMatrix[1][1] = cos(angle);
		rotMatrix[1][2] = -sin(angle);
		rotMatrix[2][1] = sin(angle);
		rotMatrix[2][2] = cos(angle);
	}
	if (type == 1) {
		rotMatrix[0][0] = cos(angle);
		rotMatrix[0][2] = sin(angle);
		rotMatrix[2][0] = -sin(angle);
		rotMatrix[2][2] = cos(angle);
	}
	if (type == 2) {
		rotMatrix[0][0] = cos(angle);
		rotMatrix[0][1] = -sin(angle);
		rotMatrix[1][0] = sin(angle);
		rotMatrix[1][1] = cos(angle);
	}
	double dx = 0, dy = 0, dz = 0;
	dx = (rotMatrix[0][0] * (this->x)) + (rotMatrix[0][1] * (this->y)) + (rotMatrix[0][2] * (this->z));
	dy = (rotMatrix[1][0] * (this->x)) + (rotMatrix[1][1] * (this->y)) + (rotMatrix[1][2] * (this->z));
	dz = (rotMatrix[2][0] * (this->x)) + (rotMatrix[2][1] * (this->y)) + (rotMatrix[2][2] * (this->z));
	return Point(dx, dy, dz);
}

Point Point::multiplyScalar(double k){
	return Point(this->x *k, this->y *k, this->z *k);
}