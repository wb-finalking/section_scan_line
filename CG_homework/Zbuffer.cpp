#include "Zbuffer.h"

void readfile(const char *filename)
{
	FILE * file = fopen(filename, "r");
	if (file == NULL) {
		printf("open %s err!\n", filename);
		return;
	}
	srand(time(NULL));

	double max_xy = 0;
	int poly_idx = 0;    //face number
	int line_idx = 0;    //line number
	int v, n;

	while (fscanf(file, "%s", buf) != EOF) {
		switch (buf[0]) {
		case 'v':
			switch (buf[1]) {
			case '\0':
				fgets(buf, sizeof(buf), file);
				Point_t pt;
				sscanf(buf, "%lf%lf%lf", &(pt.x), &(pt.y), &(pt.z));
				if (abs(pt.x) > max_xy)
					max_xy = abs(pt.x);
				if (abs(pt.y) > max_xy)
					max_xy = abs(pt.y);
				point.push_back(pt);
				break;
			default:
				fgets(buf, sizeof(buf), file);
				break;
			}
			break;
		default:
			fgets(buf, sizeof(buf), file);
			break;
		}
	}
	printf("read %d points.\n", point.size());
	if (point.size() <= 0) {
		fclose(file);
		return;
	}

	
	if (WIDTH < HEIGHT) {
		max_scale = WIDTH / (2 * max_xy*scale);
	}
	else {
		max_scale = HEIGHT / (2 * max_xy*scale);
	}

	//previously transforming
	/*for (vector <Point_t> ::iterator it = point.begin(); it != point.end(); ++it) {
		it->x = int(it->x*max_scale + WIDTH / 2 + 0.5);
		it->y = HEIGHT - int(it->y*max_scale + HEIGHT / 2 + 0.5);
		it->z = int(it->z*max_scale + 0.5);
	}*/

	fseek(file, 0, SEEK_SET);
	while (fscanf(file, "%s", buf) != EOF) {
		switch (buf[0]) {
		case 'f':
			v = n = 0;
			//fscanf(file, "%s", buf);
			//if (strstr(buf, "//")) {
			if (1)
			{
				Line line_1;
				int max_idx;
				int min_idx;
				int idx[3];
				int max_y = 0;
				int min_y = 1;//equal to HEIGHT if previously normalizing node coordinate

				Poly *poly_1=new Poly;
				poly_1->id = poly_idx; // set ID of polygons

				poly_1->color.r = rand() % 0xFF;
				poly_1->color.g = rand() % 0xFF;
				poly_1->color.b = rand() % 0xFF;
				/*poly_1->color.r = 100 % 0xFF;
				poly_1->color.g = 100 % 0xFF;
				poly_1->color.b = 100 % 0xFF;*/

				fscanf(file, "%s", buf);
				sscanf(buf, "%d", &v);
				idx[0] = v - 1;
				poly_1->point.push_back(v - 1);

				fscanf(file, "%s", buf);
				sscanf(buf, "%d", &v);
				idx[1] = v - 1;
				poly_1->point.push_back(v - 1);

				fscanf(file, "%s", buf);
				sscanf(buf, "%d", &v);
				idx[2] = v - 1;
				poly_1->point.push_back(v - 1);

				/*fscanf(file, "%d//%d", &v, &n);
				idx[0] = v - 1;
				poly_1->point.push_back(v-1);
				fscanf(file, "%d//%d", &v, &n);
				idx[1] = v - 1;
				poly_1->point.push_back(v - 1);
				fscanf(file, "%d//%d", &v, &n);
				idx[2] = v - 1;
				poly_1->point.push_back(v - 1);*/

				for (int i = 0; i < 2; i++) {
					addLine(poly_1, poly_idx, line_idx, idx[i], idx[i + 1]);
				}

				poly_1->a = (point[idx[1]].y - point[idx[0]].y) * (point[idx[2]].z - point[idx[0]].z)
					- (point[idx[1]].z - point[idx[0]].z) * (point[idx[2]].y - point[idx[0]].y);
				poly_1->b = (point[idx[1]].z - point[idx[0]].z) * (point[idx[2]].x - point[idx[0]].x)
					- (point[idx[1]].x - point[idx[0]].x) * (point[idx[2]].z - point[idx[0]].z);
				poly_1->c = (point[idx[1]].x - point[idx[0]].x) * (point[idx[2]].y - point[idx[0]].y)
					- (point[idx[1]].y - point[idx[0]].y) * (point[idx[2]].x - point[idx[0]].x);
				//poly_1->d = -(poly_1->a * point[idx[0]].x + poly_1->b * point[idx[0]].y + poly_1->c * point[idx[0]].z);
				//if not transforming point coordinates previously
				poly_1->a = -max_scale*max_scale*poly_1->a;
				poly_1->b = max_scale*max_scale*poly_1->b;
				poly_1->c = -max_scale*max_scale*poly_1->c;
				poly_1->d = -(poly_1->a * point[idx[0]].x - poly_1->b * point[idx[0]].y + poly_1->c * point[idx[0]].z)*max_scale
					- poly_1->a*WIDTH / 2 - poly_1->b*HEIGHT / 2;


				//modifying color base on lighting
				/*double f = dot(light, eye, poly_1->a, poly_1->b, poly_1->c);
				poly_1->color.r = int(f*poly_1->color.r);
				poly_1->color.g = int(f*poly_1->color.g);
				poly_1->color.b = int(f*poly_1->color.b);*/

				// find the max and min value of y
				for (int i = 0; i < 3; i++) {
					if (int(point[idx[i]].y + 0.5) < min_y) {
						min_y = int(point[idx[i]].y + 0.5);
					}
					if (int(point[idx[i]].y + 0.5) > max_y) {
						max_y = int(point[idx[i]].y + 0.5);
					}
				}
				//when more than 3 lines
				/*while (fscanf(file, "%d//%d", &v, &n) > 0) {
					idx[1] = idx[2];
					idx[2] = v - 1;
					poly_1->point.push_back(v - 1);
					addLine(poly_1, poly_idx, line_idx, idx[1], idx[2]);

					if (int(point[idx[2]].y + 0.5) < min_y) {
						min_y = int(point[idx[2]].y + 0.5);
					}
					if (int(point[idx[2]].y + 0.5) > max_y) {
						max_y = int(point[idx[2]].y + 0.5);
					}
				}*/
				while (fscanf(file, "%d", &v) > 0) {
					fscanf(file, "%s", buf);
					idx[1] = idx[2];
					idx[2] = v - 1;
					poly_1->point.push_back(v - 1);
					addLine(poly_1, poly_idx, line_idx, idx[1], idx[2]);

					if (int(point[idx[2]].y + 0.5) < min_y) {
						min_y = int(point[idx[2]].y + 0.5);
					}
					if (int(point[idx[2]].y + 0.5) > max_y) {
						max_y = int(point[idx[2]].y + 0.5);
					}
				}
				idx[1] = idx[2];
				idx[2] = idx[0];
				addLine(poly_1,poly_idx, line_idx, idx[1], idx[2]);

				poly_1->dy = max_y - min_y + 1;
				poly[max_y].push_back(poly_1);
				poly_idx++;
			}
			else {
				//printf("TODO with others!\n");
				fgets(buf, sizeof(buf), file);
			}
			break;
		default:
			fgets(buf, sizeof(buf), file);
			break;
		}
	}

	fclose(file);
	// reading over
	printf("read %d polys, %d lines\n", poly_idx, line_idx);

}

