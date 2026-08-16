#include "gomoku.h"


void initialize_game_state(t_game_state *input){
    bzero(input, sizeof(t_game_state));

    input->available_machine_moves[BOARD_CELLS / 2 ] = 1;
}
