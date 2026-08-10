#include "gomoku.h"


int evaluate(const t_game_state *input){
  

    return (input->score[0] + input->score[1]);
}
