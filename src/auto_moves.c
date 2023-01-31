#include <stdio.h>
#include "board.h"
#include "display_board.h"
#include "movement.h"
#include "auto_moves.h"
#include <string.h>
#include <time.h>

//functions used for autonomous mode (mostly)

//checks if coordinates are correct
int is_on_board(board_str board, int x, int y) {
    if (x > 0 && x <= board.n && y > 0 && y <= board.m) {
        //printf("coordinates are on board\n");
        return 1;
    } else {
        //printf("coordinates are outside the board\n");
        return 0;
    }
}

//returns 1 if unoccupied, 0 if occupied or outside the board
//x y from screen 1..n 1..m
int check_if_unoccupied(board_str board, int x, int y) {
    if (x > 0 && x <= board.n && y > 0 && y <= board.m) {
        if (board.squares[x - 1][y - 1].occupation == 0) {
            //printf("unoccupied\n");
            return 1;
        } else {
            //printf("occupied\n");
            return 0;
        }
    } else {
        //printf("outside\n");
        return 0;
    }
}

//temporary: shooting to where i came from
//next try: random shot
board_str auto_shoot_arrow(board_str board, int col_d, int row_d) {
    board.squares[col_d - 1][row_d - 1].occupation = 9;
    display_board(board);
    /*int a, b;

    //srand(time(0));

    do {
    a = rand() % board.n + 1;
    b = rand() % board.m + 1;
    } while(check_if_unoccupied(board, a, b));

    col_d = a;      //assigns random generated address to destination coords of the arrow
    row_d = b;

    board.squares[col_d - 1][row_d - 1].occupation = 9;
    display_board(board); */
    return board;
}

//returns player_id for given player name
int get_player_id(board_str board, char name[15]) {
    int player_id = 0;
    for (int i = 1; i <= board.player_number; i++) {
        printf("Name:%s\n", board.players[i].name);
        if (strcmp(board.players[i - 1].name, name) == 0) {
            player_id = i;
        }
    }
    return player_id;
}

//find players' amazons
amazons_str get_amazons(board_str board, int player_id) {
    int row, col;
    amazons_str amazons;
    amazons.number = 0;
    if (player_id > 0)
        for (row = 1; row <= board.m; row++) {
            for (col = 1; col <= board.n; col++) {
                if (board.squares[col - 1][row - 1].occupation == player_id) {
                    amazons.x[amazons.number] = col;
                    amazons.y[amazons.number] = row;
                    amazons.number++;
                }
            }
        }
    return amazons;
}

//find horses
amazons_str get_horses(board_str board) {
    int row, col;
    amazons_str horses;
    horses.number = 0;
    for (row = 1; row <= board.m; row++) {
        for (col = 1; col <= board.n; col++) {
            //printf("- x=%d y=%d n=%d\n", col, row, board.squares[col - 1][row - 1].artifact);
            if (board.squares[col - 1][row - 1].artifact == 1) {
                horses.x[horses.number] = col;
                horses.y[horses.number] = row;
                horses.number++;

            }
        }
    }
    return horses;

}

//return all possible moves in one direction
//moves from (x,y) in direction (inc_x,inc_y)
moves_str get_moves_direction(board_str board, moves_str moves, int x, int y, int inc_x, int inc_y, int amazon_id) {
    int on_board;
    do {
        x = x + inc_x;
        y = y + inc_y;
        //printf("x%d y%d\n", x, y);
        //printf("Moves counter:%d\n", moves.counter);
        on_board = is_on_board(board, x, y);
        if (on_board == 1 && board.squares[x - 1][y - 1].occupation == 0) {
            //printf("x%d y%d\n", x, y);
            moves.x[moves.counter] = x;
            moves.y[moves.counter] = y;
            moves.amazon_id[moves.counter] = amazon_id;
            moves.score[moves.counter] = 0;
            moves.counter++;
            //printf("Moves counter:%d\n", moves.counter);
        }
    } while (board.squares[x - 1][y - 1].occupation == 0 && on_board == 1);
    return moves;
}

// checks how many unoccupied fields in neighbourhood - returns 0 to 8
int get_moves_count(board_str board, int x, int y) {
    int count = 0;
    count = count + check_if_unoccupied(board, x - 1, y - 1)
            + check_if_unoccupied(board, x, y - 1)
            + check_if_unoccupied(board, x + 1, y - 1)
            + check_if_unoccupied(board, x - 1, y)
            + check_if_unoccupied(board, x + 1, y)
            + check_if_unoccupied(board, x - 1, y - 1)
            + check_if_unoccupied(board, x, y - 1)
            + check_if_unoccupied(board, x + 1, y - 1);
    return count;
}

