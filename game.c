#include "gomoku.h"


void print_screen(t_game_state *game){
    
    char  printable[19][20];
    int sector, residue, row, col;
    
    for(int i = 0; i < 19 ; i++){
        bzero(printable[i], sizeof(printable[i]));
    }
    

    for(int i = 0; i<19*19; i++){
        sector = i / 64;
        residue = i % 64;
        row = i / 19;
        col = i % 19;
        if(game ->white[sector] & (1ULL << residue)){
            printable[row][col] = 'W';
        }  else if(game ->black[sector] & (1ULL << residue)){
            printable[row][col] = 'B';
        } else {
            printable[row][col] = '0';
        }
    }
    for(int i = 0; i < 19 ; i++){
        printf("%2d: %s\n",i,printable[i]);
    }


}


void play_move(t_game_state *input, t_game_state *output, int row, int col){
    memcpy(output, input, sizeof(t_game_state));

    int totpos, sector, residue;

    totpos = row*19 + col;
    sector = totpos / 64;
    residue = totpos % 64;

    if (output->turn % 2 == 1){
        output->white[sector] = 1ULL << residue;
    } else {
        output->black[sector] = 1ULL << residue;
    }

    (output->turn )++;
}

