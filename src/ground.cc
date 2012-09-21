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
		else
		{
			//Or the player can be on an empty case
			player.j = col_to_save->find(PLAYER);
			if (player.j != string::npos)
			{
				(*col_to_save)[player.j] = EMPTY;
				player.i = __ground.size();
			}
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
		//std::cout <<"Found a box" << std::endl;
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

Point Ground::getPlayer()
{
    return player;
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
	
Point Ground::getInitialPosPlayer()
{
	return player;
}

vector<Point> Ground::getGoals() {
	return goals;
}

bool Ground::isDeadend(const Point& here)
{
    
    bool isDeadend = false;
    int i = 0;
    while(i < deadends.size() && isDeadend == false)
    {
        if (here == deadends[i]) isDeadend = true;
        i++;
    }
    
    return isDeadend;
}

bool Ground::isPassable(const Point& here, const Point& last)
{
    
    bool passable = (!isOut(here) && !isBlocked(here) && boxes.find(here) == boxes.end() && here != last && !isDeadend(here));
    
	return passable;
}

int Ground::calcManhattDist(const Point& a, const Point& b)
{
    
    int distY = a.i-b.i;
    int distX = a.j-b.j;
    
    if(distX < 0) distX = distX*-1;
    if(distY < 0) distY = distY*-1;
    
    return (distX+distY);
    
}

Point Ground::getNextCell(const Point& from, const Point& to, const Point& last)
{
    
    Point next = from;
    
    int distY = from.i-to.i;
    int distX = from.j-to.j;
    
    int passable[4];
    
    for (int i = 0; i<4; i++) passable[i] = ground.isPassable(from + DIR[i], last);
    
    if     (distX < 0 && passable[3]) next = from + RIGHT;
    else if(distX > 0 && passable[1]) next = from + LEFT;
    else if(distY < 0 && passable[2]) next = from + DOWN;
    else if(distY > 0 && passable[0]) next = from + UP;
    else{
        for (int i = 0; i < 4; i++) {
            if(passable[i] == true)
            {
                next = from+DIR[i];
                break;
            }
        }
    }
    
    
    return next;
}

string Ground::addDirectionLetter(const Point& from, const Point& next)
{
    
    string letter;
    
    if(next == (from+UP)) letter = "U";
    if(next == (from+DOWN)) letter = "D";
    if(next == (from+RIGHT)) letter = "R";
    if(next == (from+LEFT)) letter = "L";
    
    return letter;
}

string Ground::findPath(const Point& from, const Point& to)
{
    std::cout << "Start finding path!" << std::endl;
    string path;
    tempPath.clear();
    deadends.clear();
    tempPath.push_back(from);
    explorePath(from, to);
    
    std::cout << "Found path with " << tempPath.size() << " steps!" << std::endl;
    
    std::cout << "Start generating solution string!" << std::endl;
    
    for(int i = 0; i < tempPath.size(); i++) path += addDirectionLetter(tempPath[i],tempPath[i+1]);
    
    return path;
}

void Ground::explorePath(const Point& from, const Point& to)
{
    //std::cout << "Last place:  " << tempPath[tempPath.size()-2].i << " " << tempPath[tempPath.size()-2].j << std::endl;
    //std::cout << "Current place:  " << from.i << " " << from.j << std::endl;
    //std::cout << "Function start: Path size:  " << tempPath.size() << " steps!" << std::endl;
    
    
    if(from != to)
    {
        Point next;
        if(tempPath.size() >= 2) next = getNextCell(from, to, tempPath[tempPath.size()-2]);
        else{
            Point dummy;
            next = getNextCell(from, to, dummy);
        }
        
        if(next != from)
        {
            //std::cout << "Found next cell!" << std::endl;
            tempPath.push_back(next);
            //std::cout << "Function End: Path size:  " << tempPath.size() << " steps!" << std::endl;
            //std::cout << "Next place:  " << next.i << " " << next.j << std::endl;
            explorePath(next, to);
            
        }
        else
        {
            //std::cout << "This is a deadend!" << std::endl;
            deadends.push_back(from);
            tempPath.pop_back();
            //std::cout << "Function End: Path size:  " << tempPath.size() << " steps!" << std::endl;
            next = tempPath[tempPath.size()-1];
            //std::cout << "Back to place:  " << next.i << " " << next.j << std::endl;
            explorePath(next, to);
        }
    }
    else std::cout << "Reached aim!" << std::endl;
}

//Well not really important...
Ground::~Ground() {
	vector<string *>::iterator it;
	for (it = __ground.begin(); it != __ground.end(); it++)
		delete *it;
}
