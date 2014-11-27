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

void Object::vertices(){
	for (int i = 0; i < this->vertex.size(); i++){
		printf("%d", this->vertex[i]->index);
	}
}

void Object::calculateVertexNormals(){
	int size = this->faces.size();
	int qlqer = 1;
	for (int i = 0; i < size; i++){
		if (this->faces.at(i)->v1->index == 0){
			printf("%f %f %f bla1\n", this->faces.at(i)->fn->x, this->faces.at(i)->fn->y, this->faces.at(i)->fn->z);
		}
		this->normals[this->faces.at(i)->v1->index]->add(this->faces.at(i)->fn);



		if (this->faces.at(i)->v2->index == 0){
			printf("%f %f %f bla2\n", this->faces.at(i)->fn->x, this->faces.at(i)->fn->y, this->faces.at(i)->fn->z);
		}
		this->normals[this->faces.at(i)->v2->index]->add(this->faces.at(i)->fn);




		if (this->faces.at(i)->v3->index == 0){
			printf("%f %f %f bla3\n", this->faces.at(i)->fn->x, this->faces.at(i)->fn->y, this->faces.at(i)->fn->z);
		}
		this->normals[this->faces.at(i)->v3->index]->add(this->faces.at(i)->fn);
	}
	size = this->normals.size();
	for (int i = 0; i < size; i++){
		this->normals[i]->normalize();
		if (i == 0){
			printf("%f %f %f he \n", normals[i]->x, normals[i]->y, normals[i]->z);
		}
	}
}
