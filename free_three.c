#include "gomoku.h"

int is_free_three(t_game_state *input, int move, int dir){
    int dx, dy, x, y,counter;
    char color[9], player_color;


    move_to_coords(move,&x,&y);
    set_dir(dir,&dx,&dy);

    color[4] = get_pos(input,x,y);
    if (color[4] == 'X')
        return 0;
    
    for (int i = -4 ; i<=4; i++){
        if (i == 0)
            continue;
        color[4+i] = get_pos(input,x+i*dx,y+i*dy); 
    }

    //inverse as the actual turn because the stone is played before
    player_color = input->turn %2 ? 'B' : 'W';
    for(int i=0; i<4; i++){
        //Extremes
        if (color[i] != '0')
            continue;
        if (color[i+5] != '0')
            continue;
        //Interior only empty + same color
        counter = 0;
        for(int j=1; j<=4; j++){

            if (color[i+j] != player_color && color[i+j] != '0'){
                counter = 0;
                break;
            }
            if (color[i+j] == player_color){
                counter++;
            }
        }
        if (counter == 3){
            return 1;
        }
    }
    return 0;
}






static int is_double_three_internal(t_game_state *input, int move, int crossing){
    
    
    int dir_question = -1;
    
    for (int dir = 0; dir < 4; dir ++){
        if(is_free_three(input,move,dir)){
            if (dir_question != -1){
 
                return 1;
                
            }
            dir_question = dir;
       } 
    }
    if (dir_question == -1){
        
        return 0;
    }

    if (crossing){
        //inverse as the actual turn because the stone is played before

        int player_color = input->turn %2 ? 'B' : 'W';
        int x,y,dx,dy;
        move_to_coords(move,&x,&y);
        set_dir(dir_question,&dx,&dy);    
        
        for (int offset= -4; offset <= 4; offset++){
            char achar = get_pos(input,x+offset*dx,y+offset*dy);
            if (offset !=0 &&  (achar == player_color || achar == '0')){
                if (is_double_three_internal(input,coors_to_move(x+offset*dx, y+offset*dy),0)){
                    return 1;
                }
            }

        }
    }
    return 0; 
}



int is_double_three(t_game_state *input, int move, int crossing){
    if (get_pos_move(input, move) != '0'){
        return 0;
    }
    t_move_undo undo = play_move_number(input, move);
    int to_return = 0;
    if (undo.captured_count == 0)
        to_return = is_double_three_internal(input, move, crossing);
     
    undo_move (input, undo);
    return to_return;
}
