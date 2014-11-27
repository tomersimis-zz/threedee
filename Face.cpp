#include "Face.h"
#define EPS 1e-7
Face::Face(Point* v1, Point* v2, Point* v3){
	this->v1 = v1;
	this->v2 = v2;
	this->v3 = v3;
	//printf("Construtor 5/n");
	this->R = 1;  this->G = this->B = 0;
}

Face::Face(Point* v1, Point* v2, Point* v3, Vector* n1, Vector* n2, Vector* n3){
	this->v1 = v1;
	this->v2 = v2;
	this->v3 = v3;
	this->n1 = n1;
	this->n2 = n2;
	this->n3 = n3;
	this->fn = new Vector(0, 0, 0);
	//printf("Construtor 4/n");
	this->R = 1;  this->G = this->B = 0;
}

Face::Face(){

	v1 = (Point*)malloc(1 * sizeof(Point));
	v1->x = 0, v1->y = 0, v1->z = 0;
	v2 = (Point*)malloc(1 * sizeof(Point));
	v2->x = 0, v2->y = 0, v2->z = 0;
	v3 = (Point*)malloc(1 * sizeof(Point));
	v3->x = 0, v3->y = 0, v3->z = 0;
	this->fn = new Vector(0, 0, 0);
	//	printf("Construtor 6/n"); 
	this->R = 1;  this->G = this->B = 0;
}

Face::Face(Point a, Point b, Point c){

	v1 = (Point*)malloc(1 * sizeof(Point));
	v1->x = a.x, v1->y = a.y, v1->z = a.z; v1->index = a.index;

	v2 = (Point*)malloc(1 * sizeof(Point));
	v2->x = b.x, v2->y = b.y, v2->z = b.z; v2->index = b.index;
	v3 = (Point*)malloc(1 * sizeof(Point));
	v3->x = c.x, v3->y = c.y, v3->z = c.z; v3->index = c.index;
	/*v1 = new Point(a.x, a.y, a.z);
	v2 = new Point(b.x, b.y, b.z);
	v3 = new Point(c.x, c.y, c.z);
	*/
	//printf("Construtor 1 -> [%lf %lf %lf] /n" ,a.x, a.y, a.z);

	n1 = (Vector*)malloc(1 * sizeof(Vector));
	n2 = (Vector*)malloc(1 * sizeof(Vector));
	n3 = (Vector*)malloc(1 * sizeof(Vector));
	this->fn = new Vector(0, 0, 0);
	this->R = 1;  this->G = this->B = 0;

}


Face::Face(Point a, Point b, Point c, Vector d, Vector e, Vector f){
	v1 = new Point(a.x, a.y, a.z); v1->index = a.index;
	v2 = new Point(b.x, b.y, b.z); v2->index = b.index;
	v3 = new Point(c.x, c.y, c.z); v3->index = c.index;

	n1 = (Vector*)malloc(1 * sizeof(Vector));
	n2 = (Vector*)malloc(1 * sizeof(Vector));
	n3 = (Vector*)malloc(1 * sizeof(Vector));

	n1->x = d.x;
	n1->y = d.y;
	n1->z = d.z;

	n2->x = e.x;
	n2->y = e.y;
	n2->z = e.z;

	n3->x = f.x;
	n3->y = f.y;
	n3->z = f.z;
	// printf("Construtor 2/n");
	this->fn = new Vector(0, 0, 0);
	this->R = 1;  this->G = this->B = 0;
}

Face::Face(Point* v1, Point* v2, Point* v3, Vector* fn){
	this->v1 = v1;
	this->v2 = v2;
	this->v3 = v3;
	this->fn = fn;
	//printf("Construtor 3/n");
	this->R = 1;  this->G = this->B = 0;
}

void Face::calculateNormal(){
	Point *x1 = new Point(0, 0, 0), *x2 = new Point(0, 0, 0);
	double len;


	//printf("\nDentro metodo face: %lf %lf %lf %lf %lf %lf %lf %lf %lf\n", v1->x, v1->y, v1->z, v2->x, v2->y, v2->z, v3->x, v3->y, v3->z);
	x1->x = v2->x - v1->x;
	x1->y = v2->y - v1->y;
	x1->z = v2->z - v1->z;

	x2->x = v3->x - v1->x;
	x2->y = v3->y - v1->y;
	x2->z = v3->z - v1->z;

	if ((fabs(x1->x) <= EPS && fabs(x1->y) <= EPS && fabs(x1->z) <= EPS) || (fabs(x2->x) <= EPS && fabs(x2->y) <= EPS && fabs(x2->z) <= EPS)){

	labelNulo:;
		fn->x = 0;
		fn->y = 0;
		fn->z = 0;

	}
	else {
		/*cross product between v1 and v2*/
		fn->x = (x1->y * x2->z) - (x1->z * x2->y);
		fn->y = (x1->z * x2->x) - (x1->x * x2->z);
		fn->z = (x1->x * x2->y) - (x1->y * x2->x);

		//printf("normal: %lf lf %lf\n", fn->x, fn->y, fn->z);

		len = sqrt(fn->x*fn->x + fn->y*fn->y + fn->z*fn->z);

		if (fabs(len) <= EPS) goto labelNulo;
		fn->x /= len;
		fn->y /= len;
		fn->z /= len;
	}
}

Point Face::centroid(){
	double dx = (this->v1->x + this->v2->x + this->v3->x) / 3.0;
	double dy = (this->v1->y + this->v2->y + this->v3->y) / 3.0;
	double dz = (this->v1->z + this->v2->z + this->v3->z) / 3.0;
	return Point(dx, dy, dz);
}