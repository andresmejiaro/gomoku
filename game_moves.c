#include "gomoku.h"


void play_move(const t_game_state *input, t_game_state *output, int row, int col){
    memcpy(output, input, sizeof(t_game_state));

    int totpos;

    totpos = row*BOARD_SIDE + col;
 
    if (output->turn % 2 == 1){
        output->board[totpos] = -1;
    } else {
        output->board[totpos] = 1;
    }

    (output->turn)++;
}

void play_move_number(const t_game_state *input, t_game_state *output, int move_number){
        int x = move_number / BOARD_SIDE;
        int y = move_number % BOARD_SIDE;
        play_move(input, output, x, y);
}

char get_pos(const t_game_state *input, int row, int col){
    
    int totpos;

    if (row < 0 || col < 0 || row > BOARD_SIDE - 1 || col > BOARD_SIDE - 1)
        return 'X';
    
    totpos = row*BOARD_SIDE + col;
 
    if (input->board[totpos] > 0)
        return 'B';
    if (input->board[totpos] < 0)
        return 'W';
    return '0';
}


int comp_moves(const void *a,const void *b){
    
    const t_scored_move *ma = a;
    const t_scored_move *mb = b;
    
    if (ma->score < mb->score)
        return -1;
    if (ma->score > mb->score)
        return 1;
    return 0;
}
