#include <stdio.h>
#include <stdint.h>

#include "../inc/tt.h"
#include "../inc/tt_sim.h"

// The hand the player starts with
uint8_t player_start_hand[TTC_MAX_HAND_SIZE] = {1, 2, 3, 4, 5};
uint8_t enemy_start_hand[TTC_MAX_HAND_SIZE] = {6, 7, 8, 9, 10};

#define N_MOVES 4

uint8_t enemy_moves[N_MOVES][3] = {
    {0, 0, 1},
    {0, 1, 1},
    {0, 0, 2},
    {0, 1, 0}
};

uint8_t player_moves[N_MOVES][3] = {
    {0, 0, 0},
    {0, 1, 2},
    {0, 2, 0},
    {0, 2, 1}
};


int main(void)
{
    run_simulation(player_start_hand, enemy_start_hand, player_moves, enemy_moves);
    return 0;
}
