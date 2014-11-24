#ifndef __AUXILIAR_H_INCLUDED__
#define __AUXILIAR_H_INCLUDED__


#include "Face.h"
#include "Vector.h"
#include "Point.h"


class Auxiliar{

public:
	bool pointIn(Point point, Face face);
	Point project_point(Face face, Point origin_point);
	Auxiliar();

};

#endif