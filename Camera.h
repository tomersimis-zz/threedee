#ifndef __CAMERA_H_INCLUDED__
#define __CAMERA_H_INCLUDED__

#include "Point.h"
#include "Vector.h"

class Camera{

public:
	Point position;
	Point look;
	Vector forward;
	Vector side;
	Vector up;
	double rollAngle;
	double yawAngle;
	double pitchAngle;
	Camera(double x = 0, double y = 0, double z = 0);
	double* getMatrix();
	void pitch();
	void roll();
	void yaw();
	void slide(double x, double y, double z);
	void setVectors();


};

#endif