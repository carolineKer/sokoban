#include "state.h"
#include "ground.h"
#include "deadlock.h"
#include <iostream>
#include <cassert>
#include <algorithm>
#include "tunnel.h"


//Contains the states wich must be expanded (because we are doing a BFS)
std::priority_queue< State*, std::vector<State*>, State::Compare > State::to_expand;

//Contains all the already visited state
std::unordered_set<State*, State::Hash_X, State::Equality> State::all_states;

/* Constructor for the initial state */
State::State(const std::set<Point>& boxes, const Point& player):boxes(boxes),  
	parent(NULL), 
	is_in_all_list(true),
	is_in_expand_list(true),
	depth(0),
	tunnel(NULL)
						
{
	const Point ground_size = ground.getSize();
	max_pos.i = -1;
	max_pos.j = -1;

	std::cout << "Player " << std::endl;
	std::cout << player.i << " " << player.j << " " << std::endl;

	allocate_reachable_area();
	compute_reachable_area(player);
	this->add_to_l();
	all_states.insert(this);
}

State::State(State& prev_state, const Point& moved_box, int dir, const Tunnel* tunnel):
	boxes(prev_state.boxes), 
	parent(&prev_state), 
	dir(dir), 
	is_in_all_list(false), 
	is_in_expand_list(false), 
	moved_box(moved_box), 
	tunnel(tunnel),
	reachable_area(NULL)
{
	//std::cout << "Build a state move box " << moved_box.i << " " << moved_box.j
	//	<< " direction :" << -DIR[dir].i << " " << -DIR[dir].j << std::endl;
	
	depth = parent->depth+1;
 	const Point ground_size = ground.getSize();
	max_pos.i = -1;
	max_pos.j = -1;

	std::set<Point>::iterator it;
	it = this->boxes.find(moved_box);
	assert(it != this->boxes.end());
	this->boxes.erase(it);

	if (tunnel == NULL)
		this->boxes.insert(moved_box-DIR[dir]);
	else
	{
		if (moved_box == tunnel->b)
		{
			this->boxes.insert(tunnel->a);
		}
		else 
		{
			this->boxes.insert(tunnel->b);
		}
	}

	allocate_reachable_area();
	compute_reachable_area(moved_box+DIR[dir]);
	free_reachable_area();
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
				case GOAL:
					if (is_box)
						std::cout << B_ON_GOAL;
					else
						std::cout << GOAL;
					break;
				default:
					if (is_box)
						std::cout << BOX;
					else
						std::cout << EMPTY;
					break;
			}
		}
		std::cout << std::endl;
	}
}


