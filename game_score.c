#include "gomoku.h"

void set_dir(int dir, int *dx, int *dy){
    if (dir == DIRECTION_X){
        *dx = 1;
        *dy = 0;
    }
    if (dir == DIRECTION_Y){
        *dx = 0;
        *dy = 1;
    }
    if (dir == DIRECTION_XY){
        *dx = 1;
        *dy = 1;
    }
    if (dir == DIRECTION_XnY){
        *dx = 1;
        *dy = -1;
    }
}


void dir_score(t_game_state *input,int move, int dir){
    int x, y;
    int dx, dy;
    int old_score = input->score_board[move].score;
    char start_boundary;
    char end_boundary;
    set_dir(dir,&dx,&dy);  
    move_to_coords(move,&x,&y);
    char color = get_pos(input,x,y);
    
    if (color == '0')
        return;
    
    
    int start_steps = 1;
    while ((start_boundary = get_pos(input, x-start_steps*dx,
        y-start_steps*dy)) == color)
        start_steps++;
    
    (input->score_board)[move].start[dir] = coors_to_move(x-(start_steps-1)*dx, y-(start_steps-1)*dy);


    int end_steps = 1;
    while ((end_boundary = get_pos(input, x+end_steps*dx,
        y+end_steps*dy)) == color)
        end_steps++;

    (input->score_board)[move].size[dir] = start_steps + end_steps - 1;

    if (end_boundary == '0')
        (input->score_board)[move].open_end[dir] = 1;
    else 
        (input->score_board)[move].open_end[dir] = 0;

    if (start_boundary == '0')
        (input->score_board)[move].open_init[dir] = 1;
    else 
        (input->score_board)[move].open_init[dir] = 0; 
        

    (input->score_board)[move].score_dir[dir] = ((input->score_board)[move].open_init[dir]+
        (input->score_board)[move].open_end[dir])*(end_steps+start_steps -1);

    if (end_steps+start_steps -1>=5)
         (input->score_board)[move].score_dir[dir] = 10000;

    input->score_board[move].score = input->score_board[move].score_dir[0];
    for (int i = 1; i < 4; i++)
        input->score_board[move].score = MAX(input->score_board[move].score,
            input->score_board[move].score_dir[i]);

    int player = input->board[move] > 0 ? 0 : 1;
    input->score[player] += input->score_board[move].score - old_score;

}


void update_score_board_move_pos_dir(t_game_state *input,int move, int dir){
    int x;
    int y;
    int dx;
    int dy;

    move_to_coords(move, &x, &y);
    set_dir(dir, &dx, &dy);

    if (input->board[move] != 0)
        dir_score(input, move, dir);
    else {
        input->score_board[move].start[dir] = 0;
        input->score_board[move].size[dir] = 0;
        input->score_board[move].open_init[dir] = 0;
        input->score_board[move].open_end[dir] = 0;
        input->score_board[move].score_dir[dir] = 0;
        input->score_board[move].score = 0;
    }

    for (int side = -1; side <= 1; side += 2) {
        int row = x + side * dx;
        int col = y + side * dy;
        char color = get_pos(input, row, col);

        if (color != 'B' && color != 'W')
            continue;
        while (get_pos(input, row, col) == color) {
            dir_score(input, coors_to_move(row, col), dir);
            row += side * dx;
            col += side * dy;
        }
    }
}


void update_score_board_move_pos(t_game_state *input,int move, int old_piece){
    if (old_piece != 0 && input->board[move] == 0) {
        int player = old_piece > 0 ? 0 : 1;
        input->score[player] -= input->score_board[move].score;
        input->score_board[move].score = 0;
    }

    for (int i = 0; i < 4; i++){
        update_score_board_move_pos_dir(input,move,i);
    }
}
