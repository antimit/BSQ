#include <unistd.h>  // For read, write, close, and access modes
#include <fcntl.h>   // For open flags
#include <stdlib.h>  // For malloc, free, exit
#include "map_validator.h"


#define BUFFER_SIZE 4096

// Function to parse the map from a file
char **parse_map(const char *filename, int *rows, int *cols, char *empty, char *obstacle, char *full) {
    int fd = open(filename, O_RDONLY);
    if (fd == -1) {
        write(2, "Error opening file\n", 19);
        exit(EXIT_FAILURE);
    }

    char buffer[BUFFER_SIZE];
    int bytes_read = read(fd, buffer, BUFFER_SIZE);
    if (bytes_read <= 0) {
        write(2, "Error reading file\n", 19);
        close(fd);
        exit(EXIT_FAILURE);
    }

    // Parse the first line
    int i = 0;
    *rows = 0;
    while (buffer[i] >= '0' && buffer[i] <= '9') {
        *rows = (*rows * 10) + (buffer[i] - '0');
        i++;
    }

    *empty = buffer[i++];
    *obstacle = buffer[i++];
    *full = buffer[i++];

    if (buffer[i] != '\n') {
        write(2, "Invalid map format\n", 19);
        close(fd);
        exit(EXIT_FAILURE);
    }
    i++; // Skip the newline

    // Calculate the number of columns by finding the length of the next line
    *cols = get_line_length(buffer, i, bytes_read);

    // Allocate memory for the map
    char **map = (char **)malloc(sizeof(char *) * (*rows));
    for (int j = 0; j < *rows; j++) {
        map[j] = (char *)malloc(sizeof(char) * (*cols + 1)); // +1 for the null terminator
    }

    // Copy the map data into the 2D array
    int row = 0;
    int col = 0;
    for (; i < bytes_read; i++) {
        if (buffer[i] == '\n') {
            map[row][col] = '\0'; // Null terminate the line
            row++;
            col = 0;
        } else {
            map[row][col++] = buffer[i];
        }
        if (row >= *rows) break;
    }

    close(fd);
    return map;
}









// Function to find the largest square sub-matrix of 1s
void find_largest_square(int **int_map, int rows, int cols, int *max_size, int *max_row, int *max_col) {
    int **dp = (int **)malloc(rows * sizeof(int *));
    for (int i = 0; i < rows; i++) {
        dp[i] = (int *)malloc(cols * sizeof(int));
    }

    *max_size = 0;
    *max_row = 0;
    *max_col = 0;

    // Initialize the first row and column of the dp array
    for (int i = 0; i < rows; i++) {
        dp[i][0] = int_map[i][0];
        if (dp[i][0] > *max_size) {
            *max_size = dp[i][0];
            *max_row = i;
            *max_col = 0;
        }
    }
    for (int j = 0; j < cols; j++) {
        dp[0][j] = int_map[0][j];
        if (dp[0][j] > *max_size) {
            *max_size = dp[0][j];
            *max_row = 0;
            *max_col = j;
        }
    }

    // Fill the dp array and find the maximum square
    for (int i = 1; i < rows; i++) {
        for (int j = 1; j < cols; j++) {
            if (int_map[i][j] == 1) {
                dp[i][j] = 1 + (dp[i - 1][j] < dp[i][j - 1] ? (dp[i - 1][j] < dp[i - 1][j - 1] ? dp[i - 1][j] : dp[i - 1][j - 1]) : (dp[i][j - 1] < dp[i - 1][j - 1] ? dp[i][j - 1] : dp[i - 1][j - 1]));
                if (dp[i][j] > *max_size) {
                    *max_size = dp[i][j];
                    *max_row = i;
                    *max_col = j;
                }
            } else {
                dp[i][j] = 0;
            }
        }
    }

    // Free the dp array
    for (int i = 0; i < rows; i++) {
        free(dp[i]);
    }
    free(dp);
}

// Function to mark the largest square on the map
void mark_largest_square(char **map, int max_size, int max_row, int max_col, char full) {
    for (int i = max_row - max_size + 1; i <= max_row; i++) {
        for (int j = max_col - max_size + 1; j <= max_col; j++) {
            map[i][j] = full;
        }
    }
}



int main(int argc, char **argv) {
    if (argc != 2) {
        write(2, "Usage: ./bsq <map file>\n", 24);
        exit(EXIT_FAILURE);
    }

    // Validate the map
    if (validate_map(argv[1]) == -1) {
        return 1; // Invalid map, exit the program
    }

    char **map;
    int rows, cols;
    char empty, obstacle, full;

    // Parse the map file
    map = parse_map(argv[1], &rows, &cols, &empty, &obstacle, &full);

    // Transform character map to integer map
    int **int_map = transform_map(map, rows, cols, empty, obstacle);

    // Find the largest square sub-matrix
    int max_size, max_row, max_col;
    find_largest_square(int_map, rows, cols, &max_size, &max_row, &max_col);

    // Mark the largest square on the map
    mark_largest_square(map, max_size, max_row, max_col, full);

    // Output the updated map
    for (int i = 0; i < rows; i++) {
        write(1, map[i], cols);
        write(1, "\n", 1);
    }

    // Free the allocated memory
    for (int i = 0; i < rows; i++) {
        free(map[i]);
    }
    free(map);
    free_int_map(int_map, rows);

    return 0;
}
