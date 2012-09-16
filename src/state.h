#ifndef __STATE_H
#define __STATE_H

#include "point.h"
#include <vector>
#include <set>


class State
{
	public:
		State(const std::set<Point>& boxes, const Point& player);
		~State();
		void display();
	
	private:
		std::set<Point> boxes;
		Point player;

		//Area that the player can reach
		bool ** reachable_area;
		Point max_pos;
		void compute_reachable_area(const Point& from);
};
#endif
