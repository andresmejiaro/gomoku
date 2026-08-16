#include "gomoku.h"


void captures(t_game_state *input, int move, t_move_undo *to_return){
    (void)input;
    (void)move;
    (void)to_return;
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
