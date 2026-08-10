#ifndef GOMOKU_H
 #define GOMOKU_H
 #include <string.h>
 #include <strings.h>
 #include <inttypes.h>
 #include <stdio.h>
 #include <stdlib.h>
 #include <sys/param.h>

  #define BOARD_CELLS 361
  #define BOARD_SIDE 19
  #define MAX_MACHINE_MOVES 312

 typedef struct scored_move
 {
   int move;
   int score;
 } t_scored_move;

 typedef struct score_place
 {
  int score;
  int score_dir[4];
  int start[4];
  int size[4];
  int closed_init[4];
  int closed_end[4];
 } t_score_place;

 typedef struct struct_game_state
 {
    int8_t   board[BOARD_CELLS];

    uint16_t   turn;
    uint8_t   captures[2];

    t_score_place score_board[BOARD_CELLS];
    int32_t   score[2];
    
    int8_t          available_machine_moves[BOARD_CELLS];
    t_scored_move   machine_moves_scores[MAX_MACHINE_MOVES];


 } t_game_state;

 
void print_screen(const t_game_state *game);
void play_move(const t_game_state *input, t_game_state *output, int row, int col);
void play_move_number(const t_game_state *input, t_game_state *output, int move_number);
char get_pos(const t_game_state *input, int row, int col);
int five_in_a_row(const t_game_state *input, int row, int col, int row_dir, int col_dir);
char has_won(const t_game_state *input);
t_scored_move next_move(const t_game_state *input, int last_move);
int is_terminal(const t_game_state * input);
u_int16_t evaluate_window(const t_game_state *input, int row, int col, int row_dir, int col_dir);
int evaluate(const t_game_state *input);
int negapruning(int depth,
                const t_game_state *input,
                int alpha,
                int beta,
                int root,
                int *out_move);
int is_move_valid(const t_game_state *input, int move);

#endif