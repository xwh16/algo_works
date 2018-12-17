#define _CRT_SECURE_NO_WARNINGS

#include <cstdlib>
#include <cstdio>
#include <cmath>

#include <vector>
#include <limits>
#include <iostream>
#include <algorithm>

using namespace std;

struct Dots {
	double x;	// mo*******er
	double y;
};

unsigned int N;
vector<Dots> points;
vector<Dots> pointsY;

struct xSortLessThan
{
	inline bool operator() (const Dots &p1, const Dots &p2) {
		return p1.x < p2.x;
	}
};

struct ySortLessThan
{
	inline bool operator() (const Dots &p1, const Dots &p2) {
		return p1.y < p2.y;
	}
};

double dist(const Dots &p1, const Dots &p2) {
	return sqrt(pow(p1.x - p2.x, 2) + pow(p1.y - p2.y, 2));
}

double closetpair(unsigned int lo, unsigned int hi) {
	// [lo, hi]!!!
	if (lo == hi) {
		return numeric_limits<double>::max();
	}
	if (lo + 1 == hi) {
		sort(pointsY.begin() + lo, pointsY.begin() + hi + 1, ySortLessThan());
		return dist(points[lo], points[hi]);
	}

	unsigned int i, j, k;
	double dmin, d1, d2;
	d1 = closetpair(lo, (hi + lo) / 2 - 1);
	d2 = closetpair((hi + lo) / 2, hi);
	dmin = (d1 > d2) ? d2 : d1;
	// mergesort 
	// ...
	inplace_merge(
		pointsY.begin() + lo, 
		pointsY.begin() + (hi + lo) / 2, 
		pointsY.begin() + hi + 1, 
		ySortLessThan()
	);
	// scan points in 2-delta strip by y-order
	double ld, rd;
	ld = points[(hi + lo) / 2 - 1].x - dmin;
	rd = points[(hi + lo) / 2].x + dmin;
	for (i = lo; i <= hi; i++) {
		// exclude those points that are not in the strip
		if (pointsY[i].x <= ld || pointsY[i].x >= rd) {
			continue;
		}
		else {
			for (j = 1, k = 1; j <= 11 && k <= hi - i; k++) {
				if (pointsY[i + k].x <= ld || pointsY[i + k].x >= rd) {
					continue;
				}
				j++;
				double d = dist(pointsY[i], pointsY[i + k]);
				if (d < dmin) {
					dmin = d;
				}
			}
		}
	}
	return dmin;
}

int main() {
	unsigned int i;
	double x, y;
	scanf("%ud", &N);
	if (N == 0)
		return 0;
	for (i = 0; i < N; i++) {
		scanf("%lf %lf", &x, &y);
		points.push_back(Dots{ x, y });
	}
	sort(points.begin(), points.end(), xSortLessThan());
	pointsY.resize(N);
	copy(points.begin(), points.end(), pointsY.begin());
	printf("%.2lf", closetpair(0, N-1));
	return 0;
}
