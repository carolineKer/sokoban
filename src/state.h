#ifndef __STATE_H
#define __STATE_H

#include "point.h"
#include <vector>
#include <set>
#include <list>

class State
{
	public:
		//build initial state
		State(const std::set<Point>& boxes, const Point& player);

		//build a state from a move and a previous state
		State(State& prev_state, const Point& moved_box, int dir);
		~State();
		void display();

		//Returns true if one of the newly created node is final
		bool expand();

		bool isFinal();

		void add_to_l();
		void remove_from_l();
		static State* nextState();
		bool operator==(const State& a);
		bool operator!=(const State& a);
		State* parent;
	
	private:
		std::set<Point> boxes;

		//Area that the player can reach
		//--> this is big and it is perhaps unecessary to keep it
		//once the node is expanded.
		bool ** reachable_area;
		Point max_pos;
		bool is_in_expand_list;
		bool is_in_all_list;
		void compute_reachable_area(const Point& from);
		std::vector<State*> next;
		std::list<State*>::iterator p_in_list;
		std::list<State*>::iterator p_in_all_list;
		static std::list<State*> to_expand;
		static std::list<State*> all_states;

		Point moved_box;
		int dir;
};
#endif
