#ifndef __Ground__H
#define __Ground__H

#include <string>
#include <vector>
#include "point.h"
#include "state.h"


using std::string;
using std::vector;

//This class stores the position of the different objects.
//The unmovable object such as walls and goals are stored in
//a vector of vector.
//The position of the player and of the 
class Ground {

	public:
		////////////////////////
		// Building the ground
		////////////////////////
		//Build an empty Ground
		Ground();
		~Ground();

		//Add a row at the end of the Ground
		bool addRow(const string& col);

		void display();

		State getInitialState();

	private:
		//Current player position
		Point player;
		Point Ground_size;
		vector<string *> __ground;
		vector<Point> goals;
		vector<Point> boxes;
		Point ground_size;
};

#endif /*__Ground__H*/
