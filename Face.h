#ifndef __FACE_H_INCLUDED__
#define __FACE_H_INCLUDED__

#include "Point.h"
#include "Normal.h"

class Face{

	public:
		Point* v1;
		Point* v2;
		Point* v3;
		Normal* n1;
		Normal* n2;
		Normal* n3;
		Face(Point* v1, Point* v2, Point* v3, Normal* n1, Normal* n2, Normal* n3);
		Face(Point* v1, Point* v2, Point* v3);

};
#endif