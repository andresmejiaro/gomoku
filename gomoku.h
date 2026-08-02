#ifndef GOMOKU_H
 #define GOMOKU_H
 #include <strings.h>
 #include <inttypes.h>
 #include <stdio.h>
 #include <stdlib.h>
 #include <sys/param.h>

 typedef struct game_func
 {
    int (*evaluate)(void *state);
    int (*is_terminal)(void *state);
    void *(*play_move)(void *state, void *move);
    void **(*get_moves)(void *state, int *n_moves);
    void (*free_moves)(void **moves, int *n_moves);
 } t_game_func;
 
 typedef struct struct_game_state
 {
    uint64_t white[6];
    uint64_t black[6];
    uint8_t  turn;
    uint8_t  captures[2];
 } t_game_state;
 void print_screen(t_game_state *game);

 
#endif