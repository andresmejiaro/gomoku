#include "gomoku.h"


static int is_stone(char cell){
    return cell == 'B' || cell == 'W';
}



int is_machine_move(const t_game_state *input,
    int x ,int y){
    
    if (get_pos(input,x,y) != '0')
        return 0;
    if (x == BOARD_SIDE / 2 && y == BOARD_SIDE / 2)
        return 1;


    int c1 = is_stone(get_pos(input,x+1,y));
    int c2 = is_stone(get_pos(input,x+1,y+1));
    int c3 = is_stone(get_pos(input,x+1,y-1));
    int c4 = is_stone(get_pos(input,x-1,y));
    int c5 = is_stone(get_pos(input,x-1,y+1));
    int c6 = is_stone(get_pos(input,x-1,y-1));
    int c7 = is_stone(get_pos(input,x,y+1));
    int c8 = is_stone(get_pos(input,x,y-1));

    return c1 || c2 || c3 || c4 || c5 || c6 || c7 || c8 ;    

}


void update_available_machine_move_pos(t_game_state *input,
    int move){
    int x = move / BOARD_SIDE;
    int y = move % BOARD_SIDE;
    
    char cell_content = get_pos(input,x,y); 

    if (cell_content == 'W' || cell_content == 'B'){
        if (get_pos(input,x+1,y) == '0')
            set_av(input,x+1,y,1);
        if (get_pos(input,x+1,y+1) == '0')
            set_av(input,x+1,y+1,1);
        if (get_pos(input,x+1,y-1) == '0')
            set_av(input,x+1,y-1,1);
        if (get_pos(input,x-1,y) == '0')
            set_av(input,x-1,y,1);
        if (get_pos(input,x-1,y+1) == '0')
            set_av(input,x-1,y+1,1);
        if (get_pos(input,x-1,y-1) == '0')
            set_av(input,x-1,y-1,1);
        if (get_pos(input,x,y+1) == '0')
            set_av(input,x,y+1,1);
        if (get_pos(input,x,y-1) == '0')
            set_av(input,x,y-1,1);  
        set_av(input,x,y,0);
    }

    if (cell_content == '0'){
        set_av(input,x+1,y+1,is_machine_move(input,x+1,y+1));
        set_av(input,x+1,y-1,is_machine_move(input,x+1,y-1));
        set_av(input,x+1,y,is_machine_move(input,x+1,y));
        set_av(input,x-1,y+1,is_machine_move(input,x-1,y+1));
        set_av(input,x-1,y-1,is_machine_move(input,x-1,y-1));
        set_av(input,x-1,y,is_machine_move(input,x-1,y));
        set_av(input,x,y+1,is_machine_move(input,x,y+1));
        set_av(input,x,y-1,is_machine_move(input,x,y-1));
        set_av(input,x,y,is_machine_move(input,x,y));
    }

}
