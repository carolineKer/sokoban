#ifndef __STATE_H
#define __STATE_H

#include "point.h"
#include <vector>


class State
{
	public:
		State(const std::vector<Point>& boxes, const Point& player);
		void display();
	
	private:
		std::vector<Point> boxes;
		Point player;
};
#endif
