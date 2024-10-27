#include "../inc/tt.h"

#include <string.h>
#include <assert.h>

static struct tt_board board = { 0 };

// The board index of the last card placed on the board
static uint8_t         last_card_board_idx;

// The board index of the neighbours of the last placed card
static uint8_t         last_neigh_idxs[TT_Pos_Count];


static const struct tt_card card_master_list[0xFF] = {
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
    {1, "Cockatrice",     {2, 1, 2, 6}, TT_Elem_Lightning},
    [0xFE] = {1, "None",     {1, 1, 1, 1}, TT_Elem_None}

// Level 2
};


/* Static functions */
static uint8_t on_board_for_owner(enum tt_player_type owner)
{
    uint8_t count = 0;
    for(uint8_t i = 0; i < (TTC_N_ROWS * TTC_N_COLS); ++i)
    {
        if(owner == board.cards[i].owner)
        {
            count++;
        }
    }
    return count;
}


static void remove_card_from_hand(uint8_t *array, size_t length, size_t index)
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
    array[length - 1] = TTC_EMPTY_CARD;
}


static void check_neigbours()
{
    struct tt_card card = card_master_list[last_card_board_idx];
}


static void update_last_neighbors(uint8_t card_idx)
{
    memset(last_neigh_idxs, TTC_EMPTY_CARD, TT_Pos_Count);

    const uint8_t col = card_idx % TTC_N_COLS;
    const uint8_t row = card_idx / TTC_N_COLS;

    if(col > 0)
    {
        if(TTC_EMPTY_CARD != board.cards[card_idx - 1].master_idx)
        {
            last_neigh_idxs[TT_Pos_Left] = card_idx - 1;
        }
    }
    if(col < 2)
    {
        if(TTC_EMPTY_CARD != board.cards[card_idx + 1].master_idx)
        {
            last_neigh_idxs[TT_Pos_Right] = card_idx + 1;
        }
    }
    if(row > 0)
    {
        if(TTC_EMPTY_CARD != board.cards[card_idx - TTC_N_ROWS].master_idx)
        {
            last_neigh_idxs[TT_Pos_Up] = card_idx - TTC_N_ROWS;
        }
    }
    if(row < 2)
    {
        if(TTC_EMPTY_CARD != board.cards[card_idx + TTC_N_ROWS].master_idx)
        {
            last_neigh_idxs[TT_Pos_Down] = card_idx + TTC_N_ROWS;
        }
    }

    printf("Neigh (%d): (U:%d, R:%d:, D:%d, L:%d)\n",
        card_idx,
        last_neigh_idxs[TT_Pos_Up],
        last_neigh_idxs[TT_Pos_Right],
        last_neigh_idxs[TT_Pos_Down],
        last_neigh_idxs[TT_Pos_Left]);
}


static void toggle_player_turn(void)
{
    if(board.player_turn == TT_PLAYER_A)
    {
        board.player_turn = TT_PLAYER_B;
    }
    else if(board.player_turn == TT_PLAYER_B)
    {
        board.player_turn = TT_PLAYER_A;
    }
    assert(board.player_turn != TT_PLAYER_NONE);
}


void tt_init(void)
{
    printf("Starting Triple Triad!\n");
    board.state = StateSelectHand;
    board.player_turn = TT_PLAYER_A;

    for(uint8_t i = 0; i < (TTC_N_ROWS * TTC_N_COLS); ++i)
    {
        board.cards[i].master_idx = TTC_EMPTY_CARD;
        board.cards[i].owner      = TT_PLAYER_NONE;
    }
}


void tt_set_start_player(enum tt_player_type player)
{
    board.player_turn = player;
}


const struct tt_board* tt_board_state(void)
{
    static struct tt_board _board;
    memcpy(&_board, &board, sizeof(struct tt_board));
    return &_board;
}

#define TT_BOARD_JSON_STR_FORMAT

const char* tt_board_state_json(void)
{
    static char buf[0xFF];
    sprintf(buf,
            "{\"turn\":\"%c\","
            "\"handA\":[%d,%d,%d,%d,%d],"
            "\"handB\":[%d,%d,%d,%d,%d],"
            "\"cards\":[" //%d,%d,%d,%d,%d,%d,%d,%d,%d],"
            "{\"o\":\"%c\",\"i\":%d},"
            "{\"o\":\"%c\",\"i\":%d},"
            "{\"o\":\"%c\",\"i\":%d},"
            "{\"o\":\"%c\",\"i\":%d},"
            "{\"o\":\"%c\",\"i\":%d},"
            "{\"o\":\"%c\",\"i\":%d},"
            "{\"o\":\"%c\",\"i\":%d},"
            "{\"o\":\"%c\",\"i\":%d},"
            "{\"o\":\"%c\",\"i\":%d}"
            "]"
            "}",
            (board.player_turn == TT_PLAYER_A) ? 'A' : 'B',
            board.hand[TT_PLAYER_A].values[0],
            board.hand[TT_PLAYER_A].values[1],
            board.hand[TT_PLAYER_A].values[2],
            board.hand[TT_PLAYER_A].values[3],
            board.hand[TT_PLAYER_A].values[4],
            board.hand[TT_PLAYER_B].values[0],
            board.hand[TT_PLAYER_B].values[1],
            board.hand[TT_PLAYER_B].values[2],
            board.hand[TT_PLAYER_B].values[3],
            board.hand[TT_PLAYER_B].values[4],
            (board.cards[0].owner == TT_PLAYER_A) ? 'A' : 'B',
            board.cards[0].master_idx,
            (board.cards[1].owner == TT_PLAYER_A) ? 'A' : 'B',
            board.cards[1].master_idx,
            (board.cards[2].owner == TT_PLAYER_A) ? 'A' : 'B',
            board.cards[2].master_idx,
            (board.cards[3].owner == TT_PLAYER_A) ? 'A' : 'B',
            board.cards[3].master_idx,
            (board.cards[4].owner == TT_PLAYER_A) ? 'A' : 'B',
            board.cards[4].master_idx,
            (board.cards[5].owner == TT_PLAYER_A) ? 'A' : 'B',
            board.cards[5].master_idx,
            (board.cards[6].owner == TT_PLAYER_A) ? 'A' : 'B',
            board.cards[6].master_idx,
            (board.cards[7].owner == TT_PLAYER_A) ? 'A' : 'B',
            board.cards[7].master_idx,
            (board.cards[8].owner == TT_PLAYER_A) ? 'A' : 'B',
            board.cards[8].master_idx
    );
    return buf;
}


