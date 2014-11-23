#ifndef __FACE_H_INCLUDED__
#define __FACE_H_INCLUDED__

#include "Point.h"
#include "Normal.h"
#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <cstring>

class Face{

	public:
		Point* v1;
		Point* v2;
		Point* v3;
		Normal* n1;
		Normal* n2;
		Normal* n3;
		Normal* fn; //face normal
		Face(Point* v1, Point* v2, Point* v3, Normal* n1, Normal* n2, Normal* n3);
		Face(Point* v1, Point* v2, Point* v3);
		Face();
		Face(Point a, Point b, Point c);
		Face(Point a, Point b, Point c, Normal d, Normal e, Normal f);
		Face(Point* v1, Point* v2, Point* v3, Normal *fn);

};
#endif
