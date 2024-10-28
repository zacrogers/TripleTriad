#include "../inc/tt.h"

#include <string.h>
#include <assert.h>

#define XY_TO_I(x, y) (y * TTC_N_COLS + x)

static struct tt_board board = { 0 };

// The board index of the last card placed on the board
static uint8_t         last_card_board_idx;

// The board index of the neighbours of the last placed card
static uint8_t         last_neigh_ids[TT_Pos_Count];

// Set true if a card has been added to the board and need to be checked against the rules
static bool            check_pending = false;

static const struct tt_card card_master_list[0xFF] = {
// Level 1
    { 1, "Geezard",        {1, 4, 1, 5}, TT_Elem_None },
    { 1, "Funguar",        {5, 1, 1, 3}, TT_Elem_None },
    { 1, "Bite Bug",       {1, 3, 3, 5}, TT_Elem_None },
    { 1, "Red Bat",        {6, 1, 1, 2}, TT_Elem_None },
    { 1, "Blobra",         {2, 3, 1, 5}, TT_Elem_None },
    { 1, "Gayla",          {2, 1, 4, 4}, TT_Elem_Lightning },
    { 1, "Gesper",         {1, 5, 4, 1}, TT_Elem_None },
    { 1, "Fastitocalon-F", {3, 5, 2, 1}, TT_Elem_Earth },
    { 1, "Blood Soul",     {2, 1, 6, 1}, TT_Elem_None },
    { 1, "Caterchipillar", {4, 2, 4, 3}, TT_Elem_None },
    { 1, "Cockatrice",     {2, 1, 2, 6}, TT_Elem_Lightning },

// Level 2
    { 2, "Grat",           {7, 1, 3, 1}, TT_Elem_None },
    { 2, "Buel",           {6, 2, 2, 3}, TT_Elem_None },
    { 2, "Mesmerize",      {5, 3, 3, 4}, TT_Elem_None },
    { 2, "Glacial Eye",    {6, 1, 4, 3}, TT_Elem_Ice },
    { 2, "Belhelmel",      {3, 4, 5, 3}, TT_Elem_None },
    { 2, "Thrustaevis",    {5, 3, 2, 5}, TT_Elem_Wind },
    { 2, "Anacondaur",     {5, 1, 3, 5}, TT_Elem_Poison },
    { 2, "Creeps",         {5, 2, 5, 2}, TT_Elem_Lightning },
    { 2, "Grendel",        {4, 4, 5, 2}, TT_Elem_Lightning },
    { 2, "Jelleye",        {3, 2, 1, 7}, TT_Elem_None },
    { 2, "Grand Mantis",   {5, 2, 5, 3}, TT_Elem_None },

// Level 3
    { 3, "Forbidden",      {6, 6, 3, 2}, TT_Elem_None },
    { 3, "Armadodo",       {6, 3, 1, 6}, TT_Elem_Earth },
    { 3, "Tri-Face",       {3, 5, 5, 5}, TT_Elem_Poison },
    { 3, "Fastitocalon",   {7, 5, 1, 3}, TT_Elem_Earth },
    { 3, "Snow Lion",      {7, 1, 5, 3}, TT_Elem_Ice },
    { 3, "Ochu",           {5, 6, 3, 3}, TT_Elem_None },
    { 3, "SAM08G",         {5, 6, 2, 4}, TT_Elem_Fire },
    { 3, "Death Claw",     {4, 4, 7, 2}, TT_Elem_Fire },
    { 3, "Cactuar",        {6, 2, 6, 3}, TT_Elem_None },
    { 3, "Tonberry",       {3, 6, 4, 4}, TT_Elem_None },
    { 3, "Abyss Worm",     {7, 2, 3, 5}, TT_Elem_Earth },

// Level 4
    { 4, "Turtapod",       {2, 3, 6, 7}, TT_Elem_None },
    { 4, "Vysage",         {6, 5, 4, 5}, TT_Elem_None },
    { 4, "T-Rexaur",       {4, 6, 2, 7}, TT_Elem_None },
    { 4, "Bomb",           {7, 5, 1, 3}, TT_Elem_Fire },
    { 4, "Blitz",          {1, 6, 4, 7}, TT_Elem_Lightning },
    { 4, "Wendigo",        {7, 3, 1, 6}, TT_Elem_None },
    { 4, "Torama",         {7, 4, 4, 4}, TT_Elem_None },
    { 4, "Imp",            {3, 7, 3, 6}, TT_Elem_None },
    { 4, "Blue Dragon",    {6, 2, 7, 3}, TT_Elem_Poison },
    { 4, "Adamantoise",    {4, 5, 5, 6}, TT_Elem_Earth },
    { 4, "Hexadragon",     {7, 5, 4, 3}, TT_Elem_Fire },

// Level 5
    { 5, "Iron Giant",     {6, 5, 6, 5},   TT_Elem_None },
    { 5, "Behemoth",       {3, 6, 5, 7},   TT_Elem_None },
    { 5, "Chimera",        {7, 6, 5, 3},   TT_Elem_Water },
    { 5, "PuPu",           {3, 0xA, 2, 1}, TT_Elem_None },
    { 5, "Elastoid",       {6, 2, 6, 2},   TT_Elem_None },
    { 5, "GIM47N",         {5, 5, 7, 4},   TT_Elem_None },
    { 5, "Marlboro",       {7, 7, 4, 2},   TT_Elem_Poison },
    { 5, "Ruby Dragon",    {7, 2, 7, 4},   TT_Elem_Fire },
    { 5, "Elnoyle",        {5, 3, 7, 6},   TT_Elem_None },
    { 5, "Tonberry King",  {4, 6, 7, 4},   TT_Elem_None },
    { 5, "BiggsWedge",     {6, 6, 2, 7},   TT_Elem_None },

    [0xFE] = {1, "None",     {1, 1, 1, 1}, TT_Elem_None}
};


