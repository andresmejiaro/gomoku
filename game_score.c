#include "gomoku.h"


#define CAPTURE_IMPORTANCE_EMPTY 1


void check_space_around(t_game_state *input, int move, t_update_run *update){
    int x, y, dx, dy;
    move_to_coords(move, &x,&y);
    
    for(int ray = 0; ray < 8; ray ++){
        set_ray(ray,&dx,&dy);
        if (get_pos(input,x+dx,y+dy) == '0'){
            queue_update_free_space(coors_to_move(x+dx,y+dy), update);
        } 
    }
}




void queue_update_stone(t_game_state *input, int move, t_update_run *update)
{
    (update->board)[move] = 1;
    check_space_around(input, move, update);
}


void queue_update_free_space(int move, t_update_run *update)
{
    (update->board)[move] = -1;
 
}

void queue_local_space(t_game_state *input, int move, t_update_run *update){
    int x, y, dx, dy;
    char content;
    move_to_coords(move, &x,&y);
    
    for(int ray = 0; ray < 8; ray ++){
        set_ray(ray,&dx,&dy);
        
        for(int dis = 0; dis < 5; dis++){
            if(dis == 0 && ray != 0)
                continue;
            content = get_pos(input,x+dis*dx,y+dis*dy);
            if (content == 'X')
                continue;         
            if (content == '0'){
                queue_update_free_space(coors_to_move(x+dis*dx,y+dis*dy), update);
            } else {
                queue_update_stone(input, coors_to_move(x+dis*dx,y+dis*dy), update); 
            }
        }
    }
}


void update_all(t_game_state *input, t_update_run *update){
    for (int i = 0; i< BOARD_CELLS; i++){
        if ((update->board)[i] == 1){
            update_score_board_move_pos(input,i,update);
        }
    }

    for (int i = 0; i< BOARD_CELLS; i++){
        if ((update->board)[i] == -1){
            update_score_board_move_pos(input,i,update);
        }
    }

}




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


void set_ray(int ray, int *dx, int *dy){
    if (ray == RAY_Xpos){
        *dx = 1;
        *dy = 0;
    }
    if (ray == RAY_Xneg){
        *dx = -1;
        *dy = 0;
    }
    if (ray == RAY_Ypos){
        *dx = 0;
        *dy = 1;
    }
    if (ray == RAY_Yneg){
        *dx = 0;
        *dy = -1;
    }
    if (ray == RAY_XYpos){
        *dx = 1;
        *dy = 1;
    }
    if (ray == RAY_XYneg){
        *dx = -1;
        *dy = -1;
    }
    if (ray == RAY_XnYpos){
        *dx = 1;
        *dy = -1;
    }
    if (ray == RAY_XnYneg){
        *dx = -1;
        *dy = 1;
    }
}


int ray_to_dir(int ray){
    if (ray == RAY_Xpos){
        return DIRECTION_X;
    }
    if (ray == RAY_Xneg){
        return DIRECTION_X;
    }
    if (ray == RAY_Ypos){
        return DIRECTION_Y;
    }
    if (ray == RAY_Yneg){
        return DIRECTION_Y;
    }
    if (ray == RAY_XYpos){
        return DIRECTION_XY;
    }
    if (ray == RAY_XYneg){
        return DIRECTION_XY;
    }
    if (ray == RAY_XnYpos){
        return DIRECTION_XnY;
    }
    if (ray == RAY_XnYneg){
        return DIRECTION_XnY;
    }
    return -1;
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


static void cleanup_stone_properties(t_score_place *scorep){
    scorep->score = 0;
    scorep->capture_potential_tot = 0;
    for(int i = 0; i <4; i++){
        scorep->score_dir[i] = 0;
        scorep->start[i] = 0;
        scorep->size[i] = 0;
        scorep->open_init[i] = 0;
        scorep->open_end[i] = 0;
        scorep->capture_potential[2*i] = 0;
        scorep->capture_potential[2*i+1] = 0;
        
    }
}


static void cleanup_candidate_properties(t_score_place *scorep){
    scorep->candidate_score = 0;
    for(int i = 0; i <8; i++){
        scorep->candidate_capture[i] = 0;
        scorep->candidate_dir_score[i] = 0;        
    }
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
    cleanup_candidate_properties(&(input->score_board[move]));
}


static void update_chain_score(t_game_state *input, int start, int dir,
    int length, int open_s, int open_e, int score, t_update_run *update){
    
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
        check_space_around(input, move, update);
    }
    
}




