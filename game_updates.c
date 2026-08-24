#include "gomoku.h"


void queue_update_stone(t_game_state *input, int move, t_update_run *update)
{
    (void)input;
    (void)move;
    (void)update;
}


void queue_update_free_space(t_game_state *input, int move, t_update_run *update)
{
    (void)input;
    (void)move;
    (void)update;
}



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


void update_available_machine_move_pos(t_game_state *input,
    int move){
    int x = move / BOARD_SIDE;
    int y = move % BOARD_SIDE;
    
    char cell_content = get_pos(input,x,y); 

    if (cell_content == 'W' || cell_content == 'B'){
        if (get_pos(input,x+1,y) == '0')
            set_av(input,x+1,y,1);
        if (get_pos(input,x+1,y+1) == '0')
            set_av(input,x+1,y+1,1);
        if (get_pos(input,x+1,y-1) == '0')
            set_av(input,x+1,y-1,1);
        if (get_pos(input,x-1,y) == '0')
            set_av(input,x-1,y,1);
        if (get_pos(input,x-1,y+1) == '0')
            set_av(input,x-1,y+1,1);
        if (get_pos(input,x-1,y-1) == '0')
            set_av(input,x-1,y-1,1);
        if (get_pos(input,x,y+1) == '0')
            set_av(input,x,y+1,1);
        if (get_pos(input,x,y-1) == '0')
            set_av(input,x,y-1,1);  
        set_av(input,x,y,0);
    }

    if (cell_content == '0'){
        set_av(input,x+1,y+1,is_machine_move(input,x+1,y+1));
        set_av(input,x+1,y-1,is_machine_move(input,x+1,y-1));
        set_av(input,x+1,y,is_machine_move(input,x+1,y));
        set_av(input,x-1,y+1,is_machine_move(input,x-1,y+1));
        set_av(input,x-1,y-1,is_machine_move(input,x-1,y-1));
        set_av(input,x-1,y,is_machine_move(input,x-1,y));
        set_av(input,x,y+1,is_machine_move(input,x,y+1));
        set_av(input,x,y-1,is_machine_move(input,x,y-1));
        set_av(input,x,y,is_machine_move(input,x,y));
    }

}


/* Scaffold only: ordering-score maintenance is not implemented yet. */
void update_machine_moves_scores_pos(t_game_state *input, int move){
    (void)input;
    (void)move;
}


/* Scaffold only: aggregate score maintenance is not implemented yet. */
void update_score(t_game_state *input){
    (void)input;
}
