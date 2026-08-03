#ifndef GOMOKU_H
 #define GOMOKU_H
 #include <string.h>
 #include <strings.h>
 #include <inttypes.h>
 #include <stdio.h>
 #include <stdlib.h>
 #include <sys/param.h>

 typedef struct struct_game_state
 {
    uint64_t white[6];
    uint64_t black[6];
    uint8_t  turn;
    uint8_t  captures[2];
 } t_game_state;

 typedef struct scored_move
 {
   int move;
   int score;
 } t_scored_move;
 
void print_screen(const t_game_state *game);
void play_move(const t_game_state *input, t_game_state *output, int row, int col);
void play_move_number(const t_game_state *input, t_game_state *output, int move_number);
char get_pos(const t_game_state *input, int row, int col);
int five_in_a_row(const t_game_state *input, int row, int col, int row_dir, int col_dir);
char has_won(const t_game_state *input);
int next_move(const t_game_state *input, int last_move);
int is_terminal(const t_game_state * input);
u_int16_t evaluate_window(const t_game_state *input, int row, int col, int row_dir, int col_dir);
int evaluate(const t_game_state *input);
int negapruning(int depth,
                const t_game_state *input,
                int alpha,
                int beta,
                int root,
                int *out_move);
 
#endif