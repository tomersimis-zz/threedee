#ifndef __VECTOR_H_INCLUDED__
#define __VECTOR_H_INCLUDED__

#include "Point.h";

class Vector{

	public:
		double x;
		double y;
		double z;
		Vector(double x, double y, double z);
		Vector(Point* from, Point* to);
		double getLength();
		void normalize();
		static Vector cross(Vector v1, Vector v2);
			
};

#endif