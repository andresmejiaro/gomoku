#include "gomoku.h"

int main(){
    t_game_state game = {0};
    int move;
    int alpha;
    int beta;
    int start;
    

    alpha = -10000000;
    beta = 10000000;

    start = 1;
    


    while(1){
        print_screen(&game);
        if (start){
            //start center
            move = BOARD_CELLS / 2;
            printf("move: %d\n",move);
            play_move_number(&game,move);
            start = 0;
            continue;
        }
        negapruning(3, &game, alpha, beta, 1, &move, NULL);
        printf("move: %d\n",move);
        play_move_number(&game,move);
        if (is_terminal(&game))
            break;
    }
    print_screen(&game);
}