static int check_ray_capture(t_game_state *input, int move, int ray){
    //this is for empty cells
    char s1, s2, s3;
    int dx, dy,row,col;
    
    move_to_coords(move, &row,&col);
    set_ray(ray,&dx,&dy);
    s1 = get_pos(input, row + dx, col +dy);
    s2 = get_pos(input, row + 2*dx, col + 2*dy);
    s3 = get_pos(input, row + 3*dx, col +3*dy);
    
    if (!(s1 == 'B' || s1 == 'W' ))
    return 0;
    if (!(s2 == 'B' || s2 == 'W' ))
    return 0;
    if (!(s3 == 'B' || s3 == 'W' ))
    return 0;
    if ((s1 == s2) && (s2 != s3)){
        if(s3 == 'B')
        return 2;
        return -2;    
    }
    return 0;
}

static int check_ray_capture2(t_game_state *input, int move, int ray){
    //this is for occupied cells
    char s0, s1, s2, s3;
    int dx, dy,row,col;
    
    move_to_coords(move, &row,&col);
    set_ray(ray,&dx,&dy);
    s0 = get_pos(input, row, col);
    s1 = get_pos(input, row + dx, col +dy);
    s2 = get_pos(input, row + 2*dx, col + 2*dy);
    s3 = get_pos(input, row + 3*dx, col +3*dy);
    
    if (s3 != '0' || s1 == '0')
    return 0;
    if ((s1 == s2) && (s2 != s0) && s1 != 'X'){
        return 1;    
    }
    return 0;
}

static void update_capture_score_stone(t_game_state *input, int move){
    
    int temp1;

    (input->score_board)[move].capture_potential_tot = 0;
    
    for (int ray = 0; ray < 8; ray ++){
        temp1 = check_ray_capture2(input,move, ray);
        (input->score_board)[move].capture_potential[ray] = temp1;
        (input->score_board)[move].capture_potential_tot += temp1;

    }

}

static int adjacent_score(const t_game_state *input, int move, int dir){
    int pri = 0, sec = 0, mult = 1;
    
    for(int i = 0; i<4; i++){
        if ((input->score_board)[move].score_dir[i] > pri){
            sec = pri;
            pri = (input->score_board)[move].score_dir[i];
        }
        else if ((input->score_board)[move].score_dir[i] > sec) 
            sec = (input->score_board)[move].score_dir[i];
    }

    if ((input->score_board)[move].score_dir[dir] == sec && sec > 0)
        mult = 2;
    if ((input->score_board)[move].score_dir[dir] == pri && pri > 0)
        mult = 3;

    return mult * (input->score_board)[move].score;
}


static int check_ray_score(t_game_state *input, int move, int ray){
    int dir = ray_to_dir(ray);
    int dx, dy,row,col, n_move;
    int to_ret;
    
    move_to_coords(move, &row,&col);
    set_ray(ray,&dx,&dy);
    
    char W = get_pos(input,row + dx, col + dy);
    if (!(W == 'X' || W == '0')){
        n_move = coors_to_move(row + dx, col + dy);
        to_ret = adjacent_score(input,n_move,dir);
        
        return to_ret;
    }
    return 0;
}


static void update_empty_placement(t_game_state *input, int move){
    int temp, temp2;

    (input->score_board)[move].candidate_score = 0;    

    for(int ray = 0; ray < 8; ray++){
    
        temp2 = check_ray_capture(input,move,ray);
        (input->score_board)[move].candidate_capture[ray] = temp2;
        if (temp2 < 0)
            temp2 *= -1;
        temp = check_ray_score(input,move,ray);
        (input->score_board)[move].candidate_dir_score[ray] = temp;
        (input->score_board)[move].candidate_score += temp + CAPTURE_IMPORTANCE_EMPTY * temp2;

    } 
}


void update_chain(t_game_state *input,int move, int dir, t_update_run *update){
    int start,length, open_s, open_e, score, space;
    start = locate_chain_start(input, move, dir);
    length = calculate_chain_length(input, start, dir);
    open_s = open_start(input, start, dir);
    open_e = open_end(input, start, dir, length);
    space = does_chain_have_space(input, start, dir, length);
    score = chain_score(open_s,open_e,length, space);
    update_chain_score(input,start,dir,length,open_s,open_e,score, update);
}


void update_score_board_move_pos(t_game_state *input,int move, t_update_run *update){
    if ((input->board)[move]!= 0){
        for (int i = 0; i < 4; i++){
            update_chain(input,move,i, update);
        }
        update_capture_score_stone(input,move);
        return ;
    }
    cleanup_stone_properties(&input->score_board[move]);
    update_empty_placement(input,move);
}
