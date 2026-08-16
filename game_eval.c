#include "gomoku.h"


int evaluate(const t_game_state *input){
    int score = input->score[0] - input->score[1];

    if (input->turn % 2 != 0)
        score = -score;
    return score;
}
