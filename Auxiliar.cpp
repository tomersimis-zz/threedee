#include "Vector.h";
#include "Point.h";
#include "Face.h"
#include "Auxiliar.h"
#include <cmath>;
#define EPS 1e-7

bool cmpDouble(double a, double b){
	if (fabs(a - b) <= EPS) return 0;
	return (a > b) ? 1 : -1;

}

//ainda não testei mas foi o algoritmo do site que eu transcrevi
//TESTAR

//dado um ponto e uma face, retorna o ponto projetado nela
Point Auxiliar::project_point(Face face, Point origin_point) {
	Vector v = Vector(face.v1, &origin_point);
	printf("Vetor dif -> %lf %lf %lf\n", v.x, v.y, v.z);

	//printf("face pra normal: %lf %lf %lf %lf %lf %lf %lf %lf %lf", face.v1->x, face.v1->y, face.v1->z, face.v2->x, face.v2->y, face.v2->z, face.v3->x, face.v3->y, face.v3->z);
	face.calculateNormal();
	Vector normal = *(face.fn);
	printf("Vetor mprmal -> %lf %lf %lf\n", normal.x, normal.y, normal.z);
	int dotRes = v.dotProduct(normal);
	printf("pos dot produt %d\n", dotRes);
	normal.multitplyScalar(dotRes);
	printf("Vetor mprmal multiply> %lf %lf %lf\n", normal.x, normal.y, normal.z);
	origin_point.x -= normal.x;
	origin_point.y -= normal.y;
	origin_point.z -= normal.z;

	return origin_point;

}

Auxiliar::Auxiliar(){


}

bool Auxiliar::pointIn(Point point, Face face){

	Point projected = this->project_point(face, point);
	Vector a, b;
	double angle = 0.0;
	a = Vector(&projected, face.v1);
	b = Vector(&projected, face.v2);
	
	angle += a.angleBetween(b); // v1_v2

	printf("1 Angle: %lf \n", a.angleBetween(b));

	b = Vector(&projected, face.v3);

	angle += a.angleBetween(b); // v1_v3

	printf("2 Angle: %lf \n", a.angleBetween(b));

	a = Vector(&projected, face.v3);
	 
	angle += a.angleBetween(b); // v2_v3

	printf("3 Angle: %lf \n", a.angleBetween(b));
	printf("Angle total: %lf\n", angle);

	return cmpDouble(angle, acos(-1)) == 0;

}

/*

Main test

Point a = Point(0, 0, 0);
Point b = Point(1, 1, 0);
Point c = Point(1, 3, 0);
Point d = Point(1.2, 1.9, 2033);
Face f = Face(a, b, c);
int main(){
Auxiliar aux = Auxiliar();
//printf("\nface: %lf %lf %lf %lf %lf %lf %lf %lf %lf\n", f.v1->x, f.v1->y, f.v1->z, f.v2->x, f.v2->y, f.v2->z, f.v3->x, f.v3->y, f.v3->z);
Point proj = aux.project_point(f, d);
printf("%lf %lf %lf\n", proj.x, proj.y, proj.z);
int marlon;
printf("Pausa!\n");
scanf("%d", &marlon);

return 0;
}
*/