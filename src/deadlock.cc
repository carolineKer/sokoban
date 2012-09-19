bool isDeadlock(char* surround){
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

    char* in_pos = new char[3];
    in_pos[0] = ' '; // empty space
    in_pos[1] = '$'; // a box
    in_pos[2] = '#'; // a wall

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
