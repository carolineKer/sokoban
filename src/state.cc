#include "state.h"
#include <iostream>

State::State(const std::vector<Point>& boxes, const Point& player):boxes(boxes), player(player)
{
}

void State::display()
{
	std::vector<Point>::iterator it;
	for (it = boxes.begin(); it!=boxes.end(); it++)
	{
		std::cout << it->i << " " << it->j << std::endl;	
	}
}
