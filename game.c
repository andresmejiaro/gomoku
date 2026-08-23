#include "gomoku.h"


void initialize_game_state(t_game_state *input){
    bzero(input, sizeof(t_game_state));

    input->available_machine_moves[BOARD_CELLS / 2 ] = 1;
    (input->machine_moves_scores)[0].move =  BOARD_CELLS / 2;
}
