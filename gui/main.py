import ctypes
from pprint import pprint
import json

import triple_triad as tt



if __name__ == "__main__":
    tt.init_board()
    tt.set_hand(tt.TTPlayerType.TT_PLAYER_A, [1,2,3,4,5])

    s = tt.get_state()

    hand_names = [tt.get_card_name(c_id) for c_id in s.get("handA")]

    print(hand_names)

    pprint(s)
    tt.place_card(tt.TTPlayerType.TT_PLAYER_A, 0, 0, 0)

    pprint(tt.get_state())

