#ifndef TEST_H_
#define TEST_H_


#define N_MOVES 4

void run_tests(
    uint8_t player_start_hand[5],
    uint8_t rival_start_hand[5],
    uint8_t player_moves[N_MOVES][3],
    uint8_t enemy_moves[N_MOVES][3]
);

#endif  /* TEST_H_ */