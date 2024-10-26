#ifndef TT_H_
#define TT_H_


#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>


enum tt_constants {
    TTC_N_ROWS         = 3,
    TTC_N_COLS         = 3,
    TTC_MAX_HAND_SIZE  = 5,
    TTC_CARD_LIST_SIZE = 11,
    TTC_EMPTY_CARD     = 0xFF
};

enum tt_error {
    TT_Err_Ok,
    TT_Err_Range_High,
    TT_Err_Range_Low,
    TT_Err_Space_Occupied,
    TT_Err_Hand_Empty,
    TT_Err_Count
};

enum tt_game_state {
    StateSelectHand,
    StateStartMatch,
    StatePlayerTurn,
    StateOpponentTurn,
    StateMatchFinished
};

enum tt_player_type {
    TT_PLAYER_A    = 0,
    TT_PLAYER_B    = 1,
    TT_PLAYER_NONE = 2
};

enum tt_elem_type {
    TT_Elem_Earth,
    TT_Elem_Fire,
    TT_Elem_Water,
    TT_Elem_Poison,
    TT_Elem_Holy,
    TT_Elem_Lightning,
    TT_Elem_Wind,
    TT_Elem_Ice,
    TT_Elem_None
};

enum tt_rules {
    TT_R_Open,
    TT_R_Same,
    TT_R_SameWall,
    TT_R_SuddenDeath,
    TT_R_Random,
    TT_R_Plus,
    TT_R_Combo,
    TT_R_Elemental,
    TT_R_Retry,
    TT_R_None
};


enum tt_card_pos {
    TT_Pos_Up,
    TT_Pos_Right,
    TT_Pos_Down,
    TT_Pos_Left,
    TT_Pos_Count
};


struct tt_card {
    uint8_t           level;
    const char*       name;
    uint8_t           values[TT_Pos_Count]; // U, R, D, L
    enum tt_elem_type elem;
};


struct tt_player {
    uint8_t hand[TTC_MAX_HAND_SIZE];
    uint8_t hand_size;
    uint8_t points;
};


struct tt_board {
    struct tt_player    player[2];
    enum tt_player_type player_turn;
    uint8_t             cards[TTC_N_ROWS * TTC_N_COLS];
    enum tt_player_type card_owners[TTC_N_ROWS * TTC_N_COLS];
    enum tt_game_state  state;
    uint8_t last_card_added;
    uint8_t           last_neighbours[TT_Pos_Count];
    bool check_pending;
};

struct tt_score {
    uint8_t a, b;
};


void                  tt_init(void);
const uint8_t*        tt_board_state(void);
enum tt_player_type   tt_curr_player_turn(void);
struct tt_score       tt_get_score(void);
bool                  tt_game_over(void);
bool                  tt_update_game(void);
const char*           tt_get_card_name(uint8_t card_index);
const struct tt_card* tt_get_player_cards(enum tt_player_type player);
void                  tt_set_player_hand(enum tt_player_type player, const uint8_t idxs[TTC_MAX_HAND_SIZE]);
const uint8_t*        tt_get_player_hand(enum tt_player_type player, uint8_t* size);
bool                  tt_place_card(enum tt_player_type player, uint8_t hand_idx, uint8_t board_x, uint8_t board_y);


void tt_print_hand(enum tt_player_type player);
void tt_print_board(void);



#endif /* TT_H_ */