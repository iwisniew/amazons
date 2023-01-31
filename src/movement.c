#include <stdio.h>
#include "board.h"
#include "display_board.h"
#include "file_operations.h"
#include "generate_board.h"
#include "movement.h"
#include "auto_moves.h"
#include <string.h>


//for given coordinates of move checks if move can be done
// s-source
// d-destination
int no_obstacles_for_action(board_str board, int col_s, int col_d, int row_s, int row_d) {
    int inc_x, inc_y;
    if (col_d - col_s == 0) {
        inc_x = 0;
    } else if (col_d - col_s > 0) {
        inc_x = 1;
    } else inc_x = -1;
    if (row_d - row_s == 0) {
        inc_y = 0;
    } else if (row_d - row_s > 0) {
        inc_y = 1;
    } else inc_y = -1;

    int col = col_s;
    int row = row_s;
    if ((inc_x!=0 && inc_y!=0 && abs(row_d-row_s)==abs(col_d-col_s)) || inc_x==0 || inc_y==0){
        do {
            col = col + inc_x;
            row = row + inc_y;
            printf("x%d y%d o%d\n", col, row, board.squares[col - 1][row - 1].occupation);
            if (board.squares[col - 1][row - 1].occupation > 0) {
                printf("view is blocked\n");
                return 0;
            }
        } while (abs(col - col_d) > 0 || abs(row - row_d) > 0);
        printf("no obstacles for shot\n");
        return 1;
    } else return 0;
}

board_str shoot_arrow(board_str board, int col_s, int row_s) {
    int shoot_is_not_ok = 1;
    int col_d, row_d;
    display_board(board);
    do {
        printf("Where to shoot the arrow?\n");
        printf("Column:");
        scanf("%d", &col_d);
        printf("Row:");
        scanf("%d", &row_d);
        if ((col_d == col_s || row_d == row_s || abs(col_d - col_s) == abs(row_d - row_s)) &&
            is_on_board(board, col_d, col_s) &&
            board.squares[col_d - 1][row_d - 1].occupation == 0 &&
                no_obstacles_for_action(board, col_s, col_d, row_s, row_d)) {
            board.squares[col_d - 1][row_d - 1].occupation = 9;
            shoot_is_not_ok = 0;
        }
    } while (shoot_is_not_ok == 1);
    return board;
}

//check if player can make any move - used in interactive mode
//returns 1 if can, 0 no moves
int has_player_valid_move(board_str board, int player_id) {
    int player_exist = 0;
    //check if player exists on board
    for (int i = 1; i <= board.player_number; i++) {
        if (board.players[i - 1].id == player_id) {
            player_exist = 1;
            printf("Player %s exists\n", board.players[player_id - 1].name);
        }
    }
    if (player_exist == 1) {
        int row, col;
        int amazon_x[MAX_AMAZONS], amazon_y[MAX_AMAZONS], amazon_number = 0;
        //finding positions of players amazons
        for (row = 1; row <= board.m; row++) {
            for (col = 1; col <= board.n; col++) {
                if (board.squares[col - 1][row - 1].occupation == player_id) {
                    //coordinates from screen
                    amazon_x[amazon_number] = col;
                    amazon_y[amazon_number] = row;
                    amazon_number++;
                }
            }
        }
        printf("Player %s has %d amazons\n", board.players[player_id - 1].name, amazon_number);
        if (amazon_number == 0) {
            printf("ERROR - There are no players amazons");
            exit(-1);
        }
        //checking possible moves for each amazon
        for (int i = 0; i < amazon_number; i++) {
            if (check_if_unoccupied(board, amazon_x[i] - 1, amazon_y[i] - 1) ||
                check_if_unoccupied(board, amazon_x[i], amazon_y[i] - 1) ||
                check_if_unoccupied(board, amazon_x[i] + 1, amazon_y[i] - 1) ||
                check_if_unoccupied(board, amazon_x[i] - 1, amazon_y[i]) ||
                check_if_unoccupied(board, amazon_x[i] + 1, amazon_y[i]) ||
                check_if_unoccupied(board, amazon_x[i] - 1, amazon_y[i] + 1) ||
                check_if_unoccupied(board, amazon_x[i], amazon_y[i] + 1) ||
                check_if_unoccupied(board, amazon_x[i] + 1, amazon_y[i] + 1)) {
                return 1;
            }
        }
        return 0;

    } else {
        printf("ERROR - There is no such player");
        exit(-1);
    }
}