enum tt_player_type tt_curr_player_turn(void)
{
    return board.player_turn;
}


struct tt_score tt_get_score(void)
{
    struct tt_score score = {0, 0};

    score.a = board.hand[TT_PLAYER_A].size;
    score.b = board.hand[TT_PLAYER_B].size;

    score.a += on_board_for_owner(TT_PLAYER_A);
    score.b += on_board_for_owner(TT_PLAYER_B);

    return score;
}


bool tt_game_over(void)
{
    for(uint8_t i = 0; i < TTC_N_ROWS * TTC_N_COLS; ++i)
    {
        if(TTC_EMPTY_CARD == board.cards[i].master_idx)
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


static void update_board_state(void)
{
    const struct tt_card last_card  = card_master_list[last_card_board_idx];
    const enum tt_player_type owner = board.cards[last_card_board_idx].owner;

    const struct tt_card* cmp_up    = &card_master_list[board.cards[last_neigh_idxs[TT_Pos_Up]].master_idx];
    const struct tt_card* cmp_right = &card_master_list[board.cards[last_neigh_idxs[TT_Pos_Right]].master_idx];
    const struct tt_card* cmp_down  = &card_master_list[board.cards[last_neigh_idxs[TT_Pos_Down]].master_idx];
    const struct tt_card* cmp_left  = &card_master_list[board.cards[last_neigh_idxs[TT_Pos_Left]].master_idx];

    if(last_card.values[TT_Pos_Up] > cmp_up->values[TT_Pos_Down])
    {
        board.cards[last_neigh_idxs[TT_Pos_Up]].owner = owner;
    }

    if(last_card.values[TT_Pos_Right] > cmp_right->values[TT_Pos_Left])
    {
        board.cards[last_neigh_idxs[TT_Pos_Right]].owner = owner;
    }

    if(last_card.values[TT_Pos_Down] > cmp_down->values[TT_Pos_Up])
    {
        board.cards[last_neigh_idxs[TT_Pos_Down]].owner = owner;
    }

    if(last_card.values[TT_Pos_Left] > cmp_left->values[TT_Pos_Right])
    {
        board.cards[last_neigh_idxs[TT_Pos_Left]].owner = owner;
    }
}


bool tt_update_game(void)
{
    if(board.check_pending)
    {

        printf("%d\n", last_card_board_idx);

        if(rule_active(TT_R_Combo))
        {
            board.check_pending = true;
        }
        update_board_state();
        toggle_player_turn();

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


const struct tt_card* tt_get_card(uint8_t card_index)
{
    if(card_index > (sizeof(card_master_list) / sizeof card_master_list[0]))
    {
        return NULL;
    }
    return &card_master_list[card_index];
}


const struct tt_card* tt_get_player_cards(enum tt_player_type player)
{
    return card_master_list;
}


void tt_set_player_hand(enum tt_player_type player, const uint8_t idxs[TTC_MAX_HAND_SIZE])
{
    for(uint8_t i = 0; i < TTC_MAX_HAND_SIZE; ++i)
    {
        board.hand[player].values[i] = idxs[i];
    }
    board.hand[player].size = TTC_MAX_HAND_SIZE;
}


const uint8_t* tt_get_player_hand(enum tt_player_type player, uint8_t* size)
{
    if(board.hand[player].size > TTC_MAX_HAND_SIZE)
    {
        board.hand[player].size = TTC_MAX_HAND_SIZE;
    }

    *size = board.hand[player].size;
    return board.hand[player].values;
}


bool tt_place_card(enum tt_player_type player, uint8_t hand_idx, uint8_t board_x, uint8_t board_y)
{

    if((board.hand[player].size > TTC_MAX_HAND_SIZE) ||
        (board_x > TTC_N_ROWS) ||
        (board_y > TTC_N_COLS) ||
        (hand_idx > board.hand[player].size))
    {
        return false;
    }

    if(TTC_EMPTY_CARD != board.cards[board_y * TTC_N_COLS + board_x].master_idx)
    {
        return false;
    }

    if(board.hand[player].size > 0)
    {
        const int board_idx = board_y * TTC_N_COLS + board_x;

        last_card_board_idx               = board_idx;
        board.cards[board_idx].master_idx = board.hand[player].values[hand_idx];
        board.cards[board_idx].owner      = player;

        remove_card_from_hand(board.hand[player].values, board.hand[player].size, hand_idx);
        board.hand[player].size--;

        update_last_neighbors(last_card_board_idx);
        board.check_pending = true;
        return true;
    }

    return false;
}


void tt_print_hand(enum tt_player_type player)
{
    printf("Hand[%d]: [", (uint8_t)player);
    for(uint8_t i = 0; i < board.hand[player].size; ++i)
    {
        printf("%d,", board.hand[player].values[i]);
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
            printf("(%d:%d),", board.cards[y * TTC_N_COLS + x].owner, board.cards[y * TTC_N_COLS + x].master_idx);
        }
        printf("]\n");
    }
}

