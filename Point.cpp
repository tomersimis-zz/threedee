#include "Point.h"
#include "Vector.h"


Point::Point(double x=0, double y=0, double z=0){
	this->x = x;
	this->y = y;
	this->z = z;
}


Point::Point(double x=0, double y=0){
	this->x = x;
	this->y = y;
	this->z = 0;
}

Point::Point(){
	this->x = 0; this->y = 0; this->z = 0;
}



