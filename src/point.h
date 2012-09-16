#ifndef __POINT_H__
#define __POINT_H__

class Point
{
	public:
		Point(): i(0),j(0) {};
		Point(int i, int j): i(i), j(j) { };
		int i;
		int j;
};

Point operator+(const Point& a, const Point& b);
bool operator<(const Point& a, const Point& b);

#endif /*__POINT_H */
