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


int dir_score(t_game_state *input,int move, int dir){
    int score, x, y;
    int dx, dy;
    set_dir(dir,&dx,&dy);  
    move_to_coords(move,&x,&y);
    char color = get_pos(input,x,y);
    
    if (color == '0')
        return 0;
    
    
    int start_steps = 0;
    while (get_pos(input, x-start_steps*dx, y-start_steps*dy) == color)
        start_steps++;
    
    (input->score_board)[move].start[dir] = coors_to_move(x-(start_steps-1)*dx, y-(start_steps-1)*dy);


    int end_steps = 0;
    while (get_pos(input, x+end_steps*dx, y+end_steps*dy) == color)
        end_steps++;

    (input->score_board)[move].size[dir] = start_steps + end_steps - 1;

    if (get_pos(input, x+end_steps*dx, y+end_steps*dy) == '0')
        (input->score_board)[move].open_end[dir] = 1;
    else 
        (input->score_board)[move].open_end[dir] = 0;

    if (get_pos(input, x-start_steps*dx, y-start_steps*dy) == '0')
        (input->score_board)[move].open_init[dir] = 1;
    else 
        (input->score_board)[move].open_init[dir] = 0; 
        

    (input->score_board)[move].score_dir[dir] = ((input->score_board)[move].open_init[dir]+
        (input->score_board)[move].open_end[dir])*(end_steps+start_steps -1);    
    
    if (end_steps+start_steps -1>=5)
         (input->score_board)[move].score_dir[dir] = 10000;


    return (input->score_board)[move].score_dir[dir];
}


void update_score_board_move_pos(t_game_state *input,int move, int dir){
    int dx, dy;
    set_dir(dir,&dx,&dy);
    

}


void update_score_board_move_pos(t_game_state *input,int move){
    for (int i = 0; i < 4; i++){
        update_score_board_move_pos_dir(*input,move,i);
    }
}