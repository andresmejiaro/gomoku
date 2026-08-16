#include "gomoku.h"

int coors_to_move(int row, int col){
    return row*BOARD_SIDE + col;
}

void move_to_coords(int move, int *row, int *col){
    *row = move / BOARD_SIDE;
    *col = move % BOARD_SIDE;
}