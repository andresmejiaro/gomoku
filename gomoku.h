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
  
  //when direction is 4
  #define DIRECTION_X 0
  #define DIRECTION_Y 1
  #define DIRECTION_XY 2
  #define DIRECTION_XnY 3

  //when direction is 8
  #define RAY_Xpos 0
  #define RAY_Xneg 1
  #define RAY_Ypos 2
  #define RAY_Yneg 3
  #define RAY_XYpos 4
  #define RAY_XYneg 5
  #define RAY_XnYpos 6
  #define RAY_XnYneg 7
  #define MAX_MOVES_CONSIDERED 5

 typedef struct scored_move
 {
   int move;
   int score;
 } t_scored_move;

 typedef struct score_place
 {
  //stone invariants
  int score;
  int score_dir[4];
  int start[4];
  int size[4];
  int open_init[4];
  int open_end[4];
  int capture_potential[8];
  int capture_potential_tot;

  //canditate invariants
  int candidate_score;
  int candidate_capture[8];
  int candidate_dir_score[8]; 

 } t_score_place;

 typedef struct struct_game_state
 {
    int8_t   board[BOARD_CELLS];

    uint16_t   turn;
    uint8_t   captures[2];

    t_score_place score_board[BOARD_CELLS];
    int32_t   score[2];
    int32_t   capture_potential[2];
 
 } t_game_state;
 
 typedef struct struct_move_undo
 {
    uint16_t  move;

    uint8_t   captured_count;
    uint16_t  captured_positions[MAX_CAPTURES];


 } t_move_undo;
 typedef struct struct_update_run
 {
   int8_t   board[BOARD_CELLS]; //1 update cell -1 update empty space
 } t_update_run;


void print_screen(const t_game_state *game);
t_move_undo play_move(t_game_state *input, int row, int col);
void undo_move(t_game_state *input, t_move_undo undo);
void captures(t_game_state *input,int move,t_move_undo *to_return, t_update_run *update);
void update_score_board_move_pos(t_game_state *input, int move, t_update_run *update);
void queue_update_stone(t_game_state *input, int move, t_update_run *update);
void queue_update_free_space(int move, t_update_run *update);
void queue_local_space(t_game_state *input, int move, t_update_run *update);
void update_all(t_game_state *input, t_update_run *update);
void update_score(t_game_state *input);
t_move_undo play_move_number(t_game_state *input, int move_number);
char get_pos(const t_game_state *input, int row, int col);
char get_pos_move(const t_game_state *input, int move);
int five_in_a_row(const t_game_state *input, int row, int col, int row_dir, int col_dir);
char has_won(const t_game_state *input);
int is_terminal(const t_game_state * input);
u_int16_t evaluate_window(const t_game_state *input, int row, int col, int row_dir, int col_dir);
int evaluate(const t_game_state *input);
int negapruning(int depth,
                t_game_state *input,
                int alpha,
                int beta,
                int root,
                int *out_move,
                t_scored_move *previous_order);
int is_move_valid(t_game_state *input, int move);
int coors_to_move(int row, int col);
void move_to_coords(int move, int *row, int *col);
void set_dir(int dir, int *dx, int *dy);
void set_ray(int ray, int *dx, int *dy);
int comp_moves(const void *a, const void *b);
void inherit_ordening(t_scored_move *move,
                      const t_scored_move *previous_order,
                      t_game_state *input);
void create_ordening(t_scored_move *move, const t_game_state *input);
void order_moves(t_scored_move *move);
int is_double_three(t_game_state *input, int move, int crossing);

#endif
