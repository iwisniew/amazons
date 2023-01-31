#ifndef BOARD_H_INCLUDED
#define BOARD_H_INCLUDED
#define MAX_BOARD_SIZE 50
#define MIN_BOARD_SIZE 2
#define MAX_PLAYERS 8
#define MIN_PLAYERS 1
#define MAX_AMAZONS 5
#define MIN_AMAZONS 1
#define PLAYER_NAME "GROUP_F"

//squares - information on fields of board
//for counting squares (on screen) we use numbers from 1 to n/m visible
//in array counting is from 0 to n-1/m-1
typedef struct squares{
        int treasure; //the value of treasure (0-5)
        int artifact; //0-none, 1-horse, 2-broken arrow, 3-spear
        int occupation; //0-tile is unoccupied, 1-8 ID of the player, 9-arrow or spear
} squares_str;

//players info
typedef struct players {
        char name[15];
        int id;
        int score;
        int game_over;
} players_str;

//complete info on board
typedef struct board {
    int m, n; //board dimension n-cols, m-rows REMEMBER ABOUT -1
    int player_number; //number of players
    int amazon_number; //number of amazons per player
    squares_str squares[MAX_BOARD_SIZE][MAX_BOARD_SIZE];
    players_str players[MAX_PLAYERS];
} board_str;

//info about players' amazons position
typedef struct amazons {
    int x[MAX_BOARD_SIZE*MAX_BOARD_SIZE], y[MAX_BOARD_SIZE*MAX_BOARD_SIZE];
    int number;
} amazons_str;

//possible moves - best move with the highest score
typedef struct moves {
    int x[MAX_BOARD_SIZE*MAX_BOARD_SIZE];
    int y[MAX_BOARD_SIZE*MAX_BOARD_SIZE];
    int score[MAX_BOARD_SIZE*MAX_BOARD_SIZE];       //defines which move is the best
    int amazon_id[MAX_BOARD_SIZE*MAX_BOARD_SIZE];
    int counter;
} moves_str;

#endif
