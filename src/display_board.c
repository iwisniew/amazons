#include <stdio.h>
#include "board.h"

void display_board(board_str board) {
    int row, col;
    printf("Displaying board information\n");
    printf("Board size: %d rows x %d cols\n", board.m, board.n);
    if (board.player_number > 0) printf("Players:\n");
    for (int player_count = 1; player_count <= board.player_number; player_count++) {
        printf("id%d %s score:%d\n", board.players[player_count - 1].id, board.players[player_count - 1].name,
               board.players[player_count - 1].score);
    }
    printf("First digit - treasure \n");
    printf("Second digit - artifact \n");
    printf(". empty field \n");
    printf("X obstacle\n");
    printf("------------------------------\n");
    if (board.n > 9) {
        for (col = 0; col <= board.n; col++) {
            if (col == 0) {
                printf("   ");
            } else {
                if (col / 10 == 0) {
                    printf("    ");
                } else printf(" %d  ", col / 10);
            }
        }
        printf("\n");
    }
    for (row = 0; row <= board.m; row++) {
        for (col = 0; col <= board.n; col++) {
            if (row == 0) {
                if (col == 0) {
                    printf("   ");
                } else {
                    printf(" %d  ", col % 10);
                }
            } else {
                if (row == 1 & col == 0) {
                    printf("  +---");
                    for (int tmp = 0; tmp < board.n - 1; tmp++) {
                        printf("----");
                    }
                    printf("\n 1|");
                } else if (col == 0) {
                    if (row < 10) {
                        printf(" %d|", row);
                    } else {
                        printf("%d|", row);
                    }
                } else {
                    printf("%d",board.squares[col - 1][row - 1].treasure);
                    printf("%d",board.squares[col - 1][row - 1].artifact);
                    if (board.squares[col - 1][row - 1].occupation == 0) {
                        printf(". ");
                    } else if (board.squares[col - 1][row - 1].occupation == 9) {
                        printf("X ");
                    } else {
                        printf("%d ", board.squares[col - 1][row - 1].occupation);
                    }
                }
            }
        }
        printf("\n");
    }
    printf("\n");
}


