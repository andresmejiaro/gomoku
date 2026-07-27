#include "gomoku.h"

int negamax(int depth, t_game_func *funcs, void *state,int root, void **out_move){
	
	int ps;
	int score = -10000000;
	int n_moves;
	void **moves;
	void *new_state;
	
	if (depth == 0 || funcs->is_terminal(state))
		return funcs->evaluate(state);
	moves = funcs->get_moves(state, &n_moves);
	for (int m = 0; m < n_moves; m++){
		new_state = funcs->play_move(state, moves[m]);
		ps = -negamax(depth - 1, funcs, new_state,0,0);
		score = max(score, ps);
		if (root && ps == score)
            (* out_move) = moves[m];
        free(new_state);
	}
	funcs->free_moves(moves,&n_moves);
	return score;
}



int negapruning(int depth,
                t_game_func *funcs,
                void *state,
                int alpha,
                int beta,
                int root,
                void **out_move){
	
	int present_score;
	int best_value = -10000000;
	int n_moves;
	void **moves;
	void *new_state;
    
	
	if (depth == 0 || funcs->is_terminal(state))
		return funcs->evaluate(state);
	moves = funcs->get_moves(state, &n_moves);
	for (int m = 0; m < n_moves; m++){
		new_state = funcs->play_move(state, moves[m]);
		present_score = -negapruning(depth - 1, funcs, new_state,-beta,-alpha,0,0);
		if (present_score > best_value){
            best_value = present_score;
            if (root)
            (* out_move) = moves[m];
            if (present_score > alpha)
                alpha = present_score;
        }
		free(new_state);
        if (present_score >= beta)
            break;
	}
	funcs->free_moves(moves,&n_moves);
	return best_value;
}
