#include "state.h"
#include "ground.h"
#include "deadlock.h"
#include <iostream>
#include <cassert>
#include <algorithm>


//Contains the states wich must be expanded (because we are doing a BFS)
std::priority_queue< State*, std::vector<State*>, State::Compare > State::to_expand;

//Contains all the already visited states
std::list<State*> State::all_states;

/* Constructor for the initial state */
State::State(const std::set<Point>& boxes, const Point& player):boxes(boxes),  
	parent(NULL), 
	is_in_all_list(true),
	is_in_expand_list(true)
						
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

	std::cout << "Player " << std::endl;
	std::cout << player.i << " " << player.j << " " << std::endl;

	compute_reachable_area(player);
	this->add_to_l();
	all_states.push_back(this);
	p_in_all_list = all_states.end() ;
	p_in_all_list--;
}

State::State(State& prev_state, const Point& moved_box, int dir):boxes(prev_state.boxes), parent(&prev_state), dir(dir), is_in_all_list(false), is_in_expand_list(false), moved_box(moved_box)
{
	//std::cout << "Build a state move box " << moved_box.i << " " << moved_box.j
	//	<< " direction :" << -DIR[dir].i << " " << -DIR[dir].j << std::endl;
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

	std::set<Point>::iterator it;
	it = this->boxes.find(moved_box);
	assert(it != this->boxes.end());
	this->boxes.erase(it);
	this->boxes.insert(moved_box-DIR[dir]);

	compute_reachable_area(moved_box+DIR[dir]);
}

void State::display()
{
	Point ground_size = ground.getSize();
	for (int i = 0; i< ground_size.i; i++)
	{
		for (int j =0; j<ground_size.j; j++)
		{
			std::set<Point>::iterator it;
			it = boxes.find(Point(i,j));
			bool is_box = (it!=boxes.end());
			switch (ground(Point(i,j)))
			{
				case WALL:
					std::cout << WALL;
					break;
				case EMPTY:
					if (is_box)
						std::cout << BOX;
					else
						std::cout << EMPTY;
					break;
				case GOAL:
					if (is_box)
						std::cout << B_ON_GOAL;
					else
						std::cout << GOAL;
					break;
			}
		}
		std::cout << std::endl;
	}
}


void State::display_reachable_area()
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
}


/* ! *\ The destruction is incorrect  */
State::~State()
{
	if (is_in_expand_list)
	{
		std::cout << "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!" << std::endl;
		std::cout << "/*!*\\ Destroying a state which is registered in expand_list" << std::endl;
		std::cout << "...Will segfault later... " << std::endl;
		std::cout << "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!" << std::endl;
	}
	const Point ground_size = ground.getSize();
	for (int i = 0; i<ground_size.i; i++)
	{
		delete[] reachable_area[i];
	}
	if (is_in_all_list)
		all_states.erase(p_in_all_list);
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

	for (int i = 0; i<4; i++)
	{
		Point next = from +DIR[i];
		if (!ground.isOut(next) && !ground.isBlocked(next) && 
			!reachable_area[next.i][next.j] &&
			boxes.find(next) == boxes.end()
		   )
		{
			compute_reachable_area(next);
		}
	}
}

State* State::expand()
{
	std::set<Point>::iterator it_b;
	//For each box
	for (it_b = boxes.begin(); it_b != boxes.end(); it_b++)
	{
		//For each direction
		for (int i = 0; i<4; i++)
		{
			Point p = (*it_b)+DIR[i];
			if (reachable_area[p.i][p.j])
			{
				std::set<Point>::iterator it;
				it = boxes.find(*it_b-DIR[i]);
				if ((ground(*it_b-DIR[i])==GOAL || ground(*it_b-DIR[i])==EMPTY) && it == boxes.end())
				{
					//std::cout << "Build new state" << std::endl;
					State * s = new State(*this, *it_b, i);
					//std::cout << "New state built" << std::endl;
					std::list<State*>::iterator it_states;
					bool repeated_state = false;
					//std::cout << "Compare with all states" << std::endl;
					for (it_states=all_states.begin(); it_states!=all_states.end(); it_states++)
					{
						if (**it_states == *s)
						{
							repeated_state = true;
							break;
						}
							
					}

					// deadlock check:
					Point inv_dir(-DIR[i].i, -DIR[i].j);
					bool hasDeadlock = isDeadlock(*it_b-DIR[i],inv_dir,boxes);                                                                                
					if (repeated_state || hasDeadlock )
					{
						//std::cout << "Repeated state" << std::endl;
						continue;
					}
					else
					{
						//std::cout << "XXX Interesting state Add to queue" << std::endl;
					}
					all_states.push_back(s);
					s->p_in_all_list = all_states.end() ;
					s->p_in_all_list--;
					s->is_in_all_list = true;
					s->add_to_l();

					next.push_back(s);
					if (s->isFinal())
						return s;
				}
			}
		}
	}
	return NULL;
	//TODO Maybe put deletion of reachable_area here rather than in the destructor
}

