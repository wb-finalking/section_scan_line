#ifndef __Structure__
#define __Structure__
#include <vector>
#include <list>

struct  Poly;
struct Point_t
{
	double x, y, z;
};
struct Line
{
	int x;						// start x
	int point1,point2;
	double dx;			// x1-x2
	int dy;					// number of crossed scaning-line(y1-y2)
	int id;					//polygon id
	Poly* poly;			//polygon pointer
};

struct Active_Line
{
	Poly* poly;			//	polygon pointer
	int id;
	double x;				// start x
	double dx;			// dx(-1/k)
	int dy;					// number of crossed scaning-line
	double z;				// depth of start point
	double dzx;			// depth variance along x, dzx = -a/c (c!= 0)
	double dzy;			// depth variance along y, dzy = b/c (c!= 0)
	//double e,m;				//	Bresenham Algorithm

};

struct Color
{
	unsigned char r, g, b,a;
};

struct  Poly
{
	int id;						// polygon id
	double a, b, c, d;		// ax + by + cz + d = 0
	int dy;						// number of crossed scaning-line
	Color color;				// polygon color
	bool flag;
	
	Poly() : flag(false){}
	std::vector <int> point;
};

#endif