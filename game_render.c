#include "gomoku.h"


void print_screen(const t_game_state *game){

    char    printable[BOARD_SIDE][20];
    int     row, col;
    
    for(int i = 0; i < BOARD_SIDE ; i++){
        bzero(printable[i], sizeof(printable[i]));
    }
    

    for(int i = 0; i<BOARD_CELLS; i++){
        row = i / BOARD_SIDE;
        col = i % BOARD_SIDE;
        if(game ->board[i] < 0){
            printable[row][col] = 'W';
        }  else if(game ->board[i] > 0){
            printable[row][col] = 'B';
        } else {
            printable[row][col] = '0';
        }
    }
    for(int i = 0; i < BOARD_SIDE ; i++){
        printf("%2d: %s\n",i,printable[i]);
    }
}
