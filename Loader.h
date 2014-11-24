#ifndef __LOADER_H_INCLUDED__
#define __LOADER_H_INCLUDED__

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cmath>
#include <iostream>
#include <string>
#include <algorithm>
#include <vector>


#include "Point.h"
#include "Face.h"
#include "Vector.h"
#include "Vector.h"
#include <fstream>
#include <string>
using namespace std;





class Loader {
public:
	Loader();
	Loader(string *_path, vector<Point> *_points, vector<Vector> *_vectors, vector<Face> *_faces);
	int load();
private:
	long long int parseInt(int i, int j);
	double parseDouble(int i, int j);
	string *path;
	vector<Point> *points;
	vector<Face> *faces;
	vector<Vector> *vectors;
	string strParse;
	int findPatternFace(int len);
	void parseFace(int pattern);
};

#endif