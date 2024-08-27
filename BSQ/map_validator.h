#ifndef MAP_VALIDATOR_H
#define MAP_VALIDATOR_H

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

int get_line_length(char *buffer, int start, int buffer_size);
int validate_first_line(char *buffer, int *rows, char *empty, char *obstacle, char *full);
int validate_map_lines(char *buffer, int bytes_read, int start, int rows, int cols, char empty, char obstacle, char full);
int validate_map(const char *filename);
int **transform_map(char **char_map, int rows, int cols, char empty, char obstacle);
void free_int_map(int **int_map, int rows);

#endif
