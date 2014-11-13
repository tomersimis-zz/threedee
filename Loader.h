#ifndef __LOADER_H_INCLUDED__
#define __LOADER_H_INCLUDED__

#include <bits/stdc++.h>
#include "Point.h"
#include "Face.h"
#include "Vector.h"
#include "Normal.h"
#include <fstream>
#include <string>
using namespace std;





class Loader {
	public:
		Loader();
		Loader(string *_path, vector<Point> *_points, vector<Normal> *_normals, vector<Face> *_faces);
		int load();
	private: 
		long long int parseInt(int i, int j);
		double parseDouble(int i, int j);
		string *path;
		vector<Point> *points;
		vector<Face> *faces;
		vector<Normal> *normals;
		string strParse;
		int findPatternFace(int len);		
		Face parseFace(int pattern);
}; 

#endif
