# TripleTriad

The card game from Final Fantasy 8. The core logic of the game has been written in C to make it easy to embed where C is easy to embed. Currently only python is implemented.


To run with the python wrapper: ```bash run.sh -w python```
To run the C demo simulation ```bash run.sh -w c```


Future ideas:
- Port it to zephyr with lvgl for graphics
- Port it to gameboy with GBDK
- Port it to go, with online multiplayer & with js frontend


The following very loosely shows the flow of how to use the game core:
```python
TripleTriadCore.init_board()
TripleTriadCore.set_hand(TTPlayerType.TT_PLAYER_A, self.p1_hand)
TripleTriadCore.set_hand(TTPlayerType.TT_PLAYER_B, self.p2_hand)

while running:
    # handle controls

    s = TripleTriadCore.get_state()

    # handle drawing based on the state

    # Place a card and update the game to move
    err = TripleTriadCore.place_card(
        ...
    )

    if not TripleTriadCore.update():
        ...
```

The board state holds everything about the game state. It is updated internally when .update() is called, and a card has been placed.:
```C
struct tt_board {
    struct tt_hand      hand[2];
    enum tt_player_type player_turn;
    struct tt_card_cell cards[TTC_BOARD_SIZE];
    enum tt_game_state  state;
    struct tt_score     score;
    rule_bitmask_t      rules;
};
```
