#ifndef _TRANSFORM_H_INCLUDED_
#define _TRANSFORM_H_INCLUDED_

class Transform{

	public:
		static void translate(double x, double y, double z, double* m);
		static void rotateX(double angle, double* m);
		static void rotateY(double angle, double* m);
		static void rotateZ(double angle, double* m);
		static void scale(double factor, double* m);

};


#endif