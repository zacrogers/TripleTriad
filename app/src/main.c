#include <stdio.h>
#include <stdint.h>

#include "../inc/tt.h"
#include "../inc/test.h"

// The hand the player starts with
uint8_t player_start_hand[5] = {1, 2, 3, 4, 5};
uint8_t enemy_start_hand[5] = {6, 7, 8, 9, 10};

#define N_MOVES 4

uint8_t enemy_moves[N_MOVES][3] = {
    {0, 0, 0},
    {0, 0, 1},
    {0, 0, 2},
    {0, 1, 0}
};
uint8_t player_moves[N_MOVES][3] = {
    {0, 1, 1},
    {0, 1, 2},
    {0, 2, 0},
    {0, 2, 1}
};



int main(void)
{
    run_tests(player_start_hand, enemy_start_hand, player_moves, enemy_moves);
    return 0;
}


int tt_main(void)
{
    tt_init(); // Initialise the board

    // const struct tt_card* user_cards = tt_get_player_cards(TT_PLAYER_A);

    printf("Select your hand...\n");
    tt_set_player_hand(TT_PLAYER_A, player_start_hand);
    tt_set_player_hand(TT_PLAYER_B, player_start_hand);

    tt_print_hand(TT_PLAYER_A);
    tt_print_hand(TT_PLAYER_B);

    bool running = true;
    enum tt_player_type curr_player = TT_PLAYER_A;

    while(running)
    {
        curr_player = tt_curr_player_turn();
        printf("Player %d turn\n", (uint8_t)curr_player);


        if(TT_PLAYER_A == curr_player)
        {
            int hand_pos = 0, board_x = 0, board_y = 0;
            uint8_t s = 0;
            const uint8_t* curr_hand = tt_get_player_hand(curr_player, &s);

            // get player input
            printf("Select (card, x, y):\n");
            scanf("%d %d %d", &hand_pos, &board_x, &board_y);
            uint8_t selected_card_idx = curr_hand[hand_pos];

            printf("(%d %d %d, (%d))\n", hand_pos, board_x, board_y, (board_x* board_y));
            tt_place_card(curr_player, hand_pos, board_x, board_y);

            printf("Placed: %s\n", tt_get_card_name(selected_card_idx));
        }
        else
        {

        }

        // if(tt_curr_player_turn() == TT_PLAYER_A)
        // {
        // }
        // else
        // {
        //     tt_place_card(TT_PLAYER_B, 2, 1, 1);
        // }

        // Print the current game state
        tt_print_hand(TT_PLAYER_A);
        tt_print_hand(TT_PLAYER_B);
        tt_print_board();

        running = tt_update_game();
    }

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