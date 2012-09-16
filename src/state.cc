#include "state.h"
#include "ground.h"
#include <iostream>

State::State(const std::set<Point>& boxes, const Point& player):boxes(boxes), player(player)
{
	const Point ground_size = ground.getSize();
	reachable_area = new bool*[ground_size.i];
	for (int i = 0; i<ground_size.i; i++)
	{
		reachable_area[i] = new bool[ground_size.j];
		for (int j = 0; j< ground_size.j ; j++)
		{
			reachable_area[i][j] = false;
		}
	}
	max_pos.i = -1;
	max_pos.j = -1;
	compute_reachable_area(player);
}

void State::display()
{
	std::set<Point>::iterator it;
	for (it = boxes.begin(); it!=boxes.end(); it++)
	{
		std::cout << it->i << " " << it->j << std::endl;	
	}

	std::cout << ground.getSize().i << std::endl;
	std::cout << ground.getSize().j << std::endl;

	for (int i = 0; i< ground.getSize().i ; i++)
	{
		for (int j = 0; j< ground.getSize().j ; j++)
		{
			if (reachable_area[i][j])
				std::cout << "O";
			else
				std::cout << "X";
		}
		std::cout << std::endl;
	}
	std::cout << "end of loop " << std::endl;
}

State::~State()
{
	const Point ground_size = ground.getSize();
	for (int i = 0; i<ground_size.i; i++)
	{
		delete[] reachable_area[i];
	}
}

void State::compute_reachable_area(const Point& from)
{
	reachable_area[from.i][from.j] = true;
	if (max_pos.i <= from.i)
	{
		max_pos.i = from.i;
			if (max_pos.j < from.j)
				max_pos.j = from.j;
	}

	//Check the four directions
	Point next;

	Point dir[4] = {
		Point(-1,0),
		Point(0,-1),
		Point(1,0),
		Point(0,1)
	};

	for (int i = 0; i<4; i++)
	{
		Point next = from +dir[i];
		if (!ground.isOut(next) && !ground.isBlocked(next) && 
			!reachable_area[next.i][next.j] &&
			boxes.find(next) == boxes.end()
		   )
		{
			compute_reachable_area(next);
		}
	}
}

