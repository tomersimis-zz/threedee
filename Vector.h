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
		Vector();
		double getLength();
		void normalize();
		void add(Vector v);
		void subtract(Vector v);
		static Vector cross(Vector v1, Vector v2);
};

#endif