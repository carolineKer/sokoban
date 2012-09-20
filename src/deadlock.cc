#include "state.h"
#include "ground.h"
#include "point.h"
#include "case_type.h"
#include <iostream>

bool isDeadlock( const Point& p ,Point& dir ,const std::set<Point>& boxes){
    // p:     is the location of the newly moved box
    // dir:   is the direction p was pushed to 
    // boxes: set of all boxes in the current state.
    //
    //  surround is an char array of five elements
    //  it represent the following situation:
    //  
    //      513
    //      4$2
    //       @
    //  
    //  Where: 
    //      surround[0] = 1,
    //      surround[1] = 2,
    //      surround[2] = 3,
    //      surround[3] = 4,
    //      surround[4] = 5,
    //
    //  Returns:
    //      true if we can conclude that this situation involves deadlock 
    //      false otherwise
    //
    // here a will denote either $ or #

    if ( ground(p) == GOAL ){
        // The box is on a goal so it dose not mater
        // if we have deadlock.
        return false;
    }
    // left and right are the directions left and right
    // relative to DIR[i]
    // left operator:  [ [0,-1], [ 1,0] ]
    // right operator: [ [0, 1], [-1,0] ] 
    Point* right = new Point( dir.j,-dir.i);
    Point* left  = new Point(-dir.j, dir.i);

    // list of direction around
    Point dir_list[5] =  {dir,*right,dir+*right,*left,dir+*left};
    char* surround = new char[5];
    for ( int iter=0 ; iter!=5 ; iter++ ){
        if (ground(p+dir_list[iter])==WALL){
            surround[iter] = WALL;
        }
        else if ( boxes.count(p+dir_list[iter]) != 0 ){
            surround[iter] = BOX;
        }
        else{
            surround[iter] = EMPTY;
        }
    }

    char* in_pos = new char[3];
    in_pos[0] = EMPTY; // empty space
    in_pos[1] = BOX; // a box
    in_pos[2] = WALL; // a wall
    

    if ( (surround[0] != in_pos[0]) && (surround[1] != in_pos[0]) && (surround[2] != in_pos[0]) ){
        //  aa
        //  $a
        //  @
        return true;
    }
    if ( (surround[0] != in_pos[0]) && (surround[3] != in_pos[0]) && (surround[4] != in_pos[0]) ){
        // aa
        // a$
        //  @
        
        return true;
    }
    if ( (surround[0] == in_pos[2]) && ((surround[1]==in_pos[2]) || (surround[3]==in_pos[2])) ){
        //  #          #
        //  $#   or   #$
        //  @          @
        return true;
    }
    if ( (surround[0] == in_pos[1]) && (
            (surround[1]==in_pos[2] && surround[4]==in_pos[2]) || 
            (surround[3]==in_pos[2] && surround[2]==in_pos[2])
            ) ){
        // #$          $#
        //  $#   or   #$
        //  @          @
        return true;
    }
    return false;
}