void State::display_reachable_area()
{
	if (reachable_area == NULL)
		return;
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

void State::free_reachable_area()
{
	if (reachable_area == NULL)
		return;

	const Point ground_size = ground.getSize();
	for (int i = 0; i<ground_size.i; i++)
	{
		delete[] reachable_area[i];
	}
	delete[] reachable_area;
	reachable_area = NULL;
}



/* ! *\ The destruction is incorrect  */
State::~State()
{
	if (is_in_expand_list || is_in_all_list)
	{
		std::cout << "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!" << std::endl;
		std::cout << "/*!*\\ Destroying a state which is registered in expand_list" << std::endl;
		std::cout << "...Will segfault later... " << std::endl;
		std::cout << "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!" << std::endl;
	}
}

void State::allocate_reachable_area()
{
	assert(reachable_area == NULL);
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
	if (reachable_area == NULL)
	{
		allocate_reachable_area();
		compute_reachable_area(moved_box+DIR[dir]);
	}
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

				//If the box is on a X or a D, it can be push to a X or D case (begining of the game):
				//######
				//#@ $
				//######
				//
				//Tunnel:
				//1/ Enter in the tunnel
				//--> ground(*it_b-DIR[i]) == 'A' or 'B'
				//We must check that it is not a deadlock (ie there is no box on the other tunnel entry)
				//###############--> this push should not be allowed, it is a deadlock
				// -->$A	   $ 
				//     #########
				//2/ Push in the tunnel
				//--> ground(*it_b) == 'A' or 'B' and push to 'A' 'B' or 'C'
				//If the box with on 'A' it must be pushed to 'B', if it was on 'B' it must be pushed to 'A'
				if (
						(
						(
							ground(*it_b-DIR[i])==GOAL || ground(*it_b-DIR[i])==EMPTY || 
							ground(*it_b-DIR[i])=='A' || ground(*it_b-DIR[i])=='B' || ground(*it_b-DIR[i]) == 'C'
						) ||
						(
						 (ground(*it_b) == 'X' || ground(*it_b)== 'D') && 
						 ground(*it_b-DIR[i]) != WALL
						))
						&& it == boxes.end())
				{

					State * s;
					/////////////////////////////
					// Push to a tunnel position
					/////////////////////////////

					if (ground(*it_b-DIR[i]) == 'A' || ground(*it_b-DIR[i]) == 'B' || ground(*it_b-DIR[i]) == 'C')
					{
						std::list<Tunnel>::iterator it_tunnel;
						if (ground(*it_b) == 'A')  //Was already inside the tunnel
						{
							std::list<Tunnel>::const_iterator it_tunnel;
							for (it_tunnel = Tunnel::tunnels.begin(); it_tunnel != Tunnel::tunnels.end(); it_tunnel++)
							{
								if (it_tunnel->a == *it_b) 
									break;
							}

							//it_tunnel = Tunnel::tunnels.find(Tunnel(*it_b, Point(-42,-42)));
							assert(it_tunnel != Tunnel::tunnels.end());
							const Tunnel& t = *it_tunnel;
							s = new State(*this, *it_b, i , &t);
						}
						else if (ground(*it_b) == 'B') //Was already inside the tunnel
						{
							std::list<Tunnel>::const_iterator it_tunnel;
							for (it_tunnel = Tunnel::tunnels.begin(); it_tunnel != Tunnel::tunnels.end(); it_tunnel++)
							{
								if (it_tunnel->b == *it_b) 
								{
									break;
								}
							}
							//it_tunnel = Tunnel::tunnels.find(Tunnel(Point(-24,-24),*it_b));
							assert(it_tunnel != Tunnel::tunnels.end());
							const Tunnel& t = *it_tunnel;
							s = new State(*this, *it_b, i , &t);
						}
						else //Entering the tunnel --> check tunnel deadlocks
						{
							assert (ground(*it_b-DIR[i]) != 'C');
							std::set<Point>::iterator it2;

							//Find tunnel
							std::list<Tunnel>::iterator it_tunnel;
							if (ground(*it_b-DIR[i]) == 'A')
							{
								for (it_tunnel = Tunnel::tunnels.begin(); it_tunnel != Tunnel::tunnels.end(); it_tunnel++)
								{
									if (it_tunnel->a == (*it_b-DIR[i]))
										break;
								}
								assert(it_tunnel != Tunnel::tunnels.end());
							}
							else if (ground(*it_b-DIR[i]) == 'B') //Was already inside the tunnel
							{
								for (it_tunnel = Tunnel::tunnels.begin(); it_tunnel != Tunnel::tunnels.end(); it_tunnel++)
								{
									if (it_tunnel->b == (*it_b-DIR[i]))
										break;
								}
								assert(it_tunnel != Tunnel::tunnels.end());
							}
							const Tunnel& t = *it_tunnel;

							if (ground(*it_b-DIR[i]) == 'A')
							{
								it2 = boxes.find(t.b);
							}
							else 
							{
								it2 = boxes.find(t.a);
							}

							//There is already a box in this tunnel
							if (it2 != boxes.end())
							{
								continue;
							}

							s = new State(*this, *it_b, i);
						}
					}
					else
					{
						s = new State(*this, *it_b, i);
					}

					bool repeated_state = false;
					//std::cout << "Compare with all states" << std::endl;
					if (all_states.find(s) != all_states.end())
					{
						repeated_state = true;
					}

					// deadlock check:
					Point inv_dir(-DIR[i].i, -DIR[i].j);
					bool hasDeadlock = isDeadlock(*it_b-DIR[i],inv_dir,boxes);                                                                                
					//If pushing in this direction, push the box inside a corral,
					//and it is the only possible push for this box (because other
					//direction are in a corral or are walls, we will have to push
					/*
					if (isInCorral(inv_dir))
					{
					//	if ((*it_b).i

					}
					*/


					if (repeated_state || hasDeadlock )
					{
						//std::cout << "Repeated state" << std::endl;
						delete s;
						continue;
					}
					else
					{
						//std::cout << "XXX Interesting state Add to queue" << std::endl;
					}
					all_states.insert(s);
					s->is_in_all_list = true;
					s->add_to_l();

					next.push_back(s);
					if (s->isFinal())
						return s;
				}
			}
		}
	}
	free_reachable_area();
	return NULL;
	//TODO Maybe put deletion of reachable_area here rather than in the destructor
}

//A corral is an area that the player can't reach (!!!! in the following picture:)
//##################
//#!!!!!!$  @
//#################
bool State::isInCorral(const Point& p)
{
	//If p is empty (or a goal), without box, but outside the reachable area.
	return (!reachable_area[p.i][p.j] && 
			(ground.isEmpty(p) || ground(p)== GOAL) && 
			boxes.find(p)==boxes.end());
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
	//std::cout << "Here" << std::endl;
	assert(!to_expand.empty());
	State * to_return = to_expand.top();
	//std::cout << "Here1" << std::endl;
	to_expand.pop();
	//std::cout << "Here2" << std::endl;
	//std::cout << to_return << std::endl;
	to_return->is_in_expand_list = false;
	//std::cout << "There" << std::endl;
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
		Point goal = moved_box + DIR[dir];
        	std::string path_part;
		path_part = ground.findPath(player_position, goal, *from_state);
		//ground.getPath(player_position,goal, path_part, *from_state);

		player_position = goal;
		goal = moved_box;
		path_part.append( ground.addDirectionLetter(player_position, moved_box));

		//**************************************************//

		final_path.insert(0, path_part);
		to_state = from_state;
		from_state = parent_state;
	}
    
    return final_path;
}
