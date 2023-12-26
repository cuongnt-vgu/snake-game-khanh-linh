#include "game_setup.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>


// Some handy dandy macros for decompression
#define E_CAP_HEX 0x45
#define E_LOW_HEX 0x65
#define S_CAP_HEX 0x53
#define S_LOW_HEX 0x73
#define W_CAP_HEX 0x57
#define W_LOW_HEX 0x77
#define DIGIT_START 0x30
#define DIGIT_END 0x39

/** Initializes the board with walls around the edge of the board.
 *
 * Modifies values pointed to by cells_p, width_p, and height_p and initializes
 * cells array to reflect this default board.
 *
 * Returns INIT_SUCCESS to indicate that it was successful.
 *
 * Arguments:
 *  - cells_p: a pointer to a memory location where a pointer to the first
 *             element in a newly initialized array of cells should be stored.
 *  - width_p: a pointer to a memory location where the newly initialized
 *             width should be stored.
 *  - height_p: a pointer to a memory location where the newly initialized
 *              height should be stored.
 */
enum board_init_status initialize_default_board(int** cells_p, size_t* width_p,
                                                size_t* height_p) {
    *width_p = 20;
    *height_p = 10;
    int* cells = malloc(20 * 10 * sizeof(int));
    *cells_p = cells;
    for (int i = 0; i < 20 * 10; i++) {
        cells[i] = FLAG_PLAIN_CELL;
    }

    // Set edge cells!
    // Top and bottom edges:
    for (int i = 0; i < 20; ++i) {
        cells[i] = FLAG_WALL;
        cells[i + (20 * (10 - 1))] = FLAG_WALL;
    }
    // Left and right edges:
    for (int i = 0; i < 10; ++i) {
        cells[i * 20] = FLAG_WALL;
        cells[i * 20 + 20 - 1] = FLAG_WALL;
    }

    // Add snake
    // cells[20 * 2 + 2] = FLAG_SNAKE;
    return INIT_SUCCESS;
}

/** Initialize variables relevant to the game board.
 * Arguments:
 *  - cells_p: a pointer to a memory location where a pointer to the first
 *             element in a newly initialized array of cells should be stored.
 *  - width_p: a pointer to a memory location where the newly initialized
 *             width should be stored.
 *  - height_p: a pointer to a memory location where the newly initialized
 *              height should be stored.
 *  - snake_p: a pointer to your snake struct (not used until part 2!)
 *  - board_rep: a string representing the initial board. May be NULL for
 * default board.
 */
enum board_init_status initialize_game(int** cells_p, size_t* width_p,
                                       size_t* height_p, snake_t* snake_p,
                                       char* board_rep) {
    // TODO: implement!
    enum board_init_status status;
    snake_p->length = 0;
    snake_p->head_pos = NULL;
    snake_p->head_direction = NULL;
    if (board_rep == NULL) {
        status = initialize_default_board(cells_p, width_p, height_p);
        grown_snake_with_pos(snake_p, (Position){2, 2}, RIGHT);
    }
    else {
        status = decompress_board_str(cells_p, width_p, height_p, snake_p, board_rep);
    }
    if (status != INIT_SUCCESS) return status;
    update_snake_in_board(*cells_p, *width_p, *height_p, snake_p, 0, true);
    place_food(*cells_p, *width_p, *height_p);
    // printf("game over:%d\n", g_game_over);
    // while (true) {}
    return status;
}

/** Takes in a string `compressed` and initializes values pointed to by
 * cells_p, width_p, and height_p accordingly. Arguments:
 *      - cells_p: a pointer to the pointer representing the cells array
 *                 that we would like to initialize.
 *      - width_p: a pointer to the width variable we'd like to initialize.
 *      - height_p: a pointer to the height variable we'd like to initialize.
 *      - snake_p: a pointer to your snake struct (not used until part 2!)
 *      - compressed: a string that contains the representation of the board.
 * Note: We assume that the string will be of the following form:
 * B24x80|E5W2E73|E5W2S1E72... To read it, we scan the string row-by-row
 * (delineated by the `|` character), and read out a letter (E, S or W) a number
 * of times dictated by the number that follows the letter.
 */

int read_number(char *compressed, int *i) {
    int num = 0;
    while (compressed[*i] >= DIGIT_START && compressed[*i] <= DIGIT_END) {
        num = num * 10 + (compressed[*i] - DIGIT_START);
        (*i)++;
        if (*i >= (int)strlen(compressed)) break;
    }
    return num;
}

enum board_init_status decompress_board_str(int** cells_p, size_t* width_p,
                                            size_t* height_p, snake_t* snake_p,
                                            char* compressed) {
    // TODO: implement!
    int len = strlen(compressed);
    if (len == 0 || compressed[0] != 'B') return INIT_ERR_BAD_CHAR;
    int i = 1;
    *height_p = read_number(compressed, &i);
    if (i >= len || compressed[i++] != 'x') return INIT_ERR_BAD_CHAR;
    *width_p = read_number(compressed, &i);
    if (i >= len || compressed[i] != '|') return INIT_ERR_BAD_CHAR;
    if (*width_p == 0 || *height_p == 0) return INIT_ERR_INCORRECT_DIMENSIONS;
    int *cells = malloc(*width_p * *height_p * sizeof(int));
    *cells_p = cells;
    int check_height = *height_p;
    int pos_cell = 0;
    while (i + 1 < len) {
        check_height--;
        if (check_height < 0) return INIT_ERR_INCORRECT_DIMENSIONS;
        i++;
        int check_width = *width_p;
        // printf("ok\n");
        while (i < len && compressed[i] != '|') {
            char type = compressed[i++];
            if (type != 'E' && type != 'S' && type != 'W') return INIT_ERR_BAD_CHAR;
            int num = read_number(compressed, &i);
            if (num == 0) return INIT_ERR_BAD_CHAR;
            check_width -= num;
            if (check_width < 0) return INIT_ERR_INCORRECT_DIMENSIONS;
            for (int cnt = 0; cnt < num; cnt++) {
                if (type == 'E') {
                    cells[pos_cell++] = FLAG_PLAIN_CELL;
                } else if (type == 'S') {
                    if (snake_p->head_direction != NULL) return INIT_ERR_WRONG_SNAKE_NUM;
                    // cells[pos_cell] = FLAG_SNAKE;
                    grown_snake_with_pos(snake_p, (Position){pos_cell % *width_p, pos_cell / *width_p}, RIGHT);
                    pos_cell++;
                } else if (type == 'W') {
                    cells[pos_cell++] = FLAG_WALL;
                }
            }
        }
        if (check_width != 0) return INIT_ERR_INCORRECT_DIMENSIONS;
    }
    if (snake_p->head_direction == NULL) return INIT_ERR_WRONG_SNAKE_NUM;
    if (check_height != 0) return INIT_ERR_INCORRECT_DIMENSIONS;
    if (i != len) {
        // printf("i:%d, len:%d\n", i, len);
        return INIT_ERR_BAD_CHAR;
    }
    return INIT_SUCCESS;
}
