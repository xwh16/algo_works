// closetpair.cpp : 定义控制台应用程序的入口点。
#define _CRT_SECURE_NO_WARNINGS

#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<time.h>
#include<float.h>

struct point {
	double x;
	double y;
};

int xComp(const void *p1, const void *p2) {
	return ((const struct point *)p1)->x < ((const struct point *)p2)->x;
}

int yComp(const void *p1, const void *p2) {
	return ((const struct point *)p1)->y < ((const struct point *)p2)->y;
}

void quicksort_x(struct point *datas, int l, int r) {
	if (l >= r)
		return;
	int pivot_index = rand() % (r - l + 1) + l;
	double pivot_x = datas[pivot_index].x;
	double pivot_y = datas[pivot_index].y;
	datas[pivot_index].x = datas[l].x;
	datas[pivot_index].y = datas[l].y;
	pivot_index = 0;

	int low = l;
	int high = r;


	while (low<high) {
		while (low<high && datas[high].x< pivot_x)
			high -= 1;
		if (low == high)
			break;
		datas[low].x = datas[high].x;
		datas[low].y = datas[high].y;

		while (low<high && datas[low].x >= pivot_x)
			low += 1;
		if (low == high)
			break;
		datas[high].x = datas[low].x;
		datas[high].y = datas[low].y;
	}

	if (low == high) {
		datas[high].x = pivot_x;
		datas[high].y = pivot_y;
		pivot_index = high;
	}
	else return;

	quicksort_x(datas, l, pivot_index - 1);
	quicksort_x(datas, pivot_index + 1, r);
	return;
}

void quicksort_y(struct point * datas, int l, int r) {
	if (l >= r)
		return;
	int pivot_index = rand() % (r - l + 1) + l;
	double pivot_x = datas[pivot_index].x;
	double pivot_y = datas[pivot_index].y;
	datas[pivot_index].x = datas[l].x;
	datas[pivot_index].y = datas[l].y;
	pivot_index = 0;

	int low = l;
	int high = r;


	while (low<high) {
		while (low<high && datas[high].y< pivot_y)
			high -= 1;
		if (low == high)
			break;
		datas[low].x = datas[high].x;
		datas[low].y = datas[high].y;

		while (low<high && datas[low].y >= pivot_y)
			low += 1;
		if (low == high)
			break;
		datas[high].x = datas[low].x;
		datas[high].y = datas[low].y;
	}

	if (low == high) {
		datas[high].x = pivot_x;
		datas[high].y = pivot_y;
		pivot_index = high;
	}
	else return;

	quicksort_y(datas, l, pivot_index - 1);
	quicksort_y(datas, pivot_index + 1, r);
	return;
}

double closestpair(struct point * datas, int l, int r) {
	// [l, r]
	double x1, x2, y1, y2;
	if (r <= l) return DBL_MAX;
	if (r - l == 1) {
		x1 = datas[l].x;
		y1 = datas[l].y;
		x2 = datas[r].x;
		y2 = datas[r].y;
		return sqrt((x1 - x2)*(x1 - x2) + (y1 - y2)*(y1 - y2));
	}
	int coordinate_index = (l + r) / 2;
	double coordinate_x = datas[coordinate_index].x;
	double dest1 = closestpair(datas, l, coordinate_index);
	double dest2 = closestpair(datas, coordinate_index + 1, r);

	double dest = dest1 < dest2 ? dest1 : dest2;

	int i = coordinate_index - 1;
	while (i >= l && datas[i].x >= coordinate_x - 2 * dest) i -= 1;
	int low = i + 1;

	i = coordinate_index + 1;
	while (i <= r && datas[i].x <= coordinate_x + 2 * dest) i += 1;
	int high = i - 1;

	struct point * temp = (struct point *)malloc((high - low + 1) * sizeof(struct point));

	for (i = low; i <= high; i++) {
		temp[i - low].x = datas[i].x;
		temp[i - low].y = datas[i].y;
	}
	qsort(temp, high - low, sizeof(struct point), yComp);
	//quicksort_y(temp, 0, high - low);
	for (i = 0; i < high - low; i++) {
		x1 = temp[i].x;
		y1 = temp[i].y;
		for (int j = i + 1; j < high - low + 1 && j <= i + 11; j++) {
			x2 = temp[j].x;
			y2 = temp[j].y;
			if (fabs(y2 - y1) > dest)
				break;
			double d = sqrt((x1 - x2)*(x1 - x2) + (y1 - y2)*(y1 - y2));
			if (d < dest)
				dest = d;
		}
	}
	free(temp);
	return dest;
}


int main()
{
	int n;
	time_t c_start, c_end;
	//c_start = clock();

	scanf("%d", &n);

	struct point * datas = (struct point *)malloc(n * sizeof(struct point));

	for (int i = 0; i < n; i++) {
		scanf("%lf%lf", &datas[i].x, &datas[i].y);
	}

	srand((unsigned)time(NULL));

	//quicksort_x(datas, 0, n - 1);
	qsort(datas, n - 1, sizeof(struct point), xComp);
	printf("%.2lf", closestpair(datas, 0, n - 1));
	//c_end = clock();
	//printf("The pause used %f ms by clock()\n", difftime(c_end, c_start));
}