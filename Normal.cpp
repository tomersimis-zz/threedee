#include "Normal.h"

Normal::Normal(double x=0, double y=0, double z=0){
	this->x = x;
	this->y = y;
	this->z = z;
}


Normal::Normal(double x=0, double y=0){
	this->x = x;
	this->y = y;
	this->z = 0;
}

Normal::Normal(){
	x=0;
	y =0 ;
	z =0;
}