//return all moves in all directions for the amazon
moves_str get_moves_for_amazon(board_str board, moves_str moves, amazons_str amazons, int amazon_id) {
    int x = amazons.x[amazon_id];
    int y = amazons.y[amazon_id];
    int inc_x = -1;
    int inc_y = -1;
    moves = get_moves_direction(board, moves, x, y, inc_x, inc_y, amazon_id);
    inc_x = 0;
    inc_y = -1;
    moves = get_moves_direction(board, moves, x, y, inc_x, inc_y, amazon_id);
    inc_x = 1;
    inc_y = -1;
    moves = get_moves_direction(board, moves, x, y, inc_x, inc_y, amazon_id);
    inc_x = -1;
    inc_y = 0;
    moves = get_moves_direction(board, moves, x, y, inc_x, inc_y, amazon_id);
    inc_x = 1;
    inc_y = 0;
    moves = get_moves_direction(board, moves, x, y, inc_x, inc_y, amazon_id);
    inc_x = -1;
    inc_y = 1;
    moves = get_moves_direction(board, moves, x, y, inc_x, inc_y, amazon_id);
    inc_x = 0;
    inc_y = 1;
    moves = get_moves_direction(board, moves, x, y, inc_x, inc_y, amazon_id);
    inc_x = 1;
    inc_y = 1;
    moves = get_moves_direction(board, moves, x, y, inc_x, inc_y, amazon_id);
    return moves;
}

//returns all possible moves for all amazons
moves_str get_moves(board_str board, int player_id) {
    moves_str moves;
    moves.counter = 0;
    amazons_str amazons;
    amazons = get_amazons(board, player_id);
    for (int i = 0; i < amazons.number; i++) {
        //find moves for every amazon
        moves = get_moves_for_amazon(board, moves, amazons, i);
    }
    printf("Possible moves: %d\n", moves.counter);
    return moves;
}

// returns the best move
// calculates score for every possible move
//TODO improvements to calculation score
int best_move(board_str board, moves_str moves) {
    int col, row;
    int c;
    for (int i = 0; i < moves.counter; i++) {
        moves.score[i] = 0;             //temporary score for choosing the best move
        col = moves.x[i];
        row = moves.y[i];
        moves.score[i] += board.squares[col - 1][row - 1].treasure * 10;
        if (board.squares[col - 1][row - 1].artifact == 1) moves.score[i] = (moves.score[i] + 11) * 6; //
        c = get_moves_count(board, col, row);
        //printf("X:%d Y:%d count:%d\n", col, row, c);
        if (c == 0) moves.score[i] = moves.score[i] / 10;
        if (c == 1) moves.score[i] = moves.score[i] / 5;
        moves.score[i] = moves.score[i] + c;
        //printf("X:%d Y:%d c:%d score:%d\n", col, row, c, moves.score[i]);
    }
    int max_score = 0;
    int best_move;
    for (int i = 0; i < moves.counter; i++) {
        if (moves.score[i] >= max_score) {
            max_score = moves.score[i];
            best_move = i;
        }
    }
    printf("Max score: %d\n", max_score);
    return best_move;
}

