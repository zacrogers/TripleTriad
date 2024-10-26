#include "../inc/tt.h"

#include <string.h>
#include <assert.h>

static struct tt_board board = { 0 };


static const struct tt_card card_master_list[] = {
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


/* Static functions */
static uint8_t on_board_for_owner(enum tt_player_type owner)
{
    uint8_t count = 0;
    for(uint8_t i = 0; i < (TTC_N_ROWS * TTC_N_COLS); ++i)
    {
        if(owner == board.card_owners[i])
        {
            count++;
        }
    }
    return count;
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


static void update_last_neighbors(uint8_t card_idx)
{
    memset(board.last_neighbours, TTC_EMPTY_CARD, TT_Pos_Count);

    const uint8_t col = card_idx % TTC_N_COLS;
    const uint8_t row = card_idx / TTC_N_COLS;

    if(col > 0)
    {
        if(TTC_EMPTY_CARD != board.cards[card_idx - 1])
        {
            board.last_neighbours[TT_Pos_Left] = card_idx - 1;
        }
    }
    if(col < 2)
    {
        if(TTC_EMPTY_CARD != board.cards[card_idx + 1])
        {
            board.last_neighbours[TT_Pos_Right] = card_idx + 1;
        }
    }
    if(row > 0)
    {
        if(TTC_EMPTY_CARD != board.cards[card_idx - TTC_N_ROWS])
        {
            board.last_neighbours[TT_Pos_Up] = card_idx - TTC_N_ROWS;
        }
    }
    if(row < 2)
    {
        if(TTC_EMPTY_CARD != board.cards[card_idx + TTC_N_ROWS])
        {
            board.last_neighbours[TT_Pos_Down] = card_idx + TTC_N_ROWS;
        }
    }

    printf("Neigh (%d): (U:%d, R:%d:, D:%d, L:%d)\n",
        card_idx,
        board.last_neighbours[TT_Pos_Up],
        board.last_neighbours[TT_Pos_Right],
        board.last_neighbours[TT_Pos_Down],
        board.last_neighbours[TT_Pos_Left]);
}


void tt_init()
{
    printf("Starting Triple Triad!\n");
    board.state = StateSelectHand;

    for(uint8_t i = 0; i < (TTC_N_ROWS * TTC_N_COLS); ++i)
    {
        board.cards[i] = TTC_EMPTY_CARD;
        board.card_owners[i] = TT_PLAYER_NONE;;
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


struct tt_score tt_get_score(void)
{
    struct tt_score score = {0, 0};

    score.a = board.player[TT_PLAYER_A].hand_size;
    score.b = board.player[TT_PLAYER_B].hand_size;

    score.a += on_board_for_owner(TT_PLAYER_A);
    score.b += on_board_for_owner(TT_PLAYER_B);

    return score;
}



bool tt_game_over(void)
{
    for(uint8_t i = 0; i < TTC_N_ROWS * TTC_N_COLS; ++i)
    {
        if(TTC_EMPTY_CARD == board.cards[i])
        {
            return false;
        }
    }
    return true;
}


bool rule_active(enum tt_rules rule)
{
    return false;
}


bool tt_update_game(void)
{
    if(board.check_pending)
    {
        printf("%d\n", board.last_card_added);

        if(rule_active(TT_R_Combo))
        {

            board.check_pending = true;
        }

        board.check_pending = false;
        return true;
    }

    return false;
}



const char* tt_get_card_name(uint8_t card_index)
{
    if(card_index > (sizeof(card_master_list) / sizeof card_master_list[0]))
    {
        return "dsdasdsadasd";
    }
    return card_master_list[card_index].name;
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


bool tt_place_card(enum tt_player_type player, uint8_t hand_idx, uint8_t board_x, uint8_t board_y)
{

    if((board.player[player].hand_size > TTC_MAX_HAND_SIZE) ||
        (board_x > TTC_N_ROWS) ||
        (board_y > TTC_N_COLS) ||
        (hand_idx > board.player[player].hand_size))
    {
        return false;
    }

    if(TTC_EMPTY_CARD != board.cards[board_y * TTC_N_COLS + board_x])
    {
        return false;
    }

    if(board.player[player].hand_size > 0)
    {
        const int board_idx = board_y * TTC_N_COLS + board_x;

        board.last_card_added        = board_idx;
        board.cards[board_idx]       = board.player[player].hand[hand_idx];
        board.card_owners[board_idx] = player;

        remove_element(board.player[player].hand, board.player[player].hand_size, hand_idx);
        board.player[player].hand_size--;

        update_last_neighbors(board.last_card_added);
        board.check_pending = true;
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
            printf("(%d:%d),", board.card_owners[y * TTC_N_COLS + x], board.cards[y * TTC_N_COLS + x]);
        }
        printf("]\n");
    }
}

