#ifndef AUTO_MOVES_H_INCLUDED
#define AUTO_MOVES_H_INCLUDED
#include "board.h"
#include <stdio.h>
#include <stdlib.h>
#include "display_board.h"
#include <string.h>

int is_on_board(board_str board, int x, int y);

moves_str get_moves_for_amazon(board_str board, moves_str moves, amazons_str amazons, int amazon_id);

amazons_str get_amazons(board_str board, int player_id);

int check_if_unoccupied(board_str board, int x, int y);

amazons_str get_horses(board_str board);
//make 1 automated move
board_str make_auto_round(board_str board);

board_str auto_move(board_str board, int player_id);

int get_player_id(board_str board, char name[15]);

moves_str get_moves(board_str board, int player_id);


#endif
