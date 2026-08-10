#include "gomoku.h"


int valid_plays(const t_game_state *input){
    int cum_sum = 0;

    for (int i = 0; i < BOARD_CELLS; i++){
        cum_sum += input->available_machine_moves[i];
    }

    return cum_sum > 0;
}


int is_terminal(const t_game_state *input)
{
    if (has_won(input) != '0')
        return 1;
    if (!valid_plays(input))
        return 1;
    return 0;
}


bool is_move_valid(const t_game_state *input, int move){
    
}