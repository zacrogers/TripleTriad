import sys
import os
import json
import pygame
from triple_triad_core import (
    TripleTriadCore,
    TTError,
    TTPlayerType
)

clock = pygame.time.Clock()
# Initialize Pygame
pygame.init()

SCRIPT_DIR = os.path.dirname(os.path.abspath(__file__))


SCREEN_WIDTH = 800
SCREEN_HEIGHT = 700

# Colors
GREEN_FELT = (34, 139, 34)  # Background color

# Slot and card dimensions
CARD_WIDTH = 80
CARD_HEIGHT = 120
GRID_SIZE = 3  # 3x3 grid

# Padding and spacing
MARGIN = 10
SIDE_SLOT_SPACING = 20  # Space between side card slots
GRID_SPACING = 5        # Space between grid cells

# Calculate column positions
LEFT_COLUMN_X = MARGIN
RIGHT_COLUMN_X = SCREEN_WIDTH - CARD_WIDTH - MARGIN
CENTER_COLUMN_X = (SCREEN_WIDTH - (GRID_SIZE * (CARD_WIDTH + GRID_SPACING))) // 2

MAX_MOVES = 5
MAX_TURNS = (MAX_MOVES * 2)


class TripleTriadGame:
    def __init__(self, screen_width=SCREEN_WIDTH, screen_height=SCREEN_HEIGHT):
        self.screen_width = screen_width
        self.screen_height = screen_height

        self.hand_selected_index = 0

        self.curr_turn = 0
        self.curr_move = 0

        self.sim_data = None
        if self.load_sim_data():
            print("Loaded sim data")
            self.moves         = [self.sim_data["movesA"], self.sim_data["movesB"]]
            self.p1_hand       = self.sim_data["handA"]
            self.p2_hand       = self.sim_data["handB"]
            self.p1_hand_names = [TripleTriadCore.get_card_name(card_id) for card_id in self.p1_hand]
            self.p2_hand_names = [TripleTriadCore.get_card_name(card_id) for card_id in self.p2_hand]

            # Setup game
            TripleTriadCore.init_board()
            TripleTriadCore.set_hand(TTPlayerType.TT_PLAYER_A, self.p1_hand)
            TripleTriadCore.set_hand(TTPlayerType.TT_PLAYER_B, self.p2_hand)


        else:
            print("Failed to load sim data")
            exit(1)

        self.screen = pygame.display.set_mode((self.screen_width, self.screen_height))
        pygame.display.set_caption("Triple Triad")


    def load_sim_data(self, fname=os.path.join(SCRIPT_DIR, "sim_data.json")) -> bool:
        self.sim_data = None

        with open(fname, "r") as f:
            self.sim_data = json.loads(f.read())
            return True

        return False


    '''
        Drawing
    '''
    def draw_card(self, card_name, owner, x, y, selected=False):
        """Draw a card PNG at the specified position."""
        card_image = pygame.image.load(f"/Users/zacharyrogers/Code/TripleTriad/images/TT{card_name}.png")
        card_image = pygame.transform.scale(card_image, (CARD_WIDTH, CARD_HEIGHT))

        pad = 5
        if owner == 0:
            pygame.draw.rect(self.screen, 0xFF0000, [x-pad, y-pad, CARD_WIDTH+(pad*2), CARD_HEIGHT+(pad*2)])
        elif owner == 1:
            pygame.draw.rect(self.screen, 0x0000FF, [x-pad, y-pad, CARD_WIDTH+(pad*2), CARD_HEIGHT+(pad*2)])
        else:
            pygame.draw.rect(self.screen, 0xFFFFFF, [x-pad, y-pad, CARD_WIDTH+(pad*2), CARD_HEIGHT+(pad*2)])

        self.screen.blit(card_image, (x, y))


    def draw_grid(self, card_names, card_owners):
        """Draw the 9x9 grid in the center column with card images."""
        r= 0
        c = 0
        for row in range(GRID_SIZE):
            for col in range(GRID_SIZE):
                x = CENTER_COLUMN_X + col * (CARD_WIDTH + GRID_SPACING)
                y = MARGIN + row * (CARD_HEIGHT + GRID_SPACING)
                self.draw_card(card_names[c * 3 + r], card_owners[c * 3 + r], x, y)
                c += 1
            c = 0
            r += 1


    def draw_side_column(self, x, card_names, card_owner):
        """Draw a side column with 5 card slots."""
        for i in range(5):
            y = MARGIN + i * (CARD_HEIGHT + SIDE_SLOT_SPACING)
            self.draw_card(card_names[i], card_owner, x, y)


    def run(self):
        # Main loop
        running = True
        while running:
            for event in pygame.event.get():
                if event.type == pygame.QUIT:
                    pygame.quit()
                    sys.exit()

            s = TripleTriadCore.get_state()

            board_card_names  = [TripleTriadCore.get_card_name(card_id.get("id")) for card_id in s.get("board")]
            board_card_owners = [c.get("o") for c in s.get("board")]
            curr_player       = s.get("turn")

            # Fill the background
            self.screen.fill(GREEN_FELT)

            # Draw the left column, center grid, and right column with PNGs
            self.draw_side_column(LEFT_COLUMN_X, self.p1_hand_names, 0)
            self.draw_grid(board_card_names, board_card_owners)
            self.draw_side_column(RIGHT_COLUMN_X, self.p2_hand_names, 1)

            # assert(curr_player == sim_data["expectedOutput"][curr_move]["turn"])
            print(f"asdsda: {curr_player}, {self.sim_data["expectedOutput"][self.curr_turn]["turn"]}")

            err = TripleTriadCore.place_card(
                curr_player,
                self.moves[curr_player][self.curr_move][0],
                self.moves[curr_player][self.curr_move][1],
                self.moves[curr_player][self.curr_move][2]
            )

            if not TripleTriadCore.update():
                print("nah")
                # exit(0)
            else:
                self.curr_turn += 1
                self.curr_move += 1

                if self.curr_move >= MAX_MOVES:
                    self.curr_move = 0

            # Update the display
            pygame.display.flip()
            clock.tick(1)

if __name__ == "__main__":
    tt = TripleTriadGame()
    tt.run()