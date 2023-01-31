#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "board.h"
#include "display_board.h"
#include <string.h>
#include "auto_moves.h"

//function generating random board with given dimensions
board_str generate_board() {
    board_str board;
    int m, n;
    int row, col;
    do {
        printf("Please enter board size\n");
        printf("Number of rows (%d-%d):", MIN_BOARD_SIZE, MAX_BOARD_SIZE);
        scanf("%d", &m);
        printf("Number of columns (%d-%d):", MIN_BOARD_SIZE, MAX_BOARD_SIZE);
        scanf("%d", &n);
        if (m < MIN_BOARD_SIZE || m > MAX_BOARD_SIZE || n > MAX_BOARD_SIZE || n < MIN_BOARD_SIZE)
            printf("Wrong values. Possible board size is %d to %d.\n", MIN_BOARD_SIZE, MAX_BOARD_SIZE);
    } while (m < MIN_BOARD_SIZE || m > MAX_BOARD_SIZE || n > MAX_BOARD_SIZE || n < MIN_BOARD_SIZE);

    printf("Generating board\n");
    board.m = m;
    board.n = n;
    board.player_number = 0;
    board.amazon_number = 0;

    srand(time(0));
    for (row = 1; row <= m; row++) {
        for (col = 1; col <= n; col++) {
            int mnt = 0; //minimal treasure
            int mxt = 5;
            int mna = 0; //minimal artifact value
            int mxa = 3;
            int treasure = rand() % (mxt - mnt + 1) + mnt;        //using pseudorandom technique we generate the board
            int artifact = rand() % (mxa - mna + 1) + mna;
            board.squares[col - 1][row - 1].artifact = artifact;
            board.squares[col - 1][row - 1].treasure = treasure;
            board.squares[col - 1][row - 1].occupation = 0;
        }
    }
    printf("Finished generating board\n");
    return board;
}

//interactive mode - add players to board
board_str add_players(board_str board) {
    int players_number;
    do {
        printf("Please enter number of players (%d-%d):", MIN_PLAYERS, MAX_PLAYERS);
        scanf("%d", &players_number);
        if (players_number > MAX_PLAYERS || players_number < MIN_PLAYERS)
            printf("Wrong number. Possible number of players is %d to %d.\n", MIN_PLAYERS, MAX_PLAYERS);
    } while (players_number > MAX_PLAYERS || players_number < MIN_PLAYERS);
    int i;
    for (i = 1; i <= players_number; i++) {
        printf("Please enter name of player %d:", i);
        scanf("%s", &board.players[i - 1].name);
        board.players[i - 1].id = i;
        board.players[i - 1].score = 0;
        board.players[i - 1].game_over = 0;
    }
    board.player_number = i - 1;
    return board;
}

//in autonomous mode
board_str add_player(board_str board, char name[15]) {
    int i;
    //checks if player exist
    for (i = 1; i <= board.player_number; i++) {
        //if player exists
        if (strcmp(board.players[i - 1].name, name) == 0) return board;
    }

    board.player_number++;
    //exits if MAX_PLAYERS reached
    if (board.player_number > MAX_PLAYERS) {
        exit(-1);
    }
    //adds player
    board.players[i - 1].id = i;
    board.players[i - 1].score = 0;
    board.players[i - 1].game_over = 0;
    strcpy(board.players[i - 1].name, name);
    return board;
}

