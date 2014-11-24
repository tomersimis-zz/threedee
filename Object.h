#ifndef __OBJECT_H_INCLUDED__
#define __OBJECT_H_INCLUDED__

#include<vector>
#include "Point.h"
#include "Face.h"

class Object{

	public:
		double rotationX = 0;
		double rotationY = 0;
		double rotationZ = 0;
		double translationX = 0;
		double translationY = 0;
		double translationZ = 0;
		double scale = 1;
		std::vector<Point*> vertex;
		std::vector<Face*> faces;
		std::vector<Vector*> normals;
		Object();
		Object(std::vector<Point*> vertex, std::vector<Face*> faces, std::vector<Vector*> normals);
		void calculateNormals();
		void calculateNormal();

};

#endif