board_str shoot_spear(board_str board, int col_s, int row_s) {
    int shoot_is_not_ok = 1;
    int col_d, row_d;
    display_board(board);
    do {
        printf("Where to shoot the spear?\n");
        printf("Column:");
        scanf("%d", &col_d);
        printf("Row:");
        scanf("%d", &row_d);
        if ((col_d == col_s || row_d == row_s || abs(col_d - col_s) == abs(row_d - row_s)) &&
            is_on_board(board, col_d, col_s) && board.squares[col_d - 1][row_d - 1].occupation == 0) {
            board.squares[col_d - 1][row_d - 1].occupation = 9;
            shoot_is_not_ok = 0;
        }
    } while (shoot_is_not_ok == 1);
    return board;
}

//player make 1 move
board_str make_move(board_str board, int player_id) {
    int row_s, row_d, col_s, col_d;
    int correct_action;
    display_board(board);
    printf("Player %s id%d\n", board.players[player_id - 1].name, board.players[player_id - 1].id);
    if (board.amazon_number > 1) {
        do {
            printf("Choose amazon to move\n");
            printf("Column:");
            scanf("%d", &col_s);
            printf("Row:");
            scanf("%d", &row_s);
        } while (board.squares[col_s - 1][row_s - 1].occupation != player_id);
    } else {
        //only 1 amazon
        amazons_str amazons=get_amazons(board,player_id);
        col_s=amazons.x[0];
        row_s=amazons.y[0];
    }
    do {
        printf("Enter destination coordinates\n");
        printf("Column:");
        scanf("%d", &col_d);
        printf("Row:");
        scanf("%d", &row_d);
        correct_action=no_obstacles_for_action(board, col_s,col_d,row_s,row_d);
        if (correct_action==0){
            printf("Wrong coordinates.\n");
        }
    } while (correct_action==0);

    board.squares[col_s - 1][row_s - 1].occupation = 0;
    board.squares[col_d - 1][row_d - 1].occupation = player_id;

    printf("You have found treasure: %d points.\n", board.squares[col_d - 1][row_d - 1].treasure);
    board.players[player_id - 1].score += board.squares[col_d - 1][row_d - 1].treasure;
    printf("Your score is now: %d points.\n", board.players[player_id - 1].score);
    board.squares[col_d - 1][row_d - 1].treasure = 0;

    int artifact = board.squares[col_d - 1][row_d - 1].artifact;
    board.squares[col_d - 1][row_d - 1].artifact = 0;

    switch (artifact) {
        case 1:
            printf("You have found a horse. You make additional move.\n");
            board = shoot_arrow(board, col_d, row_d);
            board = make_move(board, player_id);
            break;
        case 2:
            printf("You have found a broken arrow. You do not shoot.\n");
            break;
        case 3:
            printf("You have found a spear. You shoot through obstacles.\n");
            board = shoot_spear(board, col_d, row_d);
            break;
        default:
            printf("You have found no artifact.\n");
            board = shoot_arrow(board, col_d, row_d);
            break;
    }
    return board;

}

//all players make 1 move
board_str make_round(board_str board) {
    int i;
    int game_over = 1;
    for (i = 1; i <= board.player_number; i++) {
        printf("Player %s\n", board.players[i - 1].name);
        if (has_player_valid_move(board, i)) {
            game_over = 0;
            board = make_move(board, board.players[i - 1].id);
        } else {
            printf("Player %s has no valid moves\n", board.players[i - 1].name);
        }
    }
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