/* Static functions */

static char player_type_to_char(enum tt_player_type p)
{
    switch (p)
    {
    case TT_PLAYER_A:
        return 'A';
    case TT_PLAYER_B:
        return 'B';
    default:
        return 'N';
    }
}


static uint8_t n_cards_on_board_for_owner(enum tt_player_type owner)
{
    uint8_t count = 0;
    for(uint8_t i = 0; i < TTC_BOARD_SIZE; ++i)
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
    if (index >= length)
    {
        printf("Index out of bounds\n");
        return;
    }

    // Shift elements to the left
    for (size_t i = index; i < length - 1; i++)
    {
        array[i] = array[i + 1];
    }

    array[length - 1] = TTC_EMPTY_CARD_ID;
}


static bool board_slot_empty(uint8_t board_index)
{
    return (TTC_EMPTY_CARD_ID == board.cards[board_index].master_id);
}


static void update_last_neighbors(uint8_t card_idx)
{
    // Set all neighs to empty
    memset(last_neigh_ids, TTC_EMPTY_CARD_ID, TT_Pos_Count);

    const uint8_t col = card_idx % TTC_N_COLS;
    const uint8_t row = card_idx / TTC_N_COLS;

    if(col > 0)
    {
        if(!board_slot_empty(card_idx - 1))
        {
            last_neigh_ids[TT_Pos_Up] = card_idx - 1;
        }
    }
    if(col < 2)
    {
        if(!board_slot_empty(card_idx + 1))
        {
            last_neigh_ids[TT_Pos_Down] = card_idx + 1;
        }
    }
    if(row > 0)
    {
        if(!board_slot_empty(card_idx - TTC_N_ROWS))
        {
            last_neigh_ids[TT_Pos_Left] = card_idx - TTC_N_ROWS;
        }
    }
    if(row < 2)
    {
        if(!board_slot_empty(card_idx + TTC_N_ROWS))
        {
            last_neigh_ids[TT_Pos_Right] = card_idx + TTC_N_ROWS;
        }
    }

    // printf("Neigh (%d): (U:%d, R:%d:, D:%d, L:%d)\n",
    //     card_idx,
    //     last_neigh_ids[TT_Pos_Up],
    //     last_neigh_ids[TT_Pos_Right],
    //     last_neigh_ids[TT_Pos_Down],
    //     last_neigh_ids[TT_Pos_Left]);
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


static void update_score(void)
{
    board.score.a = board.hand[TT_PLAYER_A].size;
    board.score.b = board.hand[TT_PLAYER_B].size;

    board.score.a += n_cards_on_board_for_owner(TT_PLAYER_A);
    board.score.b += n_cards_on_board_for_owner(TT_PLAYER_B);
}


static bool game_over(void)
{
    for(uint8_t i = 0; i < TTC_BOARD_SIZE; ++i)
    {
        if(TTC_EMPTY_CARD_ID == board.cards[i].master_id)
        {
            return false;
        }
    }
    return true;
}


static bool rule_active(enum tt_rules rule)
{
    return rule & board.rules;
}


static bool last_neighbour_empty(enum tt_card_pos pos)
{
    return (TTC_EMPTY_CARD_ID == last_neigh_ids[pos]);
}


static void check_basic_rules(void)
{
    const struct tt_card last_card  = card_master_list[board.cards[last_card_board_idx].master_id];
    const enum tt_player_type owner = board.cards[last_card_board_idx].owner;

    if(!last_neighbour_empty(TT_Pos_Up))
    {
        uint8_t up_id = board.cards[last_neigh_ids[TT_Pos_Up]].master_id;
        const struct tt_card* cmp_up    = &card_master_list[up_id];

        if((last_card.values[TT_Pos_Up] > cmp_up->values[TT_Pos_Down]))
        {
            board.cards[last_neigh_ids[TT_Pos_Up]].owner = owner;
        }
    }

    if(!last_neighbour_empty(TT_Pos_Right))
    {
        const struct tt_card* cmp_right = &card_master_list[board.cards[last_neigh_ids[TT_Pos_Right]].master_id];

        if(last_card.values[TT_Pos_Right] > cmp_right->values[TT_Pos_Left])
        {
            board.cards[last_neigh_ids[TT_Pos_Right]].owner = owner;
        }
    }


    if(!last_neighbour_empty(TT_Pos_Down))
    {
        const struct tt_card* cmp_down  = &card_master_list[board.cards[last_neigh_ids[TT_Pos_Down]].master_id];
        if(last_card.values[TT_Pos_Down] > cmp_down->values[TT_Pos_Up])
        {
            board.cards[last_neigh_ids[TT_Pos_Down]].owner = owner;
        }
    }

    if(!last_neighbour_empty(TT_Pos_Left))
    {
        const struct tt_card* cmp_left  = &card_master_list[board.cards[last_neigh_ids[TT_Pos_Left]].master_id];

        if(last_card.values[TT_Pos_Left] > cmp_left->values[TT_Pos_Right])
        {
            board.cards[last_neigh_ids[TT_Pos_Left]].owner = owner;
        }
    }
}

static void update_board_state(void)
{
    check_basic_rules();

    if(rule_active(TT_R_Same))
    {

    }
    if(rule_active(TT_R_Same))
    {

    }

    update_score();
}



/* Public function */
void tt_board_init(enum tt_player_type start_player, rule_bitmask_t rule_bitmask)
{
    board.state       = StateSelectHand;
    board.player_turn = start_player;
    board.rules       = rule_bitmask;

    for(uint8_t i = 0; i < TTC_BOARD_SIZE; ++i)
    {
        board.cards[i].master_id = TTC_EMPTY_CARD_ID;
        board.cards[i].owner     = TT_PLAYER_NONE;
    }
}


const struct tt_board* tt_board_state(void)
{
    static struct tt_board _board;
    memcpy(&_board, &board, sizeof(struct tt_board));
    return &_board;
}


const char* tt_board_state_json(void)
{
    static char buf[350];
    sprintf(buf,
            "{\"turn\":%d,"
            "\"score\":[%d,%d],"
            "\"handA\":[%d,%d,%d,%d,%d],"
            "\"handB\":[%d,%d,%d,%d,%d],"
            "\"board\":["
            "{\"o\":%d,\"id\":%d,\"e\":%d},"
            "{\"o\":%d,\"id\":%d,\"e\":%d},"
            "{\"o\":%d,\"id\":%d,\"e\":%d},"
            "{\"o\":%d,\"id\":%d,\"e\":%d},"
            "{\"o\":%d,\"id\":%d,\"e\":%d},"
            "{\"o\":%d,\"id\":%d,\"e\":%d},"
            "{\"o\":%d,\"id\":%d,\"e\":%d},"
            "{\"o\":%d,\"id\":%d,\"e\":%d},"
            "{\"o\":%d,\"id\":%d,\"e\":%d}"
            "]"
            "}",
            board.player_turn,
            board.score.a,
            board.score.b,
            board.hand[TT_PLAYER_A].ids[0],
            board.hand[TT_PLAYER_A].ids[1],
            board.hand[TT_PLAYER_A].ids[2],
            board.hand[TT_PLAYER_A].ids[3],
            board.hand[TT_PLAYER_A].ids[4],
            board.hand[TT_PLAYER_B].ids[0],
            board.hand[TT_PLAYER_B].ids[1],
            board.hand[TT_PLAYER_B].ids[2],
            board.hand[TT_PLAYER_B].ids[3],
            board.hand[TT_PLAYER_B].ids[4],
            board.cards[0].owner, board.cards[0].master_id, board.cards[0].elem,
            board.cards[1].owner, board.cards[1].master_id, board.cards[1].elem,
            board.cards[2].owner, board.cards[2].master_id, board.cards[2].elem,
            board.cards[3].owner, board.cards[3].master_id, board.cards[3].elem,
            board.cards[4].owner, board.cards[4].master_id, board.cards[4].elem,
            board.cards[5].owner, board.cards[5].master_id, board.cards[5].elem,
            board.cards[6].owner, board.cards[6].master_id, board.cards[6].elem,
            board.cards[7].owner, board.cards[7].master_id, board.cards[7].elem,
            board.cards[8].owner, board.cards[8].master_id, board.cards[8].elem
    );
    return buf;
}


bool tt_update_game(void)
{
    if(check_pending)
    {
        if(rule_active(TT_R_Combo))
        {
            check_pending = true;
        }
        update_board_state();
        toggle_player_turn();

        check_pending = false;
        return true;
    }

    return false;
}


const char* tt_get_card_name(uint8_t card_index)
{
    if(card_index > (sizeof(card_master_list) / sizeof card_master_list[0]))
    {
        return card_master_list[TTC_EMPTY_CARD_ID].name;
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


enum tt_error tt_place_card(enum tt_player_type player, uint8_t hand_idx, uint8_t board_x, uint8_t board_y)
{

    if((board.hand[player].size > TTC_MAX_HAND_SIZE) || (hand_idx > board.hand[player].size))
    {
        return TT_Err_Hand_Range;
    }

    if((board_x > TTC_N_ROWS) || (board_y > TTC_N_COLS))
    {
        return TT_Err_Board_Bounds;
    }

    const int board_idx = XY_TO_I(board_x, board_y);

    if(TTC_EMPTY_CARD_ID != board.cards[board_idx].master_id)
    {
        return TT_Err_Cell_Occupied;
    }

    if(board.hand[player].size > 0)
    {

        last_card_board_idx              = board_idx;
        board.cards[board_idx].master_id = board.hand[player].ids[hand_idx];
        board.cards[board_idx].owner     = player;

        remove_card_from_hand(board.hand[player].ids, board.hand[player].size, hand_idx);
        board.hand[player].size--;

        update_last_neighbors(last_card_board_idx);
        check_pending = true;
        return TT_Err_Ok;
    }

    return TT_Err_Unknown;
}


const struct tt_card* tt_get_player_cards(enum tt_player_type player)
{
    return card_master_list;
}


void tt_set_hand(enum tt_player_type player, const uint8_t idxs[TTC_MAX_HAND_SIZE])
{
    for(uint8_t i = 0; i < TTC_MAX_HAND_SIZE; ++i)
    {
        board.hand[player].ids[i] = idxs[i];
    }
    board.hand[player].size = TTC_MAX_HAND_SIZE;
}


const uint8_t* tt_get_hand(enum tt_player_type player, uint8_t* size)
{
    if(board.hand[player].size > TTC_MAX_HAND_SIZE)
    {
        board.hand[player].size = TTC_MAX_HAND_SIZE;
    }

    *size = board.hand[player].size;
    return board.hand[player].ids;
}

