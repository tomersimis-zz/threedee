#include "Vector.h";
#include "Point.h";
#include<cmath>;
#include <cstdio>
#define EPS 1e-7
Vector::Vector(){
	this->x = 0;
	this->y = 0;
	this->z = 0;
}

Vector::Vector(double x, double y, double z){
	this->x = x;
	this->y = y;
	this->z = z;
}
Vector::Vector(double x, double y){
	this->x = x;
	this->y = y;
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

void Vector::add(Vector v){
	this->x += v.x;
	this->y += v.y;
	this->z += v.z;
}

void Vector::subtract(Vector v){
	this->x -= v.x;
	this->y -= v.y;
	this->z -= v.z;
}

// a × b = [a2b3 − a3b2, a3b1 − a1b3, a1b2 − a2b1]
Vector Vector::cross(Vector v1, Vector v2){
	return Vector(v1.y*v2.z - v1.z*v2.y, v1.z*v2.x - v1.x*v2.z, v1.x*v2.y - v1.y*v2.x);
}

long long int Vector::dotProduct(Vector v){
	return (this->x * (v.x)) + (this->y * (v.y)) + (this->z * (v.z));
}

void Vector::multitplyScalar(int k){
	this->x *= k;
	this->y *= k;
	this->z *= k;
}

double  Vector::norm(){
	return sqrt( (this->x*this->x) + (this->y*this->y) + (this->z * this->z)  );
}

double  Vector::angleBetween(Vector v){
	double normProd = this->norm()  * v.norm();
	//printf("%lf %lf %lf --> norm: %lf  || his norm: %lf\n", this->x, this->y, this->z, this->norm(), v.norm() );
//	printf("");
	if (fabs(normProd) < EPS) normProd = 10 * EPS;

	double docProd = this->dotProduct(v);
	
	return acos(  docProd/normProd  );

}
