#include "gomoku.h"

int do_stones_need_removal(char s1, char s2, char s3, char s4){
    if (s1 != s4)
        return 0;
    if (!(s1 == 'B' || s1 == 'W'))
        return 0;
    if (s2 == s3 && s1 != s2 && (s2 == 'B' || s2 == 'W'))
        return 1;
    return 0;
}


void remove_stone(int move,t_game_state *input, t_move_undo *undo){

    int raw_side = (input->board)[move];   /* +1 or -1, before conversion */
    int side = raw_side;
    (input->board)[move] = 0;

    if (side == 1){
        side = 0;
    }
    if (side == -1){
        side = 1;
    } // If convention 1 Black -1 White and captures[0] -> Black and so forth

    input->available_machine_moves[move] = 1;

    undo->captured_positions[undo->captured_count] = move;
    (undo->captured_count)++;
    (input->captures)[(side + 1) % 2] ++; //If the stone was white it counts for black and so on
    update_score_board_move_pos(input,move,raw_side);
    update_available_machine_move_pos(input,move);
    
}



void check_dir_capture(t_game_state *input, int move, t_move_undo *undo, int dir){
    int row,col,dx,dy,sign;
    char stone_1, stone_2, stone_3, stone_4;

    move_to_coords(move,&row,&col);
    set_dir(dir,&dx,&dy);

    stone_1 = get_pos(input,row,col);

    if (!(stone_1 == 'W' || stone_1 == 'B'))
        return;
    
    sign = 1;
    for (int i = 0; i<2; i++){
        stone_2 = get_pos(input,row + sign* dx,col + sign*dy);
        stone_3 = get_pos(input,row + sign*2*dx,col + sign*2*dy);
        stone_4 = get_pos(input,row + sign*3*dx,col + sign*3*dy);

        if(do_stones_need_removal(stone_1,stone_2,stone_3,stone_4)){
            remove_stone(coors_to_move(row + sign* dx,col + sign*dy),input, undo);
            remove_stone(coors_to_move(row + sign*2*dx,col + sign*2*dy),input, undo);
        }


        sign *= -1;
    } 
}




void captures(t_game_state *input, int move, t_move_undo *undo){
   for (int i = 0; i < 4; i++){
        check_dir_capture(input,move,undo,i);
   }    
 
}


char has_won(const t_game_state *input){
    for (int move = 0; move < BOARD_CELLS; move++) {
        if (input->board[move] == 0)
            continue;
        if (input->score_board[move].score >= 10000)
            return input->board[move] > 0 ? 'B' : 'W';
    }
    return '0';
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


int is_move_valid(const t_game_state *input, int move){
    if (move < 0 || move >= BOARD_CELLS)
        return 0;
    return input->available_machine_moves[move] && input->board[move] == 0;
}


static int compare_moves(const void *left, const void *right){
    const t_scored_move *a = left;
    const t_scored_move *b = right;

    if (a->score != b->score)
        return b->score - a->score;
    return a->move - b->move;
}


static int move_priority(const t_game_state *input, int move){
    int row;
    int col;
    int priority = 0;

    move_to_coords(move, &row, &col);
    for (int dr = -1; dr <= 1; dr++) {
        for (int dc = -1; dc <= 1; dc++) {
            int neighbour_row = row + dr;
            int neighbour_col = col + dc;

            if ((dr == 0 && dc == 0)
                || neighbour_row < 0 || neighbour_row >= BOARD_SIDE
                || neighbour_col < 0 || neighbour_col >= BOARD_SIDE)
                continue;
            int neighbour = coors_to_move(neighbour_row, neighbour_col);
            if (input->board[neighbour] != 0)
                priority = MAX(priority,
                    abs(input->score_board[neighbour].score));
        }
    }
    return priority;
}


int ordered_moves(const t_game_state *input, t_scored_move *moves){
    int count = 0;

    for (int move = 0; move < BOARD_CELLS; move++) {
        if (!is_move_valid(input, move))
            continue;
        moves[count].move = move;
        moves[count].score = move_priority(input, move);
        count++;
    }
    qsort(moves, count, sizeof(*moves), compare_moves);
    return count;
}
