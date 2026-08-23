#include "gomoku.h"


t_move_undo play_move(t_game_state *input, int row, int col){
    int totpos;
    t_move_undo to_return;

    bzero(&to_return,sizeof(t_move_undo));

    totpos = row*BOARD_SIDE + col;

    to_return.move = totpos;

    if (input->turn % 2 == 1){
        input->board[totpos] = -1;
    } else {
        input->board[totpos] = 1;
    }

    captures(input,totpos,&to_return);
    //update_x_pos was run inside captures for captured stones.

    update_score_board_move_pos(input,totpos);
    update_available_machine_move_pos(input,totpos);
    update_machine_moves_scores_pos(input,totpos);
    update_score(input);

    (input->turn)++;
    return to_return;
}


void undo_move(t_game_state *input, t_move_undo undo){

    (input->turn)--;

    int me = 1;
    if (input->turn % 2)
        me = -1;

    for (int i = 0; i < undo.captured_count; i++)
        input->board[undo.captured_positions[i]] = -me;

    input->board[undo.move] = 0;
    update_score_board_move_pos(input,undo.move);
    update_available_machine_move_pos(input,undo.move);
    update_machine_moves_scores_pos(input,undo.move);

    for (int i = 0; i < undo.captured_count; i++){
        update_score_board_move_pos(input,undo.captured_positions[i]);
        update_available_machine_move_pos(input,undo.captured_positions[i]);
        update_machine_moves_scores_pos(input,undo.captured_positions[i]);
    }

    if (me == 1)
        input->captures[0] -= undo.captured_count;
    else
        input->captures[1] -= undo.captured_count;

}


t_move_undo play_move_number(t_game_state *input, int move_number){
        int x = move_number / BOARD_SIDE;
        int y = move_number % BOARD_SIDE;
        return play_move(input, x, y);
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


void set_pos(t_game_state *input, int row, int col, char color){
    
    int totpos;

    if (row < 0 || col < 0 || row > BOARD_SIDE - 1 || col > BOARD_SIDE - 1)
        return;
    
    totpos = row*BOARD_SIDE + col;
 
    if (color == 'B'){
        input->board[totpos] = 1;
        return;
    }
    if (color == 'W'){
        input->board[totpos] = -1;
        return;
    }

    return;
}


void set_av(t_game_state *input, int row, int col, int av){
    
    int totpos;

    if (row < 0 || col < 0 || row > BOARD_SIDE - 1 || col > BOARD_SIDE - 1)
        return;
    
    totpos = row*BOARD_SIDE + col;
 
    input->available_machine_moves[totpos] = av;

    return;
}

int get_av(t_game_state *input, int row, int col){
    
    int totpos;

    if (row < 0 || col < 0 || row > BOARD_SIDE - 1 || col > BOARD_SIDE - 1)
        return -1;
    
    totpos = row*BOARD_SIDE + col;
 
    return input->available_machine_moves[totpos];

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


/* Scaffold only: ordered move selection is not implemented yet. */
t_scored_move next_move(const t_game_state *input, int last_move){
    t_scored_move move;

    (void)input;
    if (last_move < 0)
        move.move = BOARD_CELLS / 2;
    else
        move.move = -1;
    move.score = 0;
    return move;
}
