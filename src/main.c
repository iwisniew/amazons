#include <stdio.h>
#include "board.h"
#include "display_board.h"
#include "file_operations.h"
#include "generate_board.h"
#include "movement.h"
#include "auto_moves.h"
#include <string.h>

//important: please check board.h for board structure
/* TODO
 * improvements to autonomous mode
 * testing
 */
void amazons_manual()
{
    printf("\n\nHere You can find Amazons' manual.\n");
    printf("In order to play game properly, You must act in particular order.\n");
    printf("\n\n1) If You want to start an entirely new game You must generate board first.\n");
    printf("Then You can just simply add players, place amazons and make moves.\n\n");
    printf("2) For running an auto mode You must add players manually and assign number of amazons to them.\n");
    printf("Then, run auto move option from game menu.\n\n");
    printf("3) Playing on last generated board is as easy as it can be.\n");
    printf("Just skip generating new board part from point 1.\n\n");
    printf("4) If You want to play on previously generated board use option 1 in game menu\n\n");
}

int main(int argc, char *argv[]) {
    int counter;
    if (argc > 1) {
        //print arguments from command line
        for (counter = 0; counter < argc; counter++) printf("\nargv[%d]: %s", counter, argv[counter]);
        printf("\n");

        //if argument name - display players name end exit the program
        for (counter = 1; counter < argc; counter++) {
            if (strcmp(argv[counter], "name") == 0) {
                printf("Player: %s\n", PLAYER_NAME);
                return 0;
            }
        }

        //phase placement
        if (strcmp(argv[1], "phase=placement") == 0) {
            board_str my_board;
            int amazon_number;
            char input_file[20];
            char output_file[20];
            sscanf(argv[2], "amazons=%d", &amazon_number);
            printf("Amazon number = %d\n", amazon_number);
            sscanf(argv[3], "%s", input_file);
            printf("Input file = %s\n", input_file);
            sscanf(argv[4], "%s", output_file);
            printf("Output file = %s\n", output_file);
            printf("Reading from file\n");
            my_board = read_board_from_file(input_file);
            int player_id = get_player_id(my_board, PLAYER_NAME);
            amazons_str amazons = get_amazons(my_board, player_id);
            if (amazons.number == amazon_number) {
                return 1;
            } else {
                my_board = place_amazon(my_board, amazon_number);
                write_board_to_file(output_file, my_board);
                return 0;
            }
        }

        //phase movement
        if (strcmp(argv[1], "phase=movement") == 0) {
            board_str my_board;
            char input_file[20];
            char output_file[20];
            sscanf(argv[2], "%s", input_file);
            printf("Input file = %s\n", input_file);
            sscanf(argv[3], "%s", output_file);
            printf("Output file = %s\n", output_file);
            printf("Reading from file\n");
            my_board = read_board_from_file(input_file);
            int player_id = get_player_id(my_board, PLAYER_NAME);
            my_board = auto_move(my_board, player_id);
            write_board_to_file(output_file, my_board);
            //display_board(my_board);
            if (my_board.players[player_id - 1].game_over == 1) return 1;
            else return 0;
        }
    }

    //if no command argument print menu
    int option = 9;
    board_str my_board;
    my_board = read_board_from_file("board.txt");
    display_board(my_board);
    //exit(5);

    do {
        printf("MENU epfu20W-103-F-amazons\n");
        printf("1 - Load board from file\n");
        printf("2 - Save board to file\n");
        printf("3 - Generate new board\n");
        printf("4 - Add players\n");
        printf("5 - Place amazons\n");
        printf("6 - Display board\n");
        printf("7 - Make move - 1 round\n");
        printf("8 - Auto move - 1 round\n");
        printf("9 - Start a new game\n");
        printf("0 - Exit\n");
        printf("10 - Help\n");
        printf("-------------------------\n");
        printf("Choose option:");
        scanf("%d", &option);
        switch (option) {
            case 1:
                my_board = read_board_from_file("board.txt");
                break;
            case 2:
                write_board_to_file("board.txt", my_board);
                break;
            case 3:
                my_board = generate_board();
                break;
            case 4:
                my_board = add_players(my_board);
                break;
            case 5:
                my_board = place_amazons(my_board);
                break;
            case 6:
                display_board(my_board);
                break;
            case 7:
                my_board = make_round(my_board);
                break;
            case 8:
                my_board = make_auto_round(my_board);
                break;
            case 9:
                my_board = generate_board();
                my_board = add_players(my_board);
                display_board(my_board);
                my_board = place_amazons(my_board);
                while(1){
                    my_board = make_round(my_board);
                }
                break;
            case 10:
                amazons_manual();
                break;
        }
    } while (option != 0);
}


