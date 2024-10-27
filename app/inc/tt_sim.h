#ifndef _TT_SIM_H_
#define _TT_SIM_H_

#include "../inc/tt.h"

#define N_MOVES 4

void sim_init(const char* moves_fname);
void sim_run(void);

void run_simulation(
    uint8_t player_start_hand[TTC_MAX_HAND_SIZE],
    uint8_t rival_start_hand[TTC_MAX_HAND_SIZE],
    uint8_t player_moves[N_MOVES][3],
    uint8_t enemy_moves[N_MOVES][3]
);

#endif  /* _TT_SIM_H_ */