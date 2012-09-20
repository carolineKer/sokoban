#ifndef __STATE_H
#define __STATE_H

#include "point.h"
#include <vector>
#include <set>
#include <list>
#include <queue>

class State
{
	public:
		//build initial state
		State(const std::set<Point>& boxes, const Point& player);

		//build a state from a move and a previous state
		State(State& prev_state, const Point& moved_box, int dir);
		~State();

		//Print the reachable area for this state
		void display();

		//Returns the final state if it one found while expanding this state.
		//Otherwise returns NULL
		State * expand();

		//Returns true if all the boxes are on a goal.
		bool isFinal();

		void add_to_l();
		void remove_from_l();

		State* getParentState() {return parent;};

		//Compare a state with anoter state. Two states are equal
		//if the boxes are at the same position and the reaching area
		//(represented by max_pos) is the same.
		bool operator==(const State& a);
		bool operator!=(const State& a);

		//Returns (and pop) the next states from to_expand_list
		static State* nextStateToExpand();

		//Getter to private set of boxes
		std::set<Point> getBoxes();
		
		//Compute a distance between a State and the Final State
		int euristic();

		struct Compare {
			bool operator()(State* a, State* b)
			{
				return (a->euristic() > b->euristic());
			};
		};

		
	
	private:
		std::set<Point> boxes;

		////////////////////////////////////////////////////////
		//  FROM PARENT: if this state is not the initial state,
		//  it was build from parent, moved_box and dir
		//  The player went to the case moved_box+DIR[dir]. He
		//  pushed the box which was on case moved_box to the
		//  case moved_box-DIR[dir].
		////////////////////////////////////////////////////////
		Point moved_box;
		int dir;
		//Pointer to the previous state, NULL only if this is the initial
		//state
		State * parent;

		///////////////////////////////
		//  NEXT STATES
		//////////////////////////////
		std::vector<State*> next;

		////////////////////////////////
		// REACHABLE AREA
		////////////////////////////////

		//Area that the player can reach
		//--> this is big and it is perhaps unecessary to keep it
		//once the node is expanded.
		bool ** reachable_area;
		Point max_pos;
		void compute_reachable_area(const Point& from);


		///////////////////////////////
		//  LISTS
		///////////////////////////////
		//Contains the states wich must be expanded (because we are doing a BFS)
		static std::priority_queue<State*, std::vector<State*>, State::Compare > to_expand;

		//Contains all the already visited states
		static std::list<State*> all_states;
		
		//True if the state is registered in to_expand
		bool is_in_expand_list;

		//True if the state is registered in all_states
		bool is_in_all_list;

		//If the state is registered in all_list, points to the position of the state in the list.
		std::list<State*>::iterator p_in_all_list;

};

#endif
