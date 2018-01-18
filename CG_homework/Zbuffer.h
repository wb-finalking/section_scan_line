#ifndef __ZbufferHHHH__
#define __ZbufferHHHH__
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <vector>
#include <list>
#include <algorithm>
#include <math.h>
#include <time.h>
#include <opencv2/core/core.hpp>  
#include <opencv2/highgui/highgui.hpp>
#include "structure.h"

using namespace std;

#define MAX_WIDTH (1024*2)
#define MAX_HEIGHT (768*2)

//image size
#define	WIDTH 1024
#define	HEIGHT 768
extern unsigned char image[HEIGHT][WIDTH][3];
extern char	buf[1024];
extern double light[3];
extern double eye[3];



//class Zbuffer
//{
//public:

extern vector <Point_t> point;
extern double max_scale;
extern double scale;
extern vector <Poly*>	poly[MAX_HEIGHT];
extern vector <Line> line[MAX_HEIGHT];
extern vector <Poly*> ape;
extern list <Active_Line> aet;
extern double z[MAX_WIDTH];
extern Color color[MAX_WIDTH];

	/*Zbuffer(char *filename)
	{
		readfile(filename);
		draw();
	}*/

	bool cmp(const Active_Line &lhs, const Active_Line &rhs);
	double dot(double light[3], double eye[3], double a, double b, double c);
	void addLine(Poly *poly, int &poly_idx, int &line_idx, int idx1, int idx2);
	void readfile(const char *filename);
	void draw();
	void rotate(double thetax, double thetay);
	bool IsRemove(Poly* &obj1, int id);

	inline bool is_zero(double f)
	{
		if (abs(f) < 1e-8)
			return true;
		return false;
	}
	
//};

	

	class testclass_remove
	{
	public:
		int m_iKey;

		bool operator( ) (Poly* &obj1)
		{
			return obj1->id == m_iKey;
		}

		testclass_remove(int iKey){ m_iKey = iKey; };
	};

#endif