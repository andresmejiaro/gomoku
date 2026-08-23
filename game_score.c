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
    int x, y;
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


static int calculate_chain_length(t_game_state *input, int start, int dir){
    int dx, dy,row,col;
    set_dir(dir,&dx,&dy);
    move_to_coords(start,&row,&col);

    char color = get_pos(input,row,col);
    int steps = 0;
    while(get_pos(input, row + steps*dx, col + steps*dy) == color){
        steps++;
    }
    steps --;
    return steps + 1;
}


static int open_start(t_game_state *input, int start, int dir){
    int dx, dy,row,col;
    char color, color1, color2;
    set_dir(dir,&dx,&dy);
    move_to_coords(start,&row,&col);

    color = get_pos(input,row,col);
    color1 = get_pos(input,row - dx ,col - dy);
    color2 = get_pos(input,row - 2*dx ,col - 2*dy);
    //if correct color 1 cannot be the same as color not checking
    if (color1 != '0')
        return 0;
    if (color2 == color)
        return 3;
    return 2;
}


static int open_end(t_game_state *input, int start, int dir, int length){
    int dx, dy,row,col;
    char color, color1, color2;
    set_dir(dir,&dx,&dy);
    move_to_coords(start,&row,&col);

    color = get_pos(input,row,col);
    color1 = get_pos(input,row +(length)* dx ,col +(length)* dy);
    color2 = get_pos(input,row + (length + 1)*dx ,col + (length + 1)*dy);
    //if correct color 1 cannot be the same as color not checking
    if (color1 != '0')
        return 0;
    if (color2 == color)
        return 3;
    return 2;
}


static int chain_score(int open_s, int open_e, int length, int space){
    int cond1, cond2;
    if (space == 0)   
        return 0;
    if (length >= 5)
        return 10000;
    cond1 = open_s > 0;
    cond2 = open_e > 0;
    if (length == 4 && (cond1 != cond2)) //patch half open 4 better than open 3
        length = 5;
    return (open_e + open_s)*length;

}


static int locate_chain_start(t_game_state *input, int move, int dir){
    int dx, dy,row,col;
    set_dir(dir,&dx,&dy);
    move_to_coords(move,&row,&col);

    char color = get_pos(input,row,col);
    int steps = 0;
    while(get_pos(input, row - steps*dx, col - steps*dy) == color){
        steps++;
    }
    steps --;
    return coors_to_move(row - steps*dx, col - steps*dy);
}

static int does_chain_have_space(t_game_state *input, int start, int dir,
    int length){
    
    if (length >= 5)
        return 1;
    
    int dx, dy,row,col,count_before,count_after;
    char color,check;
    set_dir(dir,&dx,&dy);
    move_to_coords(start,&row,&col);
    count_before = 1;
    color = get_pos(input, row, col);
    check = get_pos(input, row - count_before*dx, col - count_before*dy); 
    while(check == '0' || check == color){
        count_before++;
        check = get_pos(input, row - count_before*dx, col - count_before*dy);
    }
    count_before --;
    if (count_before + length >= 5 )
        return 1;
    count_after = length;
    check = get_pos(input, row + count_after*dx, col + count_after*dy);
    while(check == '0' || check == color){
        count_after++;
        check = get_pos(input, row + count_after*dx, col + count_after*dy);
    }
    if (count_before + count_after >= 5 )
        return 1;    
    return 0;
}


static void update_stone_score(t_game_state *input, int move){
    int old_score, best, s_best,pv, dir_score;
    old_score = (input->score_board)[move].score;
    best = MAX((input->score_board)[move].score_dir[0],(input->score_board)[move].score_dir[1]);
    s_best = MIN((input->score_board)[move].score_dir[0],(input->score_board)[move].score_dir[1]);
    for (int i = 2; i < 4; i++){
        dir_score = (input->score_board)[move].score_dir[i];
        if (dir_score > best){
            s_best = best;
            best = dir_score;
        }
        else if (dir_score > s_best)
            s_best = dir_score;
    }
    (input->score_board)[move].score = best + (s_best >> 1);
    pv = (input->board)[move];
    if (pv == 1)
        pv = 0;
    else if (pv == -1)
        pv = 1;
    (input->score)[pv] += (input->score_board)[move].score - old_score;
}


static void update_chain_score(t_game_state *input, int start, int dir,
    int length, int open_s, int open_e, int score){
    
    int dx, dy,row,col,move;
    set_dir(dir,&dx,&dy);
    move_to_coords(start,&row,&col);


    for (int i = 0; i < length; i++){
        move = coors_to_move(row +i*dx,col + i*dy);
        (input->score_board)[move].open_end[dir] = open_e;
        (input->score_board)[move].open_init[dir] = open_s;
        (input->score_board)[move].score_dir[dir] = score;
        (input->score_board)[move].start[dir] = start;
        (input->score_board)[move].size[dir] = length;
        update_stone_score(input,move);
    }
    
}


static void update_capture_score_stone(t_game_state *input, int move){
    (void)input;
    (void)move;
}


static void update_empty_placement(t_game_state *input, int move){
    (void)input;
    (void)move;
}


void update_chain(t_game_state *input,int move, int dir){
    int start,length, open_s, open_e, score, space;
    start = locate_chain_start(input, move, dir);
    length = calculate_chain_length(input, start, dir);
    open_s = open_start(input, start, dir);
    open_e = open_end(input, start, dir, length);
    space = does_chain_have_space(input, start, dir, length);
    score = chain_score(open_s,open_e,length, space);
    update_chain_score(input,start,dir,length,open_s,open_e,score);
}


void update_score_board_move_pos(t_game_state *input,int move){
    if ((input->board)[move]!= 0){
        for (int i = 0; i < 4; i++){
            update_chain(input,move,i);
        }
        update_capture_score_stone(input,move);
        return ;
    }
    update_empty_placement(input,move);
}
