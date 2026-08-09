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


int valid_plays(const t_game_state *input){
    int cum_sum = 0;

    for (int i = 0; i < BOARD_CELLS; i++){
        cum_sum += input->available_machine_moves[i];
    }

    return cum_sum > 0;
}


int is_terminal(const t_game_state *input)
{
    if (has_won(input) != '0')
        return 1;
    if (!valid_plays(input))
        return 1;
    return 0;
}


int evaluate(const t_game_state *input){
  

    return (input->score[0] + input->score[1]);
}
