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

char* in_pos = new char[3];
in_pos[0] = ' '; // empty space
in_pos[1] = '$'; // a box
in_pos[2] = '#'; // a wall


if ( surround[0] == in_pos[0] ){
    // 5 3
    // 4$2
    //  @
    return false;
}


else if ( surround[0] == in_pos[1] ){
    // 5$3
    // 4$2
    //  @
    if ( surround[1] == in_pos[0] ){
        // 5$3
        // 4$ 
        //  @
        if ( surround[2] == in_pos[0] ){
            // 5$
            // 4$ 
            //  @
            if ( surround[3] == in_pos[0] ){
                // 5$
                //  $ 
                //  @
                return false;
            }
            if ( surround[3] == in_pos[1] ){
                // 5$
                // $$ 
                //  @
                if ( surround[4] == in_pos[0] ){
                    //  $
                    // $$ 
                    //  @
                    return false;
                }
                if ( surround[4] == in_pos[1] ){
                    // $$
                    // $$ 
                    //  @
                    return true;
                }
                if ( surround[4] == in_pos[2] ){
                    // #$
                    // $$
                    //  @
                    return true;
                }
            } 
            if ( surround[3] == in_pos[2] ){
                // 5$
                // #$ 
                //  @
                if ( surround[4] == in_pos[0] ){
                    //  $
                    // #$ 
                    //  @
                    return false;
                }
                if ( surround[4] == in_pos[1] ){
                    // $$
                    // #$ 
                    //  @
                    return true;
                }
                if ( surround[4] == in_pos[2] ){
                    // #$
                    // #$
                    //  @
                    return true;
                }
            } 
        }
        if ( surround[2] == in_pos[1] ){
            // 5$$
            // 4$
            //  @
            if ( surround[3] == in_pos[0] ){
                // 5$$
                //  $ 
                //  @
                return false;
            }
            if ( surround[3] == in_pos[1] ){
                // 5$$
                // $$
                //  @
                if ( surround[4] == in_pos[0] ){
                    //  $$
                    // $$ 
                    //  @
                    return false;
                }
                if ( surround[4] == in_pos[1] ){
                    // $$$
                    // $$ 
                    //  @
                    return true;
                }
                if ( surround[4] == in_pos[2] ){
                    // #$$
                    // $$
                    //  @
                    return true;
                }
            }
            if ( surround[3] == in_pos[2] ){
                // 5$$
                // #$ 
                //  @
                if ( surround[4] == in_pos[0] ){
                    //  $$
                    // #$ 
                    //  @
                    return false;
                }
                if ( surround[4] == in_pos[1] ){
                    // $$$
                    // #$
                    //  @
                    return true;
                }
                if ( surround[4] == in_pos[2] ){
                    // #$$
                    // #$ 
                    //  @
                    return true;
                }
            }
        }
        if ( surround[2] == in_pos[2] ){ 
            // 5$#
            // 4$
            //  @
            if ( surround[3] == in_pos[0] ){
                // 5$#
                //  $ 
                //  @
                return false;
            }
            if ( surround[3] == in_pos[1] ){
                // 5$#
                // $$ 
                //  @
                if ( surround[4] == in_pos[0] ){
                    //  $#
                    // $$ 
                    //  @
                    return false;
                }
                if ( surround[4] == in_pos[1] ){
                    // $$#
                    // $$ 
                    //  @
                    return true;
                }
                if ( surround[4] == in_pos[2] ){
                    // #$#
                    // $$ 
                    //  @
                    return true;
                }
            }
            if ( surround[3] == in_pos[2] ){
                // 5$#
                // #$ 
                //  @
                return true;
            }
        }               
    }
    if ( surround[1] == in_pos[1] ){
        // 5$3
        // 4$$
        //  @
        if ( surround[2] == in_pos[0] ){
            // 5$
            // 4$$
            //  @
            if ( surround[3] == in_pos[0] ){
                // 5$
                //  $$
                //  @
                return false;
            }
            if ( surround[3] == in_pos[1] ){
                // 5$
                // $$$
                //  @
                if ( surround[4] == in_pos[0] ){
                    //  $
                    // $$$
                    //  @
                    return false;
                }
                if ( surround[4] == in_pos[1] ){
                    // $$ 
                    // $$$
                    //  @
                    return true;   
                }
                if ( surround[4] == in_pos[2] ){
                    // #$ 
                    // $$$
                    //  @
                    return true;
                }
            }
            if ( surround[3] == in_pos[2] ){
                // 5$ 
                // #$$
                //  @
                if ( surround[4] == in_pos[0] ){
                    //  $
                    // #$$
                    //  @
                    return false;
                }
                if ( surround[4] == in_pos[1] ){
                    // $$ 
                    // #$$
                    //  @
                    return true;
                }
                if ( surround[4] == in_pos[2] ){
                    // #$ 
                    // #$$
                    //  @
                    return true;
                }
            }
        }
        if ( surround[2] == in_pos[1] ){
            // 5$$ 
            // 4$$
            //  @
            return true;
        }
        if ( surround[2] == in_pos[2] ){
            // 5$#
            // 4$$
            //  @
            return true;
        }
    }
    if ( surround[1] == in_pos[2] ){
        // 5$3
        // 4$#
        //  @
        if ( surround[2] == in_pos[0] ){
            // 5$ 
            // 4$#
            //  @
            if ( surround[3] == in_pos[0] ){
                // 5$
                //  $#
                //  @
                if ( surround[4] == in_pos[0] ){
                    //  $ 
                    //  $#
                    //  @
                    return false;
                }
                if ( surround[4] == in_pos[1]){
                    // $$
                    //  $#
                    //  @
                    return false;
                }
                if ( surround[4] == in_pos[2] ){
                    // #$
                    //  $#
                    //  @
                    return true;
                }
            }
            if ( surround[3] == in_pos[1] ){
                // 5$
                // $$#
                //  @
                if ( surround[4] == in_pos[0] ){
                    //  $ 
                    // $$#
                    //  @
                    return false;
                }
                if ( surround[4] == in_pos[1]){
                    // $$
                    // $$#
                    //  @
                    return true;
                }
                if ( surround[4] == in_pos[2] ){
                    // #$
                    // $$#
                    //  @
                    return true;
                }                
            }
            if ( surround[3] == in_pos[2] ){
                // 5$ 
                // #$#
                //  @ 
                if ( surround[4] == in_pos[0] ){
                    //  $ 
                    // #$#
                    //  @
                    return false;
                }
                if ( surround[4] == in_pos[1]){
                    // $$
                    // #$#
                    //  @
                    return true;
                }
                if ( surround[4] == in_pos[2] ){
                    // #$
                    // #$#
                    //  @
                    return true;
                }
            }
        }
        if ( surround[2] == in_pos[1] ){
            // 5$$  
            // 4$#
            //  @
            return true;
        }
        if ( surround[2] == in_pos[2] ){
            // 5$#
            // 4$#
            //  @
            return true;
        }
    }
}