bool State::isFinal()
{
	std::set<Point>::iterator it_b;
	bool final = true;
	
	for (it_b = boxes.begin(); it_b != boxes.end(); it_b++)
	{
		if (ground(*it_b)!=GOAL)
		{
			final = false;
			break;
		}
	}
	return final;
}

State* State::nextStateToExpand() 
{
	State * to_return = to_expand.top();
	to_expand.pop();
	to_return->is_in_expand_list = false;
	return  to_return;
}

void State::add_to_l()
{
	is_in_expand_list = true;
	to_expand.push(this);
}

bool State::operator==(const State& a)
{
	if (this->max_pos != a.max_pos)
	{
		return false;
	}

	//TODO is it optimzed (I guess not)
	std::set<Point> intersect;

	std::set<Point>::iterator it;

	/*
	for (it = this->boxes.begin(); it !=this->boxes.end(); it++)
	{
		std::cout << "boxes " << (*it).i << " " << (*it).j << std::endl;
		std::cout << "boxes << (*it).i << " " << (*it).j << std::endl;
	}

	for (it = a.boxes.begin(); it !=a.boxes.end(); it++)
	{
		std::cout << "a " << (*it).i << " " << (*it).j << std::endl;
	}
	*/
	set_intersection(this->boxes.begin(), this->boxes.end(), a.boxes.begin(), a.boxes.end(), std::inserter(intersect, intersect.end()));
	/*
	std::cout << "Intersect" << std::endl;
	std::cout << "size of the set " << intersect.size() << std::endl;
	std::cout << "Size of the box " << this->boxes.size() << std::endl;
	*/

	return (intersect.size() == this->boxes.size());
}

bool State::operator!=(const State& a)
{
	return (!(*this==a));
}

std::set<Point> State::getBoxes() {
	return boxes;
}

int State::euristic() {

	// Return a number corresponding to the similarity between the current state and the final state
	// Number computed with euclidian distance between boxes and goals

	int distance = 0;
	int min = 0;
	int indice = 0;
	std::set<Point>::const_iterator it_boxes(this->boxes.begin()), end(this->boxes.end());
	std::vector<Point> goals = ground.getGoals();

	for(;it_boxes != end; ++it_boxes) {
		min = -1;
		for(int i=0; i<goals.size();i++) {
			if((*it_boxes).distance(goals[i])<min || min<0) {
				min = (*it_boxes).distance(goals[i]);
				indice = i;
			}
		}
		distance += min;
		goals.erase(goals.begin()+indice);
	}

	return distance;
}

std::string State::findSolutionString(State * final_state, State * initial_state)
{
    std::string final_path;
    
	State * to_state = final_state;
	State * from_state = to_state->parent;
    
    while (from_state != NULL)
	{
		State * parent_state = from_state->getParentState();
		Point player_position;

		if (parent_state == NULL) // <=> (curr_state == initial_state)
		{
			player_position = ground.getPlayer();
		}
		else
		{
			player_position = from_state->moved_box;
		}

		Point moved_box = to_state->moved_box;
		int dir = to_state->dir;
        
		/*
        std::cout << "Start Point: " << start.i << " " << start.j << std::endl;
        std::cout << "Goal Point: " << goal.i << " " << goal.j << std::endl;
		*/
        
		//***********From one state to the next***************//
		std::cout << "STATE FROM " << std::endl;
		from_state->display();
		std::cout << "STATE TO " << std::endl;
		to_state->display();
		Point goal = moved_box + DIR[dir];
		std::cout << "*********TO Pushing position*****" << std::endl;
		std::cout << "Player " << player_position.i << " " << player_position.j << std::endl;
		std::cout <<" To pushing  " << goal.i << " " << goal.j << std::endl;
        std::string path_part = ground.findPath(player_position,goal, *from_state);
		std::cout << "Path " << path_part << std::endl;


		player_position = goal;
		goal = moved_box;
		std::cout << "*********PUSH box************" << std::endl;
		std::cout << "Try to find a path from " 
			<< player_position.i << " " << player_position.j << std::endl;
		std::cout <<" To :  " << goal.i << " " << goal.j << std::endl;
		path_part.append( ground.addDirectionLetter(player_position, moved_box));
		std::cout << "Path " << path_part << std::endl;
		//**************************************************//

		final_path.insert(0, path_part);
		to_state = from_state;
		from_state = parent_state;
	}
    
    return final_path;
    
}