//in interactive mode
board_str place_amazons(board_str board) {
    int amazons, row, col, i, j;
    if (board.player_number > 0) {
        do {
            printf("Please enter number of amazons per player (%d-%d):", MIN_AMAZONS, MAX_AMAZONS);
            scanf("%d", &amazons);
            if (amazons < MIN_AMAZONS || amazons > MAX_AMAZONS) {
                printf("Wrong number. Possible number of amazons is %d to %d.\n", MIN_AMAZONS, MAX_AMAZONS);
            }
        } while (amazons < MIN_AMAZONS || amazons > MAX_AMAZONS);

        for (i = 1; i <= amazons; i++) {
            printf("-------------------------------------\n");
            printf("Round %d\n", i);
            for (j = 1; j <= board.player_number; j++) {
                int placement_ok = 0;
                do {
                    printf("Player %s\n", board.players[j - 1].name);
                    printf("Please enter coordinates for your amazon\n");
                    printf("Column:");
                    scanf("%d", &col);
                    printf("Row:");
                    scanf("%d", &row);
                    if (row < 1 || row > board.m || col < 1 || col > board.n) {
                        printf("Sorry, the coordinates are outside the board\n");
                    } else if (board.squares[col - 1][row - 1].occupation > 0) {
                        printf("Sorry, the field is already occupied\n");
                    } else if (board.squares[col - 1][row - 1].occupation == 0) {
                        board.squares[col - 1][row - 1].occupation = board.players[j - 1].id;
                        board.squares[col - 1][row - 1].treasure = 0;
                        board.squares[col - 1][row - 1].artifact = 0;
                        placement_ok = 1;
                    }
                } while (placement_ok == 0);
                display_board(board);
            }
        }
        board.amazon_number += amazons;
    } else {
        printf("No players defined for this board. Add players first.\n");
        exit(3);
    }
    return board;
}

//in autonomous mode
board_str place_amazon(board_str board, int amazons_number) {
    int my_player_id = 0;
    int amazon_x, amazon_y;
    printf("Board players number:%d\n", board.player_number);
    for (int n = 0; n < board.player_number; n++) {
        if (strcmp(board.players[n].name, PLAYER_NAME) == 0) {
            my_player_id = board.players[n].id;
        }
    }
    printf("My id:%d\n", my_player_id);
    if (my_player_id == 0) {
        strcpy(board.players[board.player_number].name, PLAYER_NAME);
        printf("Player name:%s\n", board.players[board.player_number].name);
        my_player_id = board.player_number + 1;
        board.players[board.player_number].id = my_player_id;
        board.players[board.player_number].score = 0;
        board.player_number++;
    }
    printf("My id:%d\n", my_player_id);

    amazons_str horses;
    horses = get_horses(board);

    moves_str horses_moves;
    horses_moves.counter = 0;
    int i, j;
    printf("Horses moves: %d\n", horses.number);
    for (i = 0; i < horses.number; i++) {
        //find moves for horses
        horses_moves = get_moves_for_amazon(board, horses_moves, horses, i);
    }
    printf("Horses moves: %d\n", horses_moves.counter);
    for (i = 0; i < horses_moves.counter; i++) {
        for (j = i + 1; j < horses_moves.counter; j++) {
            if ((horses_moves.x[i] == horses_moves.x[j]) && (horses_moves.y[i] == horses_moves.y[j])) {

                horses_moves.score[i]++;
                horses_moves.score[j]++;
                printf("Horses crossing: X=%d Y=%d Score=%d\n", horses_moves.x[i], horses_moves.y[i],
                       horses_moves.score[i]);
            }
        }
    }
    int max_score = 0;
    int best_move;
    for (i = 0; i < horses_moves.counter; i++) {
        if (horses_moves.score[i] >= max_score) {
            max_score = horses_moves.score[i];
            best_move = i;
        }
    }
    printf("Best placement: X=%d Y=%d Score=%d\n", horses_moves.x[best_move], horses_moves.y[best_move],
           horses_moves.score[best_move]);
    amazons_str my_amazons;
    my_amazons = get_amazons(board, my_player_id);

    printf("My amazons number:%d\n", my_amazons.number);
    if (my_amazons.number < amazons_number) {
        do {
            //amazon_x = rand() % (board.n) + 1;
            //amazon_y = rand() % (board.m) + 1;
            amazon_x = horses_moves.x[best_move];
            amazon_y = horses_moves.y[best_move];
            printf("Amazon X:%d Amazon Y:%d\n", amazon_x, amazon_y);
        } while (board.squares[amazon_x - 1][amazon_y - 1].occupation != 0);
        printf("Amazon X:%d Amazon Y:%d\n", amazon_x, amazon_y);
        board.squares[amazon_x - 1][amazon_y - 1].occupation = my_player_id;
        board.squares[amazon_x - 1][amazon_y - 1].treasure = 0;
        board.squares[amazon_x - 1][amazon_y - 1].artifact = 0;
    } else board.amazon_number = amazons_number;
    return board;
}







