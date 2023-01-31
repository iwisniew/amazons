#ifndef MOVEMENT_H_INCLUDED
#define MOVEMENT_H_INCLUDED


//returns 1 if unoccupied, 0 if occupied or outside the board
//x y from screen 1..n 1..m
int check_if_unoccupied(board_str board, int x, int y);

//check if player can make any move
//returns 1 if can, 0 no moves
int has_player_valid_move(board_str board, int player_id);


//all players make 1 move
board_str make_round(board_str board);

#endif
