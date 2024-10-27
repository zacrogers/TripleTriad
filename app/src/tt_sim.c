#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

// #include "SDL2/SDL.h"


#include "../inc/tt_sim.h"

static char input_data[350];

void sim_init(const char* moves_fname)
{
    FILE *file = fopen(moves_fname, "r");
    if (!file) {
        perror("Error opening file");
        return;
    }
}


void sim_run(void)
{

}



const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;
void run_simulation(
    uint8_t player_start_hand[TTC_MAX_HAND_SIZE],
    uint8_t enemy_start_hand[TTC_MAX_HAND_SIZE],
    uint8_t player_moves[N_MOVES][3],
    uint8_t enemy_moves[N_MOVES][3]
)
{
    // if (SDL_Init( SDL_INIT_VIDEO | SDL_INIT_TIMER ) < 0) {
    //     printf("SDL could not initialize! SDL_Error: %s", SDL_GetError());
    // } else {

    //     SDL_Window* window = SDL_CreateWindow("Hello, SDL2 on macOS",
    //                                     SDL_WINDOWPOS_CENTERED,
    //                                     SDL_WINDOWPOS_CENTERED,
    //                                     640, 480, 0);
    //     if (!window)
    //     {
    //         printf("error creating window: %s\n", SDL_GetError());
    //         SDL_Quit();
    //         // return 1;
    //     }

    //     // Keep the window open, in this case SDL_Delay(5000); statement won't work.
    //     bool running = true;
    //     while (running)
    //     {
    //         SDL_Event e;
    //         while (SDL_PollEvent(&e) != 0)
    //         {
    //             if (e.type == SDL_QUIT)
    //             {
    //                 running = false;
    //                 break;
    //             }
    //         }
    //     }
    // }

    tt_board_init(TT_PLAYER_A);

    printf("Select your hand...\n");
    tt_set_hand(TT_PLAYER_A, player_start_hand);
    tt_set_hand(TT_PLAYER_B, enemy_start_hand);

    printf("Quadruple quartet!\n");
    printf("\n%s\n", tt_board_state_json());

    int hand_pos = 0, board_x = 0, board_y = 0;

    for(uint8_t move = 0; move < N_MOVES; ++move)
    {
        for(uint8_t turn = 0; turn < 2; ++turn)
        {
            if(TT_PLAYER_A == turn)
            {
                hand_pos = player_moves[move][0];
                board_x = player_moves[move][1];
                board_y = player_moves[move][2];
            }
            if(TT_PLAYER_B == turn)
            {
                hand_pos = enemy_moves[move][0];
                board_x = enemy_moves[move][1];
                board_y = enemy_moves[move][2];
            }

            uint8_t s = 0;
            const uint8_t* curr_hand = tt_get_hand(turn, &s);
            uint8_t selected_card_idx = curr_hand[hand_pos];

            enum tt_error err = tt_place_card(turn, hand_pos, board_x, board_y);

            if(TT_Err_Ok == err)
            {
                printf("Placed: %s\n", tt_get_card_name(selected_card_idx));
            }
            else
            {
                printf("Failed to place card: %d\n", err);
            }

            while(tt_update_game())
            {
                printf("Updated\n");
                printf("\n%s\n", tt_board_state_json());
            }
        }
    }


}