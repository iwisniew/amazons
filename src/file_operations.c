#include <stdio.h>
#include <stdlib.h>
#include "board.h"

board_str read_board_from_file(char *filepath) {
    board_str board;
    int m = 0, n = 0;
    FILE *file;
    file = fopen(filepath, "r");
    if (file == NULL) {
        printf("Error! opening file");
        exit(2);
    }
    printf("Reading file information: %s\n", filepath);
    char line[100];
    //Reads size of the board
    if (fgets(line, 100, file) != NULL) {
        sscanf(line, "%d %d", &m, &n);
        if(m<MIN_BOARD_SIZE || m>MAX_BOARD_SIZE || n<MIN_BOARD_SIZE || n>MAX_BOARD_SIZE){
            printf("Wrong board size");
            exit(2);
        }
        printf("Board size: %d rows x %d columns\n", m, n);
    } else {
        printf("Unexpected end of file\n");
        exit(2);
    }
    board.m = m;
    board.n = n;

//Reads fields data
    int treasure;
    int artifact;
    int occupation;
    int tmp_field;
    int amazon_number = 0;
    for (int row = 1; row <= m; row++) {
        for (int col = 1; col <= n; col++) {
            if (fgets(line, 5, file) != NULL) {
                //printf("[%d:%d] %s\n", row, col, line);

                sscanf(line, "%d", &tmp_field);
                //printf("STEP1\n");
                //printf("(%d)\n", field);
                //printf ("%d\n", (field/100));
                //printf ("%d\n", ((field - ((field /100)*100)) /10));
                //printf ("%d\n", (field % 10));
                treasure = tmp_field / 100;
                if (treasure >= 0 && treasure <= 5) {
                    board.squares[col - 1][row - 1].treasure = treasure;
                } else {
                    printf("Wrong value of treasure\n");
                    exit(2);
                }
                artifact = (tmp_field - ((tmp_field / 100) * 100)) / 10;
                if (artifact >= 0 && artifact <= 3) {
                    board.squares[col - 1][row - 1].artifact = artifact;
                } else {
                    printf("Wrong value of artifact\n");
                    exit(2);
                }
                occupation = tmp_field % 10;
                if (occupation >= 0 && occupation <= 9) {
                    board.squares[col - 1][row - 1].occupation = occupation;
                } else {
                    printf("Wrong value of occupation\n");
                    exit(2);
                }

                //printf ("[%d][%d] %d %d %d \n",row, col, board.squares[col][row].treasure,board.squares[col][row].artifact,board.squares[col][row].occupation);
                //printf("STEP2 \n");
                //counts number of amazons (for player id1)
                if (board.squares[col - 1][row - 1].occupation == 1) amazon_number++;
            } else {
                printf("Unexpected end of file\n");
                exit(2);
            }
        }
    }
    int player_count = 0;

//Reads players data
    while (fgets(line, 1000, file) != NULL) {
        sscanf(line, "%s %d %d", &board.players[player_count].name, &board.players[player_count].id,
               &board.players[player_count].score);
        if(board.players[player_count].id<MIN_PLAYERS || board.players[player_count].id>MAX_PLAYERS){
            printf("Incorrect player id\n");
            exit(2);
        }
        printf("id%d %s score:%d\n", board.players[player_count].id, board.players[player_count].name,
               board.players[player_count].score);
        player_count++;
    }
    board.player_number = player_count;
    printf("Number of players: %d\n", board.player_number);
    board.amazon_number = amazon_number;
    printf("Number of amazons per player:%d\n", amazon_number);
    fflush(file);
    fclose(file);
    printf("------------------------------\n");
    return board;
}

void write_board_to_file(char *filepath, board_str board) {

    FILE *file = fopen(filepath, "w");
    int row, col;
    printf("Saving board to file %s\n", filepath);
    //write board size
    fprintf(file, "%d %d\n", board.m, board.n);
    //write fields data
    for (row = 1; row <= board.m; row++) {
        for (col = 1; col <= board.n; col++) {
            fprintf(file, "%d%d%d", board.squares[col - 1][row - 1].treasure, board.squares[col - 1][row - 1].artifact,
                    board.squares[col - 1][row - 1].occupation);
            if (col < board.n) fprintf(file, " ");
        }
        fprintf(file, "\n");
    }
    //write players data
    for (int player_count = 1; player_count <= board.player_number; player_count++) {
        fprintf(file, "%s %d %d\n", board.players[player_count - 1].name, board.players[player_count - 1].id,
                board.players[player_count - 1].score);
    }
    printf("------------------------------\n");
    fflush(file);
    fclose(file);
}


