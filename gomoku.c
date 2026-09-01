#include "gomoku.h"

int main(){
    t_game_state game = {0};
    int move;
    int alpha;
    int beta;

    alpha = -10000000;
    beta = 10000000;
    

    while(1){
        print_screen(&game);
        negapruning(3, &game, alpha, beta, 1, &move, NULL);
        printf("move: %d\n",move);
        play_move_number(&game,move);
        if (is_terminal(&game))
            break;
    }
    print_screen(&game);
}
