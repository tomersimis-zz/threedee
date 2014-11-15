#include "Normal.h"

Normal::Normal(double x, double y, double z){
	this->x = x;
	this->y = y;
	this->z = z;
}


Normal::Normal(double x, double y){
	this->x = x;
	this->y = y;
	this->z = 0;
}

Normal::Normal(){
	x=0;
	y =0 ;
	z =0;
}
