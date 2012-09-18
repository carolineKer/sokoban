#include <iostream>
#include <cassert>
#include "ground.h" 

using std::cout;
using std::endl;

Ground ground;

Ground::Ground()
{
	player.i = -1;
	player.j = -1;
	ground_size.i = 0;
	ground_size.j = 0;
}

//Add a row to the Ground (rows must be added from top to bottom)
bool Ground::addRow(const string& col) {

	string * col_to_save = new string(col);
	int new_size = (col.size());
	if (ground_size.j != 0)
	{
		assert(new_size == ground_size.j);
	}
	else
	{
		ground_size.j = new_size;
	}

	///////////////////////////////////////////
	//		 PLAYER							 //
	///////////////////////////////////////////
	//If we haven't found the player yet, try to find him
	if (player.i == -1)
	{
		bool player_is_on_goal = false;

		//Try to find the player
		//(the player can be on a goal
		player.j = col_to_save->find(P_ON_GOAL);

		if (player.j != string::npos)
		{
			(*col_to_save)[player.j] = GOAL;
			player.i = __ground.size();
		}
		
		//Or the player can be on an empty case
		player.j = col_to_save->find(PLAYER);
		if (player.j != string::npos)
		{
			player.i = __ground.size();
			(*col_to_save)[player.j] = EMPTY;
		}
	}
	
	///////////////////////////////////////////
	//		 BOXES							 //
	///////////////////////////////////////////
	//Are there boxes in this row ?
	Point tmp;
	tmp.j = -1;
	while ((tmp.j = col_to_save->find(BOX, tmp.j+1))!=string::npos)
	{
		std::cout << "Found a box" << std::endl;
		tmp.i = __ground.size();
		boxes.insert(tmp);	
		(*col_to_save)[tmp.j] = EMPTY;
	}

	tmp.j = -1;
	while ((tmp.j = col_to_save->find(B_ON_GOAL, tmp.j+1))!=string::npos)
	{
		tmp.i = __ground.size();
		boxes.insert(tmp);	
		(*col_to_save)[tmp.j] = GOAL;
	}
	
	///////////////////////////////////////////
	//		 GOAL							 //
	///////////////////////////////////////////
	tmp.j = -1;
	while ((tmp.j = col_to_save->find(GOAL, tmp.j+1))!=string::npos)
	{
		tmp.i = __ground.size();
		goals.push_back(tmp);	
	}

	__ground.push_back(col_to_save);
	ground_size.i++;
	return false;
}

State Ground::getInitialState()
{
	return State(boxes, player);
}

void Ground::display()
{
	std::vector<string *>::iterator it;
	std::cout << "------ DISPLAY MAP -------" << std::endl;
	for (it = __ground.begin(); it != __ground.end(); it++)
	{
		std::cout << *(*it) << std::endl;
	}

	std::cout << std::endl;
	std::cout << "Goals :" << std::endl;
	std::vector<Point>::iterator it_g;
	for (it_g = goals.begin(); it_g!=goals.end(); it_g++)
	{
		std::cout << it_g->i << " " << it_g->j << std::endl;
	}

}

bool Ground::isOut(const Point& here)
{
	bool out= (here.i < 0 || here.j <0 || here.i > ground_size.i-1 || here.j > __ground[here.i]->size()-1);
	return out;
}

bool Ground::isBlocked(const Point& here)
{
	char type = (*(__ground[here.i]))[here.j];
	return (type == WALL);
}

char Ground::operator()(const Point& here)
{
	return __ground[here.i]->at(here.j);
}
	

//Well not really important...
Ground::~Ground() {
	vector<string *>::iterator it;
	for (it = __ground.begin(); it != __ground.end(); it++)
		delete *it;
}