else if ( surround[0] == in_pos[2] ){
    // 5#3
    // 4$2
    //  @
    if ( surround[1] == in_pos[0] ){
        // 5#3
        // 4$ 
        //  @
        if ( surround[2] == in_pos[0] ){
            // 5# 
            // 4$
            //  @
            if ( surround[3] == in_pos[0] ){
                // 5# 
                //  $ 
                //  @
                return false;
            }
            if ( surround[3] == in_pos[1] ){
                // 5# 
                // $$ 
                //  @
                if ( surround[4] == in_pos[0] ){
                    //  # 
                    // $$ 
                    //  @
                    return false;
                }
                if ( surround[4] == in_pos[1] ){
                    // $# 
                    // $$ 
                    //  @
                    return true;
                }
                if ( surround[4] == in_pos[2] ){
                    // ## 
                    // $$ 
                    //  @
                    return true;
                }
            }
            if ( surround[3] == in_pos[2] ){
                // 5# 
                // #$ 
                //  @
                return true;
            }
        }
        if ( surround[2] == in_pos[1] ){
            // 5#$
            // 4$
            //  @        
            if ( surround[3] == in_pos[0] ){
                // 5#$ 
                //  $ 
                //  @
                return false;
            }
            if ( surround[3] == in_pos[1] ){
                // 5#$ 
                // $$ 
                //  @
                if ( surround[4] == in_pos[0]){
                    //  #$
                    // $$ 
                    //  @
                    return false;
                }
                if ( surround[4] == in_pos[1]){
                    // $#$
                    // $$ 
                    //  @
                    return true;
                }
                if ( surround[4] == in_pos[2]){
                    // ##$
                    // $$ 
                    //  @
                    return true;
                }
            }
            if ( surround[3] == in_pos[2] ){
                // 5#$ 
                // #$ 
                //  @
                return true;
            }
        }
        if ( surround[2] == in_pos[2] ){
            // 5##
            // 4$
            //  @        
            if ( surround[3] == in_pos[0] ){
                // 5## 
                //  $ 
                //  @
                return false;
            } 
            if ( surround[3] == in_pos[1] ){
                // 5## 
                // $$ 
                //  @
                if ( surround[4] == in_pos[0] ){
                    //  ##
                    // $$
                    //  @
                    return false;
                }
                if ( surround[4] == in_pos[1] ){
                    // $##
                    // $$
                    //  @
                    return true;
                }
                if ( surround[4] == in_pos[2] ){
                    // ###
                    // $$
                    //  @
                    return true;
                }
            }
            if ( surround[3] == in_pos[2] ){
                // 5## 
                // #$ 
                //  @
                return true;
            }
        }
    } 
    if ( surround[1] == in_pos[1] ){
        // 5#3
        // 4$$
        //  @
        if ( surround[2] == in_pos[0] ){
            // 5#
            // 4$$
            //  @
            if ( surround[3] == in_pos[0] ){
                // 5#
                //  $$
                //  @
                return false;
            }
            if ( surround[3] == in_pos[1] ){
                // 5#
                // $$$
                //  @
                if ( surround[4] == in_pos[0] ){
                    //  # 
                    // $$$
                    //  @
                    return false;
                }
                if ( surround[4] == in_pos[1] ){
                    // $# 
                    // $$$
                    //  @
                    return true;
                }
                if ( surround[4] == in_pos[2] ){
                    // ## 
                    // $$$
                    //  @
                    return true;
                }
            }
            if ( surround[3] == in_pos[2] ){
                // 5#
                // #$$
                //  @
                return true;
            }
        }
        if ( surround[2] == in_pos[1] ){
            // 5#$
            // 4$$
            //  @
            return true;
        }
        if ( surround[2] == in_pos[2] ){
            // 5##
            // 4$$
            //  @
            return true;
        }
    }
    if ( surround[1] == in_pos[2] ){
        // 5#3
        // 4$#
        //  @
        return true;
    }
}
return false;
}