// automatic move for player_id
board_str auto_move(board_str board, int player_id) {
    amazons_str amazons;
    printf("Player number:%d\n", board.player_number);
    printf("Player id:%d\n", player_id);
    amazons = get_amazons(board, player_id);
    printf("Amazon number:%d\n", amazons.number);
    moves_str moves = get_moves(board, player_id);
    if (moves.counter == 0) {
        board.players[player_id - 1].game_over = 1;
        exit(1);
    }
    int best_move_id = best_move(board, moves);

    int col_d = moves.x[best_move_id];
    int row_d = moves.y[best_move_id];
    int amazon_id = moves.amazon_id[best_move_id];

    int col_s = amazons.x[amazon_id];
    int row_s = amazons.y[amazon_id];

    printf("X:%d Y:%d > X:%d Y:%d\n", col_s, row_s, col_d, row_d);

    //checking possible moves for amazons old
    /*
    int possible_move_x[MAX_AMAZONS * 8], possible_move_y[MAX_AMAZONS * 8];
    int possible_moves_number = 0;
    int amazon_to_move[MAX_AMAZONS * 8];
    for (int i = 0; i < amazons.number; i++) {

        if (check_if_unoccupied(board, amazons.x[i] - 1, amazons.y[i] - 1)) {
            possible_move_x[possible_moves_number] = amazons.x[i] - 1;
            possible_move_y[possible_moves_number] = amazons.y[i] - 1;
            amazon_to_move[possible_moves_number] = i;
            possible_moves_number++;
        }
        if (check_if_unoccupied(board, amazons.x[i], amazons.y[i] - 1)) {
            possible_move_x[possible_moves_number] = amazons.x[i];
            possible_move_y[possible_moves_number] = amazons.y[i] - 1;
            amazon_to_move[possible_moves_number] = i;
            possible_moves_number++;
        }
        if (check_if_unoccupied(board, amazons.x[i] + 1, amazons.y[i] - 1)) {
            possible_move_x[possible_moves_number] = amazons.x[i] + 1;
            possible_move_y[possible_moves_number] = amazons.y[i] - 1;
            amazon_to_move[possible_moves_number] = i;
            possible_moves_number++;
        }
        if (check_if_unoccupied(board, amazons.x[i] - 1, amazons.y[i])) {
            possible_move_x[possible_moves_number] = amazons.x[i] - 1;
            possible_move_y[possible_moves_number] = amazons.y[i];
            amazon_to_move[possible_moves_number] = i;
            possible_moves_number++;
        }
        if (check_if_unoccupied(board, amazons.x[i] + 1, amazons.y[i])) {
            possible_move_x[possible_moves_number] = amazons.x[i] + 1;
            possible_move_y[possible_moves_number] = amazons.y[i];
            amazon_to_move[possible_moves_number] = i;
            possible_moves_number++;
        }
        if (check_if_unoccupied(board, amazons.x[i] - 1, amazons.y[i] + 1)) {
            possible_move_x[possible_moves_number] = amazons.x[i] - 1;
            possible_move_y[possible_moves_number] = amazons.y[i] + 1;
            amazon_to_move[possible_moves_number] = i;
            possible_moves_number++;
        }
        if (check_if_unoccupied(board, amazons.x[i], amazons.y[i] + 1)) {
            possible_move_x[possible_moves_number] = amazons.x[i];
            possible_move_y[possible_moves_number] = amazons.y[i] + 1;
            amazon_to_move[possible_moves_number] = i;
            possible_moves_number++;
        }
        if (check_if_unoccupied(board, amazons.x[i] + 1, amazons.y[i] + 1)) {
            possible_move_x[possible_moves_number] = amazons.x[i] + 1;
            possible_move_y[possible_moves_number] = amazons.y[i] + 1;
            amazon_to_move[possible_moves_number] = i;
            possible_moves_number++;
        }
    }
    printf("Possible moves number:%d\n", possible_moves_number);
    if (possible_moves_number==0) {
        board.players[player_id-1].game_over=1;
        return board;
    }
    //choosing random move
    int move_number = rand() % (possible_moves_number);
    printf("Chosen move:%d\n", move_number);*/

    //making move
    board.squares[col_s - 1][row_s - 1].occupation = 0;
    board.squares[col_d - 1][row_d - 1].occupation = player_id;

    //temp for shooting
//    int col_s=amazons.x[amazon_to_move[move_number]];
//    int row_s=amazons.y[amazon_to_move[move_number]];

//    int col_d, row_d;
//    col_d=possible_move_x[move_number];
//    row_d=possible_move_y[move_number];

    //TODO treasure,artifacts,shooting;
    printf("Player %s found treasure: %d points.\n", board.players[player_id - 1].name,
           board.squares[col_d - 1][row_d - 1].treasure);
    board.players[player_id - 1].score += board.squares[col_d - 1][row_d - 1].treasure;
    printf("Player %s has score: %d points.\n", board.players[player_id - 1].name, board.players[player_id - 1].score);
    board.squares[col_d - 1][row_d - 1].treasure = 0;

    int artifact = board.squares[col_d - 1][row_d - 1].artifact;
    board.squares[col_d - 1][row_d - 1].artifact = 0;

    switch (artifact) {
        case 1:
            printf("You have found a horse. You make additional move.\n");
            board = auto_shoot_arrow(board, col_s, row_s);
            board = auto_move(board, player_id);
            break;
        case 2:
            printf("You have found a broken arrow. You do not shoot.\n");
            break;
        case 3:
            printf("You have found a spear. You shoot through obstacles.\n");
            board = auto_shoot_arrow(board, col_s, row_s);
            break;
        default:
            printf("You have found no artifact.\n");
            board = auto_shoot_arrow(board, col_s, row_s);
            break;
    }
    return board;
}

//all players
board_str make_auto_round(board_str board) {
    int i;
    int game_over = 1;
    for (i = 1; i <= board.player_number; i++) {
        printf("Player %s\n", board.players[i - 1].name);
        if (has_player_valid_move(board, i)) {
            game_over = 0;
            board = auto_move(board, board.players[i - 1].id);
        } else {
            printf("Player %s has no valid moves\n", board.players[i - 1].name);
        }
    }
    display_board(board);
    if (game_over == 0) {
        return board;
    } else {
        printf("###########################\n");
        printf("GAME OVER\n");
        printf("###########################\n");
        display_board(board);
        exit(-1);
    }
}
