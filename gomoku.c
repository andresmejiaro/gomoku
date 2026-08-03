#include "gomoku.h"

int main(){
    t_game_state game = {0}, game2;
    int move;
    int alpha;
    int beta;

    alpha = -10000000;
    beta = 10000000;
    

    while(1){
        print_screen(&game);
        negapruning(7, &game, alpha, beta, 1, &move);
        printf("move: %d\n",move);
        play_move_number(&game,&game2,move);
        game = game2;
        if (is_terminal(&game))
            break;
    }
    print_screen(&game);
}
