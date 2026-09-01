#include "gomoku.h"


t_move_undo play_move(t_game_state *input, int row, int col){
    int totpos;
    t_move_undo to_return;
    t_update_run update;

    bzero(&to_return,sizeof(t_move_undo));
    bzero(&update,sizeof(t_update_run));

    totpos = row*BOARD_SIDE + col;

    to_return.move = totpos;

    if (input->turn % 2 == 1){
        input->board[totpos] = -1;
    } else {
        input->board[totpos] = 1;
    }
    

    captures(input,totpos,&to_return, &update);
    //update_x_pos was run inside captures for captured stones.

    //update_score_board_move_pos(input,totpos, &update);
    queue_local_space(input,totpos, &update);
    update_all(input, &update);
    update_score(input);

    (input->turn)++;
    return to_return;
}


void undo_move(t_game_state *input, t_move_undo undo){

    t_update_run update = {0};
    (input->turn)--;

    int me = 1;
    if (input->turn % 2)
        me = -1;
    

    for (int i = 0; i < undo.captured_count; i++)
        input->board[undo.captured_positions[i]] = -me;

    input->board[undo.move] = 0;
    input->score[(1-me)/2] -= input->score_board[undo.move].score;
    queue_local_space(input,undo.move,&update);
    //update_available_machine_move_pos(input,undo.move);

    for (int i = 0; i < undo.captured_count; i++){
        queue_local_space(input,undo.captured_positions[i],&update);
        //update_available_machine_move_pos(input,undo.captured_positions[i]);
    }

    if (me == 1)
        input->captures[0] -= undo.captured_count;
    else
        input->captures[1] -= undo.captured_count;
    update_all(input, &update);

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


int comp_moves(const void *a,const void *b){
    
    const t_scored_move *ma = a;
    const t_scored_move *mb = b;
    
    if (ma->score < mb->score)
        return 1;
    if (ma->score > mb->score)
        return -1;
    return 0;
}
