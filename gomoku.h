#ifndef GOMOKU_H
 #define GOMOKU_H
 
 typedef struct game_func
 {
    int (*evaluate)(void *state);
    int (*is_terminal)(void *state);
    void *(*play_move)(void *state, void *move);
    void **(*get_moves)(void *state, int *n_moves);
    void (*free_moves)(void **moves, int *n_moves);
 } t_game_func;
 
 
#endif