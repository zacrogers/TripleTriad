#include <stdio.h>

#include "../inc/tt.h"

int main(void)
{
    tt_init(); // Initialise the board

    const struct tt_card* user_cards = tt_get_user_cards();

    uint8_t selected_idxs[7] = {4,2,2,4,5};
    tt_set_user_hand(selected_idxs);
    tt_print_hand();
    tt_print_board();

    tt_place_card(2, 1, 1);
    tt_print_hand();
    tt_print_board();

    // uint8_t curr_hand_size = 0;
    // const uint8_t* user_hand = tt_get_user_hand(&curr_hand_size);

    // handle hand selection
        // get card list
        // set selected cards
    // start game loop
        // handle player turn
        // handle opponent turn


    // for(int i = 0; i < curr_hand_size; ++i)
    // {
    //     printf("Name: %s\n", card_master_list[user_hand[i]].name);
    // }


    return 0;
}