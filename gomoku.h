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
  #define MAX_CAPTURES 16
  #define DIRECTION_X 0
  #define DIRECTION_Y 1
  #define DIRECTION_XY 2
  #define DIRECTION_XnY 3

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
  int open_init[4];
  int open_end[4];
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
 
 typedef struct struct_move_undo
 {
    uint16_t  move;

    uint8_t   captured_count;
    uint16_t  captured_positions[MAX_CAPTURES];


 } t_move_undo;


 
void print_screen(const t_game_state *game);
void initialize_game_state(t_game_state *input);
t_move_undo play_move(t_game_state *input, int row, int col);
void undo_move(t_game_state *input, t_move_undo undo);
void captures(t_game_state *input,int move,t_move_undo *to_return);
void update_score_board_move_pos(t_game_state *input,int move);
void update_available_machine_move_pos(t_game_state *input,int move);
void update_machine_moves_scores_pos(t_game_state *input,int move);
void update_score(t_game_state *input);
t_move_undo play_move_number(t_game_state *input, int move_number);
char get_pos(const t_game_state *input, int row, int col);
int five_in_a_row(const t_game_state *input, int row, int col, int row_dir, int col_dir);
char has_won(const t_game_state *input);
t_scored_move next_move(const t_game_state *input, int last_move);
int is_terminal(const t_game_state * input);
u_int16_t evaluate_window(const t_game_state *input, int row, int col, int row_dir, int col_dir);
int evaluate(const t_game_state *input);
int negapruning(int depth,
                t_game_state *input,
                int alpha,
                int beta,
                int root,
                int *out_move);
int is_move_valid(const t_game_state *input, int move);
void set_pos(t_game_state *input, int row, int col, char color);
void set_av(t_game_state *input, int row, int col, int av);
int get_av(t_game_state *input, int row, int col);
int coors_to_move(int row, int col);
void move_to_coords(int move, int *row, int *col);
void set_dir(int dir, int *dx, int *dy);


#endif
