#include "gomoku.h"

int main(){
    t_game_state game;
    int move;
    int alpha;
    int beta;
    struct timeval start;
    struct timeval end;

    alpha = -10000000;
    beta = 10000000;
    initialize_game_state(&game);
    

    while(1){
        print_screen(&game);
        gettimeofday(&start, NULL);
        negapruning(7, &game, alpha, beta, 1, &move);
        gettimeofday(&end, NULL);
        double elapsed = end.tv_sec - start.tv_sec
            + (end.tv_usec - start.tv_usec) / 1000000.0;
        printf("move: %d (%.6f s)\n",move,elapsed);
        play_move_number(&game,move);
        if (is_terminal(&game))
            break;
    }
    print_screen(&game);
}
