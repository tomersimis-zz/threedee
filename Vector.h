#ifndef __VECTOR_H_INCLUDED__
#define __VECTOR_H_INCLUDED__

#include "Point.h"

class Vector{

	public:
		double x;
		double y;
		double z;
		Vector(double x, double y, double z);
		Vector(double x, double y);
		Vector(Point* from, Point* to);
		Vector(Point from, Point to);
		Vector();
		double getLength();
		void normalize();
		void add(Vector v);
		void add(Vector *v);
		void add(Point *p);
		void subtract(Vector v);
		static Vector cross(Vector v1, Vector v2);
		long long int dotProduct(Vector v);
		void multitplyScalar(int k);
		double norm();
		double angleBetween(Vector v);
		double dot(Vector v);


};

#endif