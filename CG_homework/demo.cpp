#include <opencv2/core/core.hpp>  
#include <opencv2/highgui/highgui.hpp>  
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string.h>
#include <vector>
#include <algorithm>
#include <math.h>
#include <time.h>
#include "Zbuffer.h"
#include "structure.h"

using namespace std;

unsigned char image[HEIGHT][WIDTH][3];
char	buf[1024];

vector <Point_t> point;
double max_scale;
double scale=1;
vector <Poly*>	poly[MAX_HEIGHT];			// polygon list
vector <Line> line[MAX_HEIGHT];				// edges list								
vector <Poly*> ape;									// active polygon list
list <Active_Line> aet;							// active edges list
double z[MAX_WIDTH];								// z buffer
Color color[MAX_WIDTH];							// color buffer
double light[3] = { HEIGHT, WIDTH ,- 5 };
double eye[3] = { HEIGHT/2, WIDTH/2, 0 };

#define use_crosspoint

void main()
{
	readfile("test.obj");

	/*for (int i = 0; i < point.size(); i++)
		printf("%f %f %f\n", point[i].x, point[i].y, point[i].z);*/
	draw();
	cv::Mat img(HEIGHT, WIDTH, CV_8UC3, (unsigned char*)image);
	cv::namedWindow("demo");
	cv::imshow("demo", img);
	//cv::waitKey();
	while (1)
	{
		rotate(0, 0.1);
		cv::imshow("demo", img);
		cv::waitKey(1);
	}

	/*for (int i = 0; i < 100; i++)
		cout << rand() << endl;*/
}