void draw()
{
	int y;
	for (y = HEIGHT - 1; y >= 0; y--) {
		// initialization of background color
		memset(color, 0, sizeof(color));

		//for (vector <Poly*>::const_iterator it = poly[y].begin(); it != poly[y].end(); ++it) {
		//	ape.push_back(*it);
		//	//printf("add a new poly, now %d\n",ape.size());
		//}

		bool flag = false;
		// add new active edges
		for (vector <Line>::iterator it = line[y].begin(); it != line[y].end();it++)
		{
			Active_Line al;
			al.id = it->id;
			al.poly = it->poly;
			al.x = it->x;
			if (is_zero(it->dy)) {
				al.dx = 0;
			}
			else {
				al.dx = it->dx / it->dy;
			}
			al.dy = it->dy;
			if (is_zero(it->poly->c)) {
				al.z = 0;
				al.dzx = 0;
				al.dzy = 0;
			}
			else {
				al.z = -(it->poly->a*it->x + it->poly->b*y + it->poly->d) / it->poly->c;  // ax + by + cz + d = 0;
				al.dzx = -(it->poly->a / it->poly->c);
				al.dzy = it->poly->b / it->poly->c;
			}
			//al.e = abs(al.dx) - 0.5;
			aet.push_back(al);
		}
			
		//delete invalid edges
		for (list <Active_Line>::iterator it = aet.begin(); it != aet.end();)
		{
			if (it->dy <= 0) {
				it=aet.erase(it);
			}
			else {
				++it;
			}
		}

		//if have no active edges please skip
		if (aet.size() <= 0)
			continue;
		//sort for adjacent edges matching easily
		//aet.sort(aet.begin(), aet.end(), cmp);
		aet.sort(cmp);
		// shadering between 2 adjacent edges but a little hard for deciding color
		list <Active_Line>::iterator right = aet.begin();
		list<Poly*> idx;
		for (int j = 0; j < aet.size() - 1; j++)
		{
			list <Active_Line>::iterator left = right++;

			bool f = true;
			//for (list <Poly*>::iterator it = idx.begin(); it != idx.end();)
			//{
			//	if ((*it)->id == left->poly->id)
			//	{
			//		f = false;
			//		it = idx.erase(it);
			//	}
			//	else
			//		it++;
			//}
			//if (f)
			//	idx.push_back(left->poly);
			int size = idx.size();
			idx.remove_if(testclass_remove(left->id));
			if (size == idx.size())
				idx.push_back(left->poly);

			//Color tmp_color = ape[0].color;
			Color tmp_color;
			tmp_color.r = 0;
			tmp_color.b = 0;
			tmp_color.g = 0;
			tmp_color.a = 0;

			//vector <Poly*>::iterator it = idx.begin();
			double a, b, c, z1l, z2l, z1r, z2r;
			/*if (aet.size() % 2)
				printf("hello");*/
			list <Poly*>::iterator idx_it = idx.begin();
			list <Poly*>::iterator idx_it_tmp = idx.begin();
			if (idx.size() > 0)
			{
				tmp_color = (*idx_it)->color;
				a = (*idx_it)->a;
				b = (*idx_it)->b;
				c = (*idx_it)->c;
				z1l = -((*idx_it)->a*left->x + (*idx_it)->b*y + (*idx_it)->d) / (*idx_it)->c;
				z1r = -((*idx_it)->a*right->x + (*idx_it)->b*y + (*idx_it)->d) / (*idx_it)->c;
			}

			list<double> interpoint;

#ifdef use_crosspoint
			for (idx_it_tmp = idx.begin(); idx_it_tmp != idx.end(); idx_it_tmp++)
			{
				z1l = -((*idx_it_tmp)->a*left->x + (*idx_it_tmp)->b*y + (*idx_it_tmp)->d) / (*idx_it_tmp)->c;
				z1r = -((*idx_it_tmp)->a*right->x + (*idx_it_tmp)->b*y + (*idx_it_tmp)->d) / (*idx_it_tmp)->c;
				idx_it = idx_it_tmp;
				while (++idx_it !=idx.end())
				{//compare section z
					z2l = -((*idx_it)->a*left->x + (*idx_it)->b*y + (*idx_it)->d) / (*idx_it)->c;
					z2r = -((*idx_it)->a*right->x + (*idx_it)->b*y + (*idx_it)->d) / (*idx_it)->c;
					if (((z1l - z2l) * (z1r - z2r)) < 0)
					{
						//(- x2* (y3 - y1) + x1*(y4 - y2)) / (y1-y2-y3+y4)
						int tmp = int((-(right->x)* (z2l - z1l) + left->x*(z2r - z1r)) / (z1l - z1r - z2l + z2r) + 0.5);
						interpoint.push_back(tmp);
					}
				}
			}
#endif
			interpoint.sort();
			interpoint.push_back(right->x);

			double lp,rp;
			lp = left->x;
			vector<Color> tmp_colorv_ector;
			list <double>::iterator interpoint_it = interpoint.begin();
			for (list <double>::iterator interpoint_it = interpoint.begin(); interpoint_it != interpoint.end(); interpoint_it++)
			{
				rp = (*interpoint_it);
				if (idx.size() > 0)
				{
					idx_it = idx.begin();
					//rp = right->x;
					z1l = -((*idx_it)->a*lp + (*idx_it)->b*y + (*idx_it)->d) / (*idx_it)->c;
					z1r = -((*idx_it)->a*rp + (*idx_it)->b*y + (*idx_it)->d) / (*idx_it)->c;
				}

				for (int i = 1; i < idx.size(); i++)
				{//compare section z
					++idx_it;
					z2l = -((*idx_it)->a*lp + (*idx_it)->b*y + (*idx_it)->d) / (*idx_it)->c;
					z2r = -((*idx_it)->a*rp + (*idx_it)->b*y + (*idx_it)->d) / (*idx_it)->c;
					if (z1l < z2l || z1r < z2r)
					{
						tmp_color = (*idx_it)->color;
						a = (*idx_it)->a;
						b = (*idx_it)->b;
						c = (*idx_it)->c;
						z1l = z2l;
						z1r = z2r;
					}
				}
				if (idx.size() > 0)
				{
					double fact = dot(light, eye, a, b, c);
					//double fact = 0.2;
					tmp_color.r = int(1.5*fact*tmp_color.r);
					tmp_color.g = int(1.5*fact*tmp_color.g);
					tmp_color.b = int(1.5*fact*tmp_color.b);

					//tmp_colorv_ector.push_back(tmp_color);
					for (int x = int(lp + 0.5); x < int(rp + 0.5); x++) {
						//color[x] = tmp_color;
						image[y][x][0] = tmp_color.r;
						image[y][x][1] = tmp_color.g;
						image[y][x][2] = tmp_color.b;
					}
				}
				//tmp_colorv_ector.push_back(tmp_color);
				//for (int x = int(lp + 0.5); x < int(rp + 0.5); x++) {
				//	//color[x] = tmp_color;
				//	image[y][x][0] = tmp_color.r;
				//	image[y][x][1] = tmp_color.g;
				//	image[y][x][2] = tmp_color.b;
				//}
				lp = rp;
			}

			/*if (idx.size() > 0)
			{
				double fact = dot(light, eye, a, b, c);
				tmp_color.r = int(1.5*fact*tmp_color.r);
				tmp_color.g = int(1.5*fact*tmp_color.g);
				tmp_color.b = int(1.5*fact*tmp_color.b);
			}*/
			//for (int x = int(left->x + 0.5); x < int(right->x + 0.5); x++) {
			//	//color[x] = tmp_color;
			//	image[y][x][0] = tmp_color.r;
			//	image[y][x][1] = tmp_color.g;
			//	image[y][x][2] = tmp_color.b;
			//}
			// update dy
			left->dy--;
			// update x
			left->x += left->dx;
			//update z = z + dzx * dx + dzy
			left->z += left->dzx* left->dx + left->dzy;


			if (j == (aet.size() - 2))
			{
				right->dy--;
				right->x += right->dx;
				right->z += right->dzx* right->dx + right->dzy;
			}
			flag = true;
			//right++;
		}


		//if (flag) {
		//	// if changed please shadering
		//	for (int ii = 0; ii < WIDTH; ii++) {

		//		image[y][ii][0] = color[ii].r;
		//		image[y][ii][1] = color[ii].g;
		//		image[y][ii][2] = color[ii].b;
		//	}
		//}
	}

	// free the memory
	/*point.clear();
	for (int i = 0; i < HEIGHT; i++)
	{
		if (poly[i].size() > 0)
		{
			for (vector <Poly*>::const_iterator it = poly[i].begin(); it != poly[i].end(); ++it) {
				delete *it;
			}
			poly[i].clear();
		}
	}
	ape.clear();*/
}

