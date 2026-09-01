#include "gomoku.h"

// int negamax(int depth, t_game_func *funcs, void *state,int root, void **out_move){
	
// 	int ps;
// 	int score = -10000000;
// 	int n_moves;
// 	void **moves;
// 	void *new_state;
	
// 	if (depth == 0 || funcs->is_terminal(state))
// 		return funcs->evaluate(state);
// 	moves = funcs->get_moves(state, &n_moves);
// 	for (int m = 0; m < n_moves; m++){
// 		new_state = funcs->play_move(state, moves[m]);
// 		ps = -negamax(depth - 1, funcs, new_state,0,0);
// 		score = MAX(score, ps);
// 		if (root && ps == score)
//             (* out_move) = moves[m];
//         free(new_state);
// 	}
// 	funcs->free_moves(moves,&n_moves);
// 	return score;
// }



int negapruning(int depth,
                t_game_state *input,
                int alpha,
                int beta,
                int root,
                int *out_move,
				t_scored_move *previous_order){
	
	int present_score;
	int best_value = -10000001;
	t_scored_move move[BOARD_CELLS];
 	
	if (depth == 0 || is_terminal(input))
		return evaluate(input);
	
	
	if (previous_order != NULL){
		inherit_ordening(move,previous_order,input);
	} else {
		create_ordening(move,input);
	}

	order_moves(move);
	
	int counter = 0;
    
	while (counter < BOARD_CELLS){
		if(!is_move_valid(input,move[counter].move)){
			counter++;
			continue;
		}
		t_move_undo undo = play_move_number(input, move[counter].move);
		present_score = -negapruning(depth - 1, input,-beta,-alpha,0,0,move);
		if (present_score > best_value){
            best_value = present_score;
            if (root)
            	(* out_move) = move[counter].move;
            if (present_score > alpha)
                alpha = present_score;
        }
		undo_move(input,undo);
        if (present_score >= beta)
            break;
		if (move[counter].score == 0)
			break;
		counter++;
	}	
	return best_value;
}
