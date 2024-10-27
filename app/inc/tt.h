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
    TTC_EMPTY_CARD_ID  = 0xFE
};

enum tt_error {
    TT_Err_Ok,
    TT_Err_Hand_Range,
    TT_Err_Master_Range,
    TT_Err_Cell_Occupied,
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
    const uint8_t           level;
    const char*             name;
    const uint8_t           values[TT_Pos_Count]; // U, R, D, L
    const enum tt_elem_type elem;
};


struct tt_hand {
    uint8_t values[TTC_MAX_HAND_SIZE];
    uint8_t size;
};


struct tt_card_cell {
    enum tt_player_type owner;
    uint8_t             master_id;
    enum tt_elem_type   elem;
};


struct tt_score {
    uint8_t a, b;
};


struct tt_board {
    struct tt_hand      hand[2];
    enum tt_player_type player_turn;
    struct tt_card_cell cards[TTC_N_ROWS * TTC_N_COLS];
    enum tt_game_state  state;
    struct tt_score     score;
};

void                   tt_board_init(enum tt_player_type start_player);
const struct tt_board* tt_board_state(void);
const char*            tt_board_state_json(void);
bool                   tt_update_game(void);

const char*            tt_get_card_name(uint8_t card_index);
const struct tt_card*  tt_get_card(uint8_t card_index);
bool                   tt_place_card(enum tt_player_type player, uint8_t hand_idx, uint8_t board_x, uint8_t board_y);

const struct tt_card*  tt_get_player_cards(enum tt_player_type player);
void                   tt_set_hand(enum tt_player_type player, const uint8_t idxs[TTC_MAX_HAND_SIZE]);
const uint8_t*         tt_get_hand(enum tt_player_type player, uint8_t* size);


void tt_print_hand(enum tt_player_type player);
void tt_print_board(void);


#endif /* TT_H_ */