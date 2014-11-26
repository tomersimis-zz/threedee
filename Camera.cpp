#include "Camera.h"
#include<math.h>
#include<cstdio>

Camera::Camera(double x, double y, double z){
	position = Point(x, y, z);
	look = Point(0, 0, 0);
	rollAngle = 0;
	yawAngle = 0;
	pitchAngle = 0;
}

double* Camera::getMatrix(){
	Vector p = Vector(position.x, position.y, position.z);

	forward = Vector(look, position);
	forward.normalize();

	side = Vector::cross(forward, Vector(0, 1, 0));
	side.normalize();

	up = Vector::cross(forward, side);

	this->roll();
	this->yaw();
	this->pitch();

	double m[16] = {
		side.x, up.x, forward.x, 0,
		side.y, up.y, forward.y, 0,
		side.z, up.z, forward.z, 0,
		-p.dot(side), -p.dot(up), -p.dot(forward), 1
	};

	return m;
}

void Camera::slide(double x, double y, double z){
	position.x += x*side.x + y*up.x + z*forward.x;
	position.y += x*side.y + y*up.y + z*forward.y;
	position.z += x*side.z + y*up.z + z*forward.z;

	look.x += x*side.x + y*up.x + z*forward.x;
	look.y += x*side.y + y*up.y + z*forward.y;
	look.z += x*side.z + y*up.z + z*forward.z;
}

void Camera::roll(){
	float cs = cos(rollAngle*3.14159265 / 180);
	float sn = sin(rollAngle*3.14159265 / 180);
	Vector t = Vector(side.x, side.y, side.z);

	this->side.x = cs*t.x - sn*up.x;
	this->side.y = cs*t.y - sn*up.y;
	this->side.z = cs*t.z - sn*up.z;

	this->up.x = sn*t.x - cs*up.x;
	this->up.y = sn*t.y - cs*up.y;
	this->up.z = sn*t.z - cs*up.z;

}

void Camera::yaw(){
	float cs = cos(yawAngle*3.14159265 / 180);
	float sn = sin(yawAngle*3.14159265 / 180);
	Vector t = Vector(forward.x, forward.y, forward.z);
	Vector s = Vector(side.x, side.y, side.z);

	forward.x = cs*t.x - sn*s.x;
	forward.y = cs*t.y - sn*s.y;
	forward.z = cs*t.z - sn*s.z;

	side.x = sn*t.x + cs*s.x;
	side.y = sn*t.y + cs*s.y;
	side.z = sn*t.z + cs*s.z;
}

void Camera::pitch(){
	float cs = cos(pitchAngle*3.14159265 / 180);
	float sn = sin(pitchAngle*3.14159265 / 180);
	Vector t = Vector(up.x, up.y, up.z);
	Vector s = Vector(forward.x, forward.y, forward.z);

	up.x = cs*t.x - sn*s.x;
	up.y = cs*t.y - sn*s.y;
	up.z = cs*t.z - sn*s.z;

	forward.x = sn*t.x + cs*s.x;
	forward.y = sn*t.y + cs*s.y;
	forward.z = sn*t.z + cs*s.z;
}