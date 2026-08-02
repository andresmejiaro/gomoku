#include "gomoku.h"


void print_screen(const t_game_state *game){
    
    char  printable[19][20];
    int sector, residue, row, col;
    
    for(int i = 0; i < 19 ; i++){
        bzero(printable[i], sizeof(printable[i]));
    }
    

    for(int i = 0; i<19*19; i++){
        sector = i / 64;
        residue = i % 64;
        row = i / 19;
        col = i % 19;
        if(game ->white[sector] & (1ULL << residue)){
            printable[row][col] = 'W';
        }  else if(game ->black[sector] & (1ULL << residue)){
            printable[row][col] = 'B';
        } else {
            printable[row][col] = '0';
        }
    }
    for(int i = 0; i < 19 ; i++){
        printf("%2d: %s\n",i,printable[i]);
    }
}


void play_move(const t_game_state *input, t_game_state *output, int row, int col){
    memcpy(output, input, sizeof(t_game_state));

    int totpos, sector, residue;

    totpos = row*19 + col;
    sector = totpos / 64;
    residue = totpos % 64;

    if (output->turn % 2 == 1){
        output->white[sector] = (output->white[sector]) | (1ULL << residue);
    } else {
        output->black[sector] = (output->black[sector]) | (1ULL << residue);
    }

    (output->turn)++;
}

void play_move_number(const t_game_state *input, t_game_state *output, int move_number){
        int x = move_number / 19;
        int y = move_number % 19;
        play_move(input, output, x, y);
}

char get_pos(const t_game_state *input, int row, int col){
    
    int totpos, sector, residue;

    if (row < 0 || col < 0 || row > 18 || col > 18)
        return 'X';
    
    totpos = row*19 + col;
    sector = totpos / 64;
    residue = totpos % 64;

    int white_stone = (input->white[sector] >> residue) & 1;
    int black_stone = (input->black[sector] >> residue) & 1;

    if (white_stone && black_stone)
        return 'X';
    if (white_stone)
        return 'W';
    if (black_stone)
        return 'B';
    return '0';
}


int five_in_a_row(const t_game_state *input, int row, int col, int row_dir, int col_dir){
    
    char base;

    if (row < 0 || col < 0 || row > 18 || col > 18)
        return -1;
    if (row_dir*row_dir + col_dir*col_dir == 0)
        return -1; 
    if (row_dir*row_dir > 1 || col_dir*col_dir > 1)
        return -1;       
    base = get_pos(input, row, col);
    if (base == 'X')
        return -2;
    if (base == '0')
        return 0;
    for (int i=1; i<5; i++){

        if (get_pos(input, row + row_dir*i,col +col_dir*i) != base)
            return 0;

    }
    return 1;    
}

char has_won(const t_game_state *input){
    int horizontal, vertical, diag1, diag2;


    for(int row = 0; row < 19; row++){
        for(int col = 0; col < 19; col++){
            horizontal = five_in_a_row(input,row,col,1,0);
            vertical = five_in_a_row(input,row,col,0,1);
            diag1 = five_in_a_row(input,row,col,1,1);
            diag2 = five_in_a_row(input,row,col,1,-1);
            if (horizontal == -2 || vertical == -2 || diag1 == -2 || diag2 == -2)
                return 'X';
            if (horizontal || vertical || diag1 || diag2)
                return get_pos(input,row,col);
            
        }

    }
    return '0';
}

int next_move(const t_game_state *input, int last_move){
    int x,y;

    if (last_move < -1){
        return -2;
    }

    for(int i = last_move +1; i < 361; i++){
        x = i / 19;
        y = i % 19;
        if (get_pos(input,x,y) == '0')
            return i;
    }
    return -1;
}

int is_terminal(const t_game_state * input){
    if(has_won(input) != '0')
        return 1;
    if (next_move(input,-1) == -1)
        return 1;
    return 0;
}


u_int16_t evaluate_window(const t_game_state *input, int row, int col, int row_dir, int col_dir){
    
    u_int8_t b_count, w_count;

    b_count = 0;
    w_count = 0;
    
    // if (row < 0 || col < 0 || row > 18 || col > 18)
    //     return -1;
    // if (row_dir*row_dir + col_dir*col_dir == 0)
    //     return -1; 
    // if (row_dir*row_dir > 1 || col_dir*col_dir > 1)
    //     return -1;       
    
    for (int i=0; i<5; i++){

        if (get_pos(input, row + row_dir*i,col +col_dir*i) == 'W')
            w_count++;
        if (get_pos(input, row + row_dir*i,col +col_dir*i) == 'B')
            b_count++;

    }
    return ((u_int16_t)w_count << 8) + (u_int16_t)b_count;    
}

int evaluate(const t_game_state *input){
  
    u_int16_t horizontal, vertical, diag1, diag2;
    int b_count, w_count, sign;

    b_count = 0;
    w_count = 0;
    sign = 1;
    if (input->turn % 2)
        sign = -1;

    for(int row = 0; row < 19; row++){
        for(int col = 0; col < 19; col++){
            horizontal = evaluate_window(input,row,col,1,0);
            vertical = evaluate_window(input,row,col,0,1);
            diag1 = evaluate_window(input,row,col,1,1);
            diag2 = evaluate_window(input,row,col,1,-1);
            //Extracting the b info from the evaluate window
            b_count = MAX(b_count,(u_int8_t)(horizontal & 0xFFFF));
            b_count = MAX(b_count,(u_int8_t)(vertical & 0xFFFF));
            b_count = MAX(b_count,(u_int8_t)(diag1 & 0xFFFF));
            b_count = MAX(b_count,(u_int8_t)(diag2 & 0xFFFF));
            //Extracting the w info from the evaluate window
            w_count = MAX(w_count,(u_int8_t)(horizontal >> 8));
            w_count = MAX(w_count,(u_int8_t)(vertical >> 8));
            w_count = MAX(w_count,(u_int8_t)(diag1 >> 8));
            w_count = MAX(w_count,(u_int8_t)(diag2 >> 8));
        }

    }
    if (b_count > 4)
        b_count = 10000000;
    if (w_count > 4)
        w_count = 10000000;    

    

    return sign * (b_count - w_count);
}