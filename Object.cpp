#include "Object.h"

Object::Object(std::vector<Point*> vertex, std::vector<Face*> faces, std::vector<Vector*> normals){
	this->vertex = vertex;
	this->faces = faces;
	this->normals = normals;
}

void Object::calculateNormals(){
	int size = this->faces.size();
	for (int i = 0; i < size; i++){
		this->faces[i]->calculateNormal();
	}
}