#include "gomoku.h"


int is_machine_move(const t_game_state *input,
    int x ,int y){
    
    if (get_pos(input,x,y) != '0')
        return 0;
    if (x == BOARD_SIDE / 2 && y == BOARD_SIDE / 2)
        return 1;


    int c1 = (get_pos(input,x+1,y) == 'B') || (get_pos(input,x+1,y) == 'W'); 
    int c2 = (get_pos(input,x+1,y+1) == 'B') || (get_pos(input,x+1,y+1) == 'W');
    int c3 = (get_pos(input,x+1,y-1) == 'B') || (get_pos(input,x+1,y-1) == 'W');
    int c4 = (get_pos(input,x-1,y) == 'B') || (get_pos(input,x-1,y) == 'W');
    int c5 = (get_pos(input,x-1,y+1) == 'B') || (get_pos(input,x-1,y+1) == 'W');
    int c6 = (get_pos(input,x-1,y-1) == 'B') || (get_pos(input,x-1,y-1) == 'W');
    int c7 = (get_pos(input,x,y+1) == 'B') || (get_pos(input,x,y+1) == 'W');
    int c8 = (get_pos(input,x,y-1) == 'B') || (get_pos(input,x,y-1) == 'W');

    return c1 || c2 || c3 || c4 || c5 || c6 || c7 || c8 ;    

}


/* Scaffold only: aggregate score maintenance is not implemented yet. */
void update_score(t_game_state *input){
    (void)input;
}
