#include "../inc/tt.h"

const struct tt_card card_master_list[] = {
// Level 1
    {1, "Geezard",        {1, 4, 1, 5}, TT_Elem_None},
    {1, "Funguar",        {5, 1, 1, 3}, TT_Elem_None},
    {1, "Bite Bug",       {1, 3, 3, 5}, TT_Elem_None},
    {1, "Red Bat",        {6, 1, 1, 2}, TT_Elem_None},
    {1, "Blobra",         {2, 3, 1, 5}, TT_Elem_None},
    {1, "Gayla",          {2, 1, 4, 4}, TT_Elem_Lightning},
    {1, "Gesper",         {1, 5, 4, 1}, TT_Elem_None},
    {1, "Fastitocalon-F", {3, 5, 2, 1}, TT_Elem_Earth},
    {1, "Blood Soul",     {2, 1, 6, 1}, TT_Elem_None},
    {1, "Caterchipillar", {4, 2, 4, 3}, TT_Elem_None},
    {1, "Cockatrice",     {2, 1, 2, 6}, TT_Elem_Lightning}

// Level 2
};

static struct tt_board board = { 0 };


void tt_init()
{
    printf("Starting Triple Triad!\n");
    board.state = StateSelectHand;
}


const struct tt_card* tt_get_user_cards(void)
{
    return card_master_list;
}


void tt_set_user_hand(const uint8_t idxs[HAND_SIZE])
{
    for(uint8_t i = 0; i < HAND_SIZE; ++i)
    {
        board.player_a.hand[i] = idxs[i];
    }
    board.player_a.hand_size = HAND_SIZE;
}


const uint8_t* tt_get_user_hand(uint8_t* size)
{
    *size = board.player_a.hand_size;
    return board.player_a.hand;
}
static void remove_element(uint8_t *array, size_t length, size_t index)
{
    if (index >= length) {
        printf("Index out of bounds\n");
        return;
    }

    // Shift elements to the left
    for (size_t i = index; i < length - 1; i++) {
        array[i] = array[i + 1];
    }

    // Optional: Set the last element to 0 (or a specific value)
    array[length - 1] = 0;
}

void tt_place_card(uint8_t card_idx, uint8_t board_x, uint8_t board_y)
{
    if(board.player_a.hand_size > 0)
    {
        board.cards[board_x * board_y] = board.player_a.hand[card_idx];

        remove_element(board.player_a.hand, board.player_a.hand_size, card_idx);
        board.player_a.hand_size--;

    }
}


void tt_print_hand(void)
{
    printf("P: [");
    for(uint8_t i = 0; i < board.player_a.hand_size; ++i)
    {
        printf("%d,", board.player_a.hand[i]);
    }
    printf("]\n");

}


void tt_print_board(void)
{

}

