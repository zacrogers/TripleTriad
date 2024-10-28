# import ctypes
# from pprint import pprint
# import json

import triple_triad as tt

import pygame
import sys
clock = pygame.time.Clock()
# Initialize Pygame
pygame.init()

# Screen dimensions and setup
SCREEN_WIDTH = 800
SCREEN_HEIGHT = 700
screen = pygame.display.set_mode((SCREEN_WIDTH, SCREEN_HEIGHT))
pygame.display.set_caption("Triple Triad")

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

# Load PNG image for the card
# card_image = pygame.image.load("/Users/zacharyrogers/Code/TripleTriad/images/TTCactuar.png")
# card_image = pygame.transform.scale(card_image, (CARD_WIDTH, CARD_HEIGHT))

# Calculate column positions
LEFT_COLUMN_X = MARGIN
RIGHT_COLUMN_X = SCREEN_WIDTH - CARD_WIDTH - MARGIN
CENTER_COLUMN_X = (SCREEN_WIDTH - (GRID_SIZE * (CARD_WIDTH + GRID_SPACING))) // 2

def draw_card(card_name, owner, x, y):
    """Draw a card PNG at the specified position."""
    card_image = pygame.image.load(f"/Users/zacharyrogers/Code/TripleTriad/images/TT{card_name}.png")
    card_image = pygame.transform.scale(card_image, (CARD_WIDTH, CARD_HEIGHT))

    pad = 5
    if owner == 0:
        pygame.draw.rect(screen, 0xFF0000, [x-pad, y-pad, CARD_WIDTH+(pad*2), CARD_HEIGHT+(pad*2)])
    elif owner == 1:
        pygame.draw.rect(screen, 0x0000FF, [x-pad, y-pad, CARD_WIDTH+(pad*2), CARD_HEIGHT+(pad*2)])
    else:
        pygame.draw.rect(screen, 0xFFFFFF, [x-pad, y-pad, CARD_WIDTH+(pad*2), CARD_HEIGHT+(pad*2)])

    screen.blit(card_image, (x, y))


def draw_grid(card_names, card_owners):
    """Draw the 9x9 grid in the center column with card images."""
    r= 0
    c = 0
    for row in range(GRID_SIZE):
        for col in range(GRID_SIZE):
            x = CENTER_COLUMN_X + col * (CARD_WIDTH + GRID_SPACING)
            y = MARGIN + row * (CARD_HEIGHT + GRID_SPACING)
            draw_card(card_names[c * 3 + r], card_owners[c * 3 + r], x, y)
            c += 1
        c = 0
        r += 1

def draw_side_column(x, card_names, card_owner):
    """Draw a side column with 5 card slots."""
    for i in range(5):
        y = MARGIN + i * (CARD_HEIGHT + SIDE_SLOT_SPACING)
        draw_card(card_names[i], card_owner, x, y)

images = []


# Setup game
tt.init_board()
tt.set_hand(tt.TTPlayerType.TT_PLAYER_A, [11, 12, 13, 14, 15])
tt.set_hand(tt.TTPlayerType.TT_PLAYER_B, [16, 17, 18, 75, 19])

player_moves = [
    [0, 0, 0],
    [0, 1, 2],
    [0, 2, 0],
    [0, 2, 1],
    [0, 2, 2]
]

enemy_moves = [
    [0, 0, 1],
    [0, 1, 1],
    [0, 0, 2],
    [0, 1, 0],
    [0, 2, 2]
]

moves = [
    player_moves,
    enemy_moves
]

MAX_MOVES = 5
curr_move = 0

def load_moves():
    ...

# Main loop
running = True
while running:
    for event in pygame.event.get():
        if event.type == pygame.QUIT:
            pygame.quit()
            sys.exit()

    s = tt.get_state()
    p1_hand_names     = [tt.get_card_name(c_id) for c_id in s.get("handA")]
    p2_hand_names     = [tt.get_card_name(c_id) for c_id in s.get("handB")]
    board_card_names  = [tt.get_card_name(c_id.get("id")) for c_id in s.get("board")]
    board_card_owners = [c.get("o") for c in s.get("board")]
    curr_player       = s.get("turn")

    # Fill the background
    screen.fill(GREEN_FELT)

    # Draw the left column, center grid, and right column with PNGs
    draw_side_column(LEFT_COLUMN_X, p1_hand_names, 0)
    draw_grid(board_card_names, board_card_owners)
    draw_side_column(RIGHT_COLUMN_X, p2_hand_names, 1)

    err = tt.place_card(
        curr_player,
        moves[curr_player][curr_move][0],
        moves[curr_player][curr_move][1],
        moves[curr_player][curr_move][2]
    )

    if not tt.update_game():
        print("nah")
        # exit(0)

    curr_move += 1

    if curr_move >= MAX_MOVES:
        curr_move = 0

    # Update the display
    pygame.display.flip()
    clock.tick(1)