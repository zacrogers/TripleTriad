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

const char* tt_get_card_name(uint8_t card_index)
{
    if(card_index > (sizeof(card_master_list) / sizeof card_master_list[0]))
    {
        return "dsdasdsadasd";
    }
    return card_master_list[card_index].name;
}


static struct tt_board board = { 0 };


void tt_init()
{
    printf("Starting Triple Triad!\n");
    board.state = StateSelectHand;

    for(uint8_t i = 0; i < (TTC_N_ROWS * TTC_N_COLS); ++i)
    {
        board.cards[i] = TTC_EMPTY_CARD;
    }
}


const uint8_t* tt_board_state(void)
{
    return board.cards;
}


enum tt_player_type tt_curr_player_turn(void)
{
    return board.player_turn;
}


bool tt_game_over(void)
{
    return true;
}


bool tt_update_game(void)
{
    return false;
}


const struct tt_card* tt_get_player_cards(enum tt_player_type player)
{
    return card_master_list;
}


void tt_set_player_hand(enum tt_player_type player, const uint8_t idxs[TTC_MAX_HAND_SIZE])
{
    for(uint8_t i = 0; i < TTC_MAX_HAND_SIZE; ++i)
    {
        board.player[player].hand[i] = idxs[i];
    }
    board.player[player].hand_size = TTC_MAX_HAND_SIZE;
}


const uint8_t* tt_get_player_hand(enum tt_player_type player, uint8_t* size)
{
    if(board.player[player].hand_size > TTC_MAX_HAND_SIZE)
    {
        board.player[player].hand_size = TTC_MAX_HAND_SIZE;
    }

    *size = board.player[player].hand_size;
    return board.player[player].hand;
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


bool tt_place_card(enum tt_player_type player, uint8_t card_idx, uint8_t board_x, uint8_t board_y)
{

    if((board.player[player].hand_size > TTC_MAX_HAND_SIZE) ||
    (board_x > TTC_N_ROWS) ||
    (board_y > TTC_N_COLS) ||
    (card_idx > board.player[player].hand_size))
    {
        return false;
    }

    if(TTC_EMPTY_CARD != board.cards[board_y * TTC_N_COLS + board_x])
    {
        return false;
    }

    if(board.player[player].hand_size > 0)
    {
        board.cards[board_y * TTC_N_COLS + board_x] = board.player[player].hand[card_idx];
        remove_element(board.player[player].hand, board.player[player].hand_size, card_idx);
        board.player[player].hand_size--;
        return true;
    }

    return false;
}


void tt_print_hand(enum tt_player_type player)
{
    printf("Hand[%d]: [", (uint8_t)player);
    for(uint8_t i = 0; i < board.player[player].hand_size; ++i)
    {
        printf("%d,", board.player[player].hand[i]);
    }
    printf("]\n");
}


void tt_print_board(void)
{
    printf("Board:\n");
    for(uint8_t y = 0; y < TTC_N_ROWS; ++y)
    {
        printf("\t[");
        for(uint8_t x = 0; x < TTC_N_COLS; ++x)
        {
            printf("%d,", board.cards[y * TTC_N_COLS + x]);
        }
        printf("]\n");
    }
}

