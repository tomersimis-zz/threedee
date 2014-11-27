#include "Transform.h"
#include<math.h>
#include<string>

void Transform::translate(double x, double y, double z, double* m){
	m[0] = 1;  m[1] = 0;  m[2] = 0;  m[3] = 0;
	m[4] = 0;  m[5] = 1;  m[6] = 0;  m[7] = 0;
	m[8] = 0;  m[9] = 0;  m[10] = 1; m[11] = 0;
	m[12] = x; m[13] = y; m[14] = z; m[15] = 1;
}

void Transform::scale(double scale, double* m){
	m[0] = scale;  m[1] = 0;  m[2] = 0;  m[3] = 0;
	m[4] = 0;  m[5] = scale;  m[6] = 0;  m[7] = 0;
	m[8] = 0;  m[9] = 0;  m[10] = scale; m[11] = 0;
	m[12] = 0; m[13] = 0; m[14] = 0; m[15] = 1;
}

void Transform::rotateZ(double angle, double* m){
	m[0] = cos(angle);  m[1] = sin(angle);  m[2] = 0;  m[3] = 0;
	m[4] = -sin(angle);  m[5] = cos(angle);  m[6] = 0;  m[7] = 0;
	m[8] = 0;  m[9] = 0;  m[10] = 1; m[11] = 0;
	m[12] = 0; m[13] = 0; m[14] = 0; m[15] = 1;
}

void Transform::rotateY(double angle, double* m){
	m[0] = cos(angle);  m[1] = 0;  m[2] = sin(angle);  m[3] = 0;
	m[4] = 0;  m[5] = 1;  m[6] = 0;  m[7] = 0;
	m[8] = -sin(angle);  m[9] = 0;  m[10] = cos(angle); m[11] = 0;
	m[12] = 0; m[13] = 0; m[14] = 0; m[15] = 1;
}

void Transform::rotateX(double angle, double* m){
	m[0] = 1;  m[1] = 0;  m[2] = 0;  m[3] = 0;
	m[4] = 0;  m[5] = cos(angle);  m[6] = sin(angle);  m[7] = 0;
	m[8] = 0;  m[9] = -sin(angle);  m[10] = cos(angle); m[11] = 0;
	m[12] = 0; m[13] = 0; m[14] = 0; m[15] = 1;
}
