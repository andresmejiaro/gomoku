#include "gomoku.h"


int evaluate(const t_game_state *input){
    int sign = input->turn % 2;

    sign = sign ? -1 : 1;

    if (input->captures[0] >= 10){
        return sign * 1600000;
    }
    if (input->captures[1] >= 10){
        return sign *-1600000;
    }

    return sign*(8*(input->score[0]) - 8*(input->score[1])+ 5*(input->captures[0])*(input->captures[0]) - 5*(input->captures[1])*(input->captures[1]) );
}
