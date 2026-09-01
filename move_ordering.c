#include "gomoku.h"


void inherit_ordening(t_scored_move *move,
                      const t_scored_move *previous_order,
                      t_game_state *input)
{
    for (int i = 0; i < BOARD_CELLS; i++)
    {
        move[i].move = previous_order[i].move;
        move[i].score = input->score_board[move[i].move].candidate_score;
    }

}


void create_ordening(t_scored_move *move, const t_game_state *input)
{
    for (int i = 0; i < BOARD_CELLS; i++)
    {
        move[i].move = i;
        move[i].score = input->score_board[i].candidate_score;
    }
}


void order_moves(t_scored_move *move)
{
    qsort(move, BOARD_CELLS, sizeof(t_scored_move), comp_moves);
}
