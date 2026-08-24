#include "gomoku.h"


int do_stones_need_removal(char s1, char s2, char s3, char s4){
    if (s1 != s4)
        return 0;
    if (!(s1 == 'B' || s1 == 'W'))
        return 0;
    if (s2 == s3 && s1 != s2 && (s2 == 'B' || s2 == 'W'))
        return 1;
    return 0;
}


void remove_stone(int move,t_game_state *input, t_move_undo *undo, t_update_run *update){

    int raw_side = (input->board)[move];   /* +1 or -1, before conversion */
    int side = raw_side;
    (input->board)[move] = 0;

    if (side == 1){
        side = 0;
    }
    if (side == -1){
        side = 1;
    } // If convention 1 Black -1 White and captures[0] -> Black and so forth

    input->available_machine_moves[move] = 1;

    undo->captured_positions[undo->captured_count] = move;
    (undo->captured_count)++;
    (input->captures)[(side + 1) % 2] ++; //If the stone was white it counts for black and so on
    
    queue_update_stone(input,move,update);
    //update_score_board_move_pos(input,move);
    queue_update_free_space(input,move,update);
    //update_available_machine_move_pos(input,move);
    
}



void check_dir_capture(t_game_state *input, int move, t_move_undo *undo, int dir, t_update_run *update){
    int row,col,dx,dy,sign;
    char stone_1, stone_2, stone_3, stone_4;

    move_to_coords(move,&row,&col);
    set_dir(dir,&dx,&dy);

    stone_1 = get_pos(input,row,col);

    if (!(stone_1 == 'W' || stone_1 == 'B'))
        return;
    
    sign = 1;
    for (int i = 0; i<2; i++){
        stone_2 = get_pos(input,row + sign* dx,col + sign*dy);
        stone_3 = get_pos(input,row + sign*2*dx,col + sign*2*dy);
        stone_4 = get_pos(input,row + sign*3*dx,col + sign*3*dy);

        if(do_stones_need_removal(stone_1,stone_2,stone_3,stone_4)){
            remove_stone(coors_to_move(row + sign* dx,col + sign*dy),input, undo, update);
            remove_stone(coors_to_move(row + sign*2*dx,col + sign*2*dy),input, undo, update);
        }


        sign *= -1;
    } 
}



void captures(t_game_state *input, int move, t_move_undo *undo, t_update_run *update){
   for (int i = 0; i < 4; i++){
        check_dir_capture(input,move,undo,i, update);
   }    
 
}


int valid_plays(const t_game_state *input){
    int cum_sum = 0;

    for (int i = 0; i < BOARD_CELLS; i++){
        cum_sum += input->available_machine_moves[i];
    }

    return cum_sum > 0;
}


/* Scaffold only: victory detection is not implemented yet. */
char has_won(const t_game_state *input){
    (void)input;
    return '0';
}


int is_terminal(const t_game_state *input)
{
    if (has_won(input) != '0')
        return 1;
    if (!valid_plays(input))
        return 1;
    return 0;
}


int is_move_valid(const t_game_state *input, int move){
    (void)input;
    (void)move;
    return 1;
}
