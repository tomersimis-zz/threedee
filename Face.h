#ifndef __FACE_H_INCLUDED__
#define __FACE_H_INCLUDED__

#include "Point.h"
#include "Vector.h"
#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <cstring>

class Face{

	public:
		Point* v1;
		Point* v2;
		Point* v3;
		Vector* n1;
		Vector* n2;
		Vector* n3;
		Vector* fn; //face normal
		Face(Point* v1, Point* v2, Point* v3, Vector* n1, Vector* n2, Vector* n3);
		Face(Point* v1, Point* v2, Point* v3);
		Face();
		Face(Point a, Point b, Point c);
		Face(Point a, Point b, Point c, Vector d, Vector e, Vector f);
		Face(Point* v1, Point* v2, Point* v3, Vector *fn);
		void calculateNormal();
		double R;
		double G;
		double B;
		int materialIndex;
		Point centroid();

};
#endif
