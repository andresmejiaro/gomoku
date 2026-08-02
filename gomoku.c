#include "gomoku.h"

int main(){
    t_game_state game = {0};

    game.white[3] = 0xFFFFFFFFFFFF;
    print_screen(&game);
}
