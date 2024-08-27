#include <stdlib.h>  // For malloc, free, exit
#include "map_validator.h"  // Include header for function prototypes

// Function to transform a character map to an integer map
int **transform_map(char **char_map, int rows, int cols, char empty, char obstacle) {
    // Allocate memory for the integer map
    int **int_map = (int **)malloc(sizeof(int *) * rows);
    if (int_map == NULL) {
        write(2, "Memory allocation error\n", 24);
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < rows; i++) {
        int_map[i] = (int *)malloc(sizeof(int) * cols);
        if (int_map[i] == NULL) {
            write(2, "Memory allocation error\n", 24);
            exit(EXIT_FAILURE);
        }
    }

    // Convert the character map to an integer map
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            if (char_map[i][j] == empty) {
                int_map[i][j] = 1;
            } else if (char_map[i][j] == obstacle) {
                int_map[i][j] = 0;
            } else {
                write(2, "Invalid character in map\n", 25);
                exit(EXIT_FAILURE);
            }
        }
    }

    return int_map;
}

// Function to free the integer map
void free_int_map(int **int_map, int rows) {
    for (int i = 0; i < rows; i++) {
        free(int_map[i]);
    }
    free(int_map);
}
