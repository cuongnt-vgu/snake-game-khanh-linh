#include "common.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

// TODO: define global variables needed for your snake! (part 1A)

// Definition of global variables for game status.
int g_game_over = 0;
int g_score;
enum Snake_Direction back_direction[5] = {DOWN, UP, RIGHT, LEFT, NONE};

int g_name_len;
char g_name[1000];

enum Snake_Direction input_key_to_snake_direction(enum input_key key) {
    if (key == INPUT_UP) return UP;
    if (key == INPUT_DOWN) return DOWN;
    if (key == INPUT_LEFT) return LEFT;
    if (key == INPUT_RIGHT) return RIGHT;
    return NONE;
}

// 1A
int get_id_cell(Position pos, int width) {
    return pos.x + pos.y * (int)width;
}

void grown_snake_with_pos(snake_t *snake, Position pos, enum Snake_Direction direction) {
    node_t *new_pos = malloc(sizeof(node_t));
    node_t* new_direction = malloc(sizeof(node_t));
    new_pos->data = malloc(sizeof(Position));
    new_direction->data = malloc(sizeof(enum Snake_Direction));
    *((Position*)new_pos->data) = pos;
    *((enum Snake_Direction*)new_direction->data) = direction;
    new_pos->next = NULL;
    new_direction->next = NULL;
    if (snake->length == 0) {
        snake->head_pos = new_pos;
        snake->head_direction = new_direction;
        snake->tail_pos = new_pos;
        snake->tail_direction = new_direction;
        snake->length++;
        return;
    }
    new_pos->prev = snake->tail_pos;
    new_direction->prev = snake->tail_direction;
    snake->tail_pos->next = new_pos;
    snake->tail_direction->next = new_direction;
    snake->tail_pos = new_pos;
    snake->tail_direction = new_direction;
    snake->length++;
}

void grown_snake(snake_t *snake) {
    Position pos = *((Position*)snake->tail_pos->data);
    enum Snake_Direction direction = *((enum Snake_Direction*)snake->tail_direction->data);
    if (direction == UP) {
        pos.y++;
    } else if (direction == DOWN) {
        pos.y--;
    } else if (direction == LEFT) {
        pos.x++;
    } else if (direction == RIGHT) {
        pos.x--;
    }
    grown_snake_with_pos(snake, pos, direction);
}

void update_snake_direction(snake_t *snake, enum Snake_Direction direction) {
    // node_t *curr = snake->head_direction->next;
    // while (curr != NULL) {
    //     *((enum Snake_Direction*)curr->data) = *((enum Snake_Direction*)curr->prev->data);
    //     curr = curr->next;
    // }
    // *((enum Snake_Direction*)snake->head_direction->data) = direction;
    node_t *curr = snake->tail_direction;
    while (curr != snake->head_direction) {
        *((enum Snake_Direction*)curr->data) = *((enum Snake_Direction*)curr->prev->data);
        curr = curr->prev;
    }
    if (direction == NONE) return;
    if (snake->length != 1 && direction == back_direction[*((enum Snake_Direction*)snake->head_direction->data)]) return;
    *((enum Snake_Direction*)snake->head_direction->data) = direction;
}

void update_snake_position(snake_t *snake) {
    node_t *curr_pos = snake->head_pos;
    node_t *curr_direction = snake->head_direction;
    while (curr_pos != NULL) {
        Position *pos = (Position*)curr_pos->data;
        enum Snake_Direction *direction = (enum Snake_Direction*)curr_direction->data;
        switch (*direction) {
            case UP:
                pos->y--;
                break;
            case DOWN:
                pos->y++;
                break;
            case LEFT:
                pos->x--;
                break;
            case RIGHT:
                pos->x++;
                break;
            default:
                break;
        }
        curr_pos = curr_pos->next;
        curr_direction = curr_direction->next;
    }
}

bool update_snake_in_board(int* cells, int width, int height, snake_t* snake_p, int snake_grows, bool init) {
    node_t *curr_pos = snake_p->head_pos;
    bool more_food = false;
    Position *pos = (Position*)curr_pos->data;
    if (pos->x < 0 || pos->x >= width || pos->y < 0 || pos->y >= height) {
        g_game_over = 1;
        return more_food;
    }
    int id = get_id_cell(*pos, width);
    if (cells[id] == FLAG_FOOD) {
        g_score++;
        if (snake_grows) grown_snake(snake_p);
        more_food = true;
    }
    // if (more_food && snake_grows) return more_food;
    // if (init) return more_food;
    if (*((enum Snake_Direction*)snake_p->head_direction->data) != NONE && cells[id] == FLAG_WALL) {
        g_game_over = 1;
        return more_food;
    }
    int save_plain_cell_id = -1;
    if ((!more_food || !snake_grows) && !init) {
        Position last_cell = *((Position*)snake_p->tail_pos->data);
        enum Snake_Direction last_direction = *((enum Snake_Direction*)snake_p->tail_direction->data);
        if (last_direction == UP) {
            last_cell.y++;
        } else if (last_direction == DOWN) {
            last_cell.y--;
        } else if (last_direction == LEFT) {
            last_cell.x++;
        } else if (last_direction == RIGHT) {
            last_cell.x--;
        }
        id = get_id_cell(last_cell, width);
        // cells[id] = FLAG_PLAIN_CELL;
        save_plain_cell_id = id;
    }
    id = get_id_cell(*pos, width);
    if (*((enum Snake_Direction*)snake_p->head_direction->data) != NONE && cells[id] == FLAG_SNAKE && id != save_plain_cell_id) {
        g_game_over = 1;
        return more_food;
    }
    if (save_plain_cell_id != -1) cells[save_plain_cell_id] = FLAG_PLAIN_CELL;
    
    while (curr_pos != NULL) {
        pos = (Position*)curr_pos->data;
        id = get_id_cell(*pos, width);
        cells[id] = FLAG_SNAKE;
        curr_pos = curr_pos->next;
    }
    
    return more_food;
}

/** Sets the seed for random number generation.
 * Arguments:
 *  - `seed`: the seed.
 */
void set_seed(unsigned seed) {
    /* DO NOT MODIFY THIS FUNCTION */
    srand(seed);
    /* DO NOT MODIFY THIS FUNCTION */
}

/** Returns a random index in [0, size)
 * Arguments:
 *  - `size`: the upper bound for the generated value (exclusive).
 */
unsigned generate_index(unsigned size) {
    /* DO NOT MODIFY THIS FUNCTION */
    return rand() % size;
    /* DO NOT MODIFY THIS FUNCTION */
}
