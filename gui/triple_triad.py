import ctypes
from pprint import pprint
import json

so_file ="/Users/zacharyrogers/Code/TripleTriad/build/libTTCoreLib.dylib"
lib = ctypes.CDLL(so_file)

lib.tt_board_state_json.restype = ctypes.c_char_p
lib.tt_get_card_name.restype = ctypes.c_char_p



class TTPlayerType(ctypes.c_int):
    TT_PLAYER_A = 0
    TT_PLAYER_B = 1


class TTError(ctypes.c_int):
    TT_Err_Ok = 0
    TT_Err_Hand_Range = 1
    TT_Err_Master_Range = 2
    TT_Err_Cell_Occupied = 3
    TT_Err_Board_Bounds = 4
    TT_Err_Hand_Empty = 5
    TT_Err_Unknown = 6
    TT_Err_Count = 7


# Set the argument types and return type for tt_set_hand
lib.tt_set_hand.argtypes = [
    TTPlayerType,                  # First argument: enum tt_player_type
    ctypes.POINTER(ctypes.c_uint8) # Second argument: const uint8_t array
]
lib.tt_set_hand.restype = None  # Function returns void


def get_card_name(card_index: int):
    return lib.tt_get_card_name(card_index).decode("utf-8").replace(" ", "").replace("-", "")


def init_board(start_player: TTPlayerType=1, rules_bitmask: int=0):
    lib.tt_board_init(start_player, rules_bitmask)


def update_game():
    return lib.tt_update_game()


def get_state():
    state = lib.tt_board_state_json()
    state_j = json.loads(state)
    return state_j


def set_hand(player: TTPlayerType, hand: list[int]):
    lib.tt_set_hand(player, (ctypes.c_uint8 * 5)(*hand[:5]))


def place_card(player: TTPlayerType, hand_idx: int, board_x: int, board_y: int):
    err = lib.tt_place_card(player, hand_idx, board_x, board_y)
    print(err)
