#include <stdio.h>
#include <stdint.h>

#include "../inc/tt.h"
#include "../inc/test.h"


void run_tests(
    uint8_t player_start_hand[5],
    uint8_t enemy_start_hand[5],
    uint8_t player_moves[N_MOVES][3],
    uint8_t enemy_moves[N_MOVES][3]
)
{
    tt_init(); // Initialise the board

    printf("Select your hand...\n");
    tt_set_player_hand(TT_PLAYER_A, player_start_hand);
    tt_set_player_hand(TT_PLAYER_B, enemy_start_hand);

    tt_print_hand(TT_PLAYER_A);
    tt_print_hand(TT_PLAYER_B);

    for(uint8_t move = 0; move < N_MOVES; ++move)
    {
        for(uint8_t turn = 0; turn < 2; ++ turn)
        {
            if(TT_PLAYER_A == turn)
            {
                int hand_pos = player_moves[move][0], board_x = player_moves[move][1], board_y = player_moves[move][2];
                uint8_t s = 0;
                const uint8_t* curr_hand = tt_get_player_hand(turn, &s);
                uint8_t selected_card_idx = curr_hand[hand_pos];

                if(tt_place_card(turn, hand_pos, board_x, board_y))
                {
                    printf("Placed: %s\n", tt_get_card_name(selected_card_idx));
                }
                else
                {
                    printf("Failed to place card\n");
                }
            }
            if(TT_PLAYER_B == turn)
            {
                int hand_pos = enemy_moves[move][0], board_x = enemy_moves[move][1], board_y = enemy_moves[move][2];
                uint8_t s = 0;
                const uint8_t* curr_hand = tt_get_player_hand(turn, &s);
                uint8_t selected_card_idx = curr_hand[hand_pos];

                if(tt_place_card(turn, hand_pos, board_x, board_y))
                {
                    printf("Placed: %s\n", tt_get_card_name(selected_card_idx));
                }
                else
                {
                    printf("Failed to place card\n");
                }
            }

            tt_print_hand(TT_PLAYER_A);
            tt_print_hand(TT_PLAYER_B);
            tt_print_board();

            if(tt_update_game())
            {
                printf("Updated\n");
            }
            else
            {
                printf("Not updated\n");
            }
        }
    }
}