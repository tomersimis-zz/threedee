#ifndef __POINT_H_INCLUDED__
#define __POINT_H_INCLUDED__

class Point{

	public:
		double x;
		double y;
		double z;
		int index;
		Point(double x, double y, double z);
		Point(double x, double y);
		Point(double x, double y, double z, int index);
		Point();
		void sum(double x, double y, double z);
		double distanceFrom(Point a);
		Point rotate(double angle, int type);
		Point multiplyScalar(double k);
};

#endif
