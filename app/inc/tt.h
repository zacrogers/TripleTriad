#ifndef TT_H_
#define TT_H_


#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

#define BOARD_SIZE (3 * 3)
#define HAND_SIZE 5
#define N_VALUES_PER_CARD 4


enum tt_game_state {
    StateSelectHand,
    StateStartMatch,
    StatePlayerTurn,
    StateOpponentTurn,
    StateMatchFinished
};

enum tt_constants {
    TTC_
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


struct tt_card {
    uint8_t level;
    const char* name;
    uint8_t values[N_VALUES_PER_CARD]; // U, R, L, D
    enum tt_elem_type elem;
};


struct tt_player {
    uint8_t hand[HAND_SIZE];
    uint8_t hand_size;
};



struct tt_board {
    struct tt_player player_a;
    struct tt_player player_b;
    bool player_turn;
    uint8_t   cards[BOARD_SIZE];
    enum tt_game_state state;
};


extern const struct tt_card card_master_list[];


void                  tt_init(void);
void tt_board_state();
const struct tt_card* tt_get_user_cards(void);
void                  tt_set_user_hand(const uint8_t idxs[HAND_SIZE]);
const uint8_t*        tt_get_user_hand(uint8_t* size);
void                  tt_place_card(uint8_t card_idx, uint8_t board_x, uint8_t board_y);


void tt_print_hand(void);
void tt_print_board(void);

// {
//     printf("Starting Triple Triad!\n");
// }



#endif /* TT_H_ */