void rotate(double thetax, double thetay)
{
	for (vector <Point_t> ::iterator it = point.begin(); it != point.end(); ++it) {
		double x, y, z;
		x = it->x;
		y = it->y;
		z = it->z;
		it->x = x*cos(thetay)-z*sin(thetay);
		it->z = x*sin(thetay) + z*cos(thetay);
		x = it->x;
		y = it->y;
		z = it->z;
		it->y = y*cos(thetax) - z*sin(thetax);
		it->z = y*sin(thetax) + z*cos(thetax);
	}
	for (int i = 0; i < MAX_HEIGHT; i++)
	{
		line[i].clear();
	}
	int lineidx = 0;
	for (int i = 0; i < MAX_HEIGHT;i++)
		for (vector <Poly*> ::iterator it = poly[i].begin(); it != poly[i].end(); ++it) 
		{
			int x, y, z;
			/*x = (*it)->a;
			y = (*it)->b;
			z = (*it)->c;
			(*it)->a -= WIDTH / 2;
			(*it)->b -= HEIGHT / 2;
			(*it)->a = int(x*cos(thetay) - z*sin(thetay) + 0.5);
			(*it)->b = int(y*cos(thetax) - z*sin(thetax) + 0.5);
			(*it)->c = int(x*sin(thetay) + z*cos(thetay) + y*sin(thetax) + z*cos(thetax) + 0.5);
			(*it)->a += WIDTH / 2;
			(*it)->b += HEIGHT / 2;*/
			int s, v, n;
			s = (*it)->point[0];
			v = (*it)->point[1];
			n = (*it)->point[2];
			(*it)->a = (point[v].y - point[s].y) * (point[n].z - point[s].z)
				- (point[v].z - point[s].z) * (point[n].y - point[s].y);
			(*it)->b = (point[v].z - point[s].z) * (point[n].x - point[s].x)
				- (point[v].x - point[s].x) * (point[n].z - point[s].z);
			(*it)->c = (point[v].x - point[s].x) * (point[n].y - point[s].y)
				- (point[v].y - point[s].y) * (point[n].x - point[s].x);
			//(*it)->d = -((*it)->a * point[s].x + (*it)->b * point[s].y + (*it)->c * point[s].z);

			(*it)->a = -max_scale*max_scale*(*it)->a;
			(*it)->b = max_scale*max_scale*(*it)->b;
			(*it)->c = -max_scale*max_scale*(*it)->c;
			(*it)->d = -((*it)->a * point[s].x - (*it)->b * point[s].y + (*it)->c * point[s].z)*max_scale
				- (*it)->a*WIDTH / 2 - (*it)->b*HEIGHT / 2;
			
			v = (*it)->point[0];
			s = v;
			for (int j = 1; j < (*it)->point.size(); j++)
			{
				n= (*it)->point[j];
				addLine((*it), (*it)->id, lineidx, v, n);
				v = n;
			}
			addLine((*it), (*it)->id, lineidx, v, s);
		}
	/*for (int i = 0; i < MAX_HEIGHT; i++)
		for (vector <Line>::iterator it = line[i].begin(); it != line[i].end(); )
		{
			it->x = point[it->maxid].x;
			it->dy = point[it->maxid].y - point[it->minid].y;
			it->dx = point[it->minid].x - point[it->maxid].x;
			if (point[it->maxid].y != i)
			{
				line[int(point[it->maxid].y)].push_back(*it);
				it = line[i].erase(it);
			}
			else
				it++;
		}*/

	memset(image, 0, sizeof(image));

	draw();
}

