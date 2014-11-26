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

void Object::calculateVertexNormals(){
	int size = this->faces.size();
	for (int i = 0; i < size; i++){
		this->normals[this->faces.at(i)->v1->index]->add(this->faces.at(i)->fn);
		this->normals[this->faces.at(i)->v2->index]->add(this->faces.at(i)->fn);
		this->normals[this->faces.at(i)->v3->index]->add(this->faces.at(i)->fn);
	}
	for (int i = 0; i < size; i++){
		this->normals[this->faces.at(i)->v3->index]->normalize();
	}
}
