#include "point.h"

Point operator+(const Point& a, const Point& b)
{
	return Point(a.i+b.i, a.j+b.j);
};

bool operator<(const Point& a, const Point& b)
{
	if (a.i != b.i) return (a.i < b.i);
	else return (a.j < b.j);
};