void addLine(Poly *poly,int &poly_idx, int &line_idx, int idx1, int idx2)
{
	Line line_1;
	line_1.id = poly_idx;
	line_1.poly = poly;

	int max_idx, min_idx;
	if (point[idx1].y < point[idx2].y) {
		max_idx = idx2;
		min_idx = idx1;
	}
	else {
		max_idx = idx1;
		min_idx = idx2;
	}

	//line_1.x = point[max_idx].x;
	//line_1.dy = point[max_idx].y - point[min_idx].y;
	//line_1.dx = point[min_idx].x - point[max_idx].x;
	//line[ int(point[max_idx].y + 0.5)].push_back(line_1);


	line_1.x = int(point[min_idx].x*max_scale + WIDTH / 2 + 0.5);
	line_1.point1 = max_idx;
	line_1.point2 = min_idx;
	int ymax, ymin,xmax,xmin;
	xmax = int(point[max_idx].x*max_scale + WIDTH / 2 + 0.5);
	xmin = int(point[min_idx].x*max_scale + WIDTH / 2 + 0.5);
	ymax = HEIGHT - int(point[max_idx].y*max_scale + HEIGHT / 2 + 0.5);
	ymin = HEIGHT - int(point[min_idx].y*max_scale + HEIGHT / 2 + 0.5);
	line_1.dy = ymin-ymax;
	//line_1.dx = int((- point[min_idx].x + point[max_idx].x)*max_scale + 0.5);
	line_1.dx = xmax-xmin;
	line[HEIGHT - int(point[min_idx].y*max_scale + HEIGHT / 2 + 0.5)].push_back(line_1);
	line_idx++;
}

bool cmp(const Active_Line &lhs, const Active_Line &rhs)
{
	if (int(lhs.x+0.5) < int(rhs.x+0.5)) {
	return true;
	}
	else if (int(lhs.x+0.5) == int(rhs.x+0.5)){
	if (int(lhs.dx+0.5) < int(rhs.dx+0.5))
	return true;
	}
	return false;
}

double dot(double light[3], double eye[3], double a, double b, double c)
{
	double f = (eye[0] - light[0]) *a + (eye[1] - light[1]) *b + (eye[2] - light[3]) *c;
	f = f / sqrt((eye[0] - light[0])*(eye[0] - light[0]) + (eye[1] - light[1])*(eye[1] - light[1]) + (eye[2] - light[2])*(eye[2] - light[2]));
	f = f / sqrt(a*a+b*b+c*c);
	return f;
}

bool IsRemove(Poly* &obj1, int id)
{
	return obj1->id == id;
}
