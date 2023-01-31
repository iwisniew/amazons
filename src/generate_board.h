#ifndef GENERATE_BOARD_H_INCLUDED
#define GENERATE_BOARD_H_INCLUDED

board_str generate_board();
 //function generating random board with given dimensions

board_str add_players(board_str board);

board_str place_amazons(board_str board);

board_str place_amazon(board_str board, int amazons_number);

#endif // GENERATE_BOARD_H_INCLUDED