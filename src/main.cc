#include <iostream>
#include <cstdlib>
#include "csocket.h"
#include "ground.h"

int main(int pArgC, char* pArgs[])
{
    if(pArgC<4)
    {
        std::cerr << "usage: " << pArgs[0] << " host port boardnum" << std::endl;
        return -1;
    }

    soko::CSocket lSocket(pArgs[1],pArgs[2]);

    lSocket.WriteLine(pArgs[3]);
    
    std::string lLine;

    //read number of rows
    lSocket.ReadLine(lLine);
    int lNumRows=atoi(lLine.c_str());
    
    //read each row
	std::cout << "Parsing rows" << std::endl;
    for(int i=0;i<lNumRows;i++)
    {
        lSocket.ReadLine(lLine);
        //here, we would store the row somewhere, to build our board
        //in this demo, we just print it
        std::cout << lLine << "\n";
		ground.addRow(lLine);
    }

	std::cout << "---- Display map ----" << std::endl;
	ground.display();
	std::cout << "---- Display initial state ----" <<std::endl;
	State init = ground.getInitialState();
	init.display();

	bool found_solution = false;
	State * final_state = NULL;
	int i = 0;
	while (!found_solution)
	{
		i++;
		State* to_expand = State::nextState();
		//to_expand->display();
		found_solution = to_expand->expand();
		if (found_solution)
		{
			//Not true
			final_state = to_expand; 
		}
	}
	std::cout << " Found a solution after having expanded " 
		<< i << "nodes " << std::endl;

	State * curr_state = final_state;
	std::cout << "------------" << std::endl;
	std::cout << "Final State " << std::endl;
	std::cout << "------------" << std::endl;
	curr_state->display();
	while (curr_state->parent !=NULL)
	{
		curr_state = curr_state->parent;
		curr_state->display();
	}



    
    //now, we should find a path from the player to any goal

    //we've found our solution    
	std::string lMySol("U R R U U L D L L U L L D R R R R L D D R U R U D L L U R");
	//these formats are also valid:
	//std::string lMySol("URRUULDLLULLDRRRRLDDRURUDLLUR");
    //std::string lMySol("0 3 3 0 0 2 1 2 2 0 2 2 1 3 3 3 3 2 1 1 3 0 3 0 1 2 2 0 3");

    //send the solution to the server
    lSocket.WriteLine(lMySol);
    
    //read answer from the server
    lSocket.ReadLine(lLine);
    
    std::cout << lLine << std::endl;

    return 0;
}

