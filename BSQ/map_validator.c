#include <unistd.h>  // For read, write, close
#include <fcntl.h>   // For open flags
#include <stdlib.h>  // For malloc, free, exit
#include "map_validator.h"

#define BUFFER_SIZE 4096


// Function to get the length of a line (until newline or end of buffer)
int get_line_length(char *buffer, int start, int buffer_size) {
    int length = 0;
    while (start + length < buffer_size && buffer[start + length] != '\n') {
        length++;
    }
    return length;
}

// Function to validate the first line of the map
int validate_first_line(char *buffer, int *rows, char *empty, char *obstacle, char *full) {
    int i = 0;
    *rows = 0;

    // Extract the number of rows
    while (buffer[i] >= '0' && buffer[i] <= '9') {
        *rows = (*rows * 10) + (buffer[i] - '0');
        i++;
    }

    // Extract the map characters
    *empty = buffer[i++];
    *obstacle = buffer[i++];
    *full = buffer[i++];

    // Check if all characters are distinct
    if (*empty == *obstacle || *obstacle == *full || *empty == *full) {
        write(2, "map error\n", 10);
        return -1;
    }

    // Ensure the first line ends with a newline
    if (buffer[i] != '\n') {
        write(2, "map error\n", 10);
        return -1;
    }

    return i + 1; // Return the index to continue reading the map
}

// Function to validate the map lines
int validate_map_lines(char *buffer, int bytes_read, int start, int rows, int cols, char empty, char obstacle, char full) {
    int row = 0;
    int col = 0;

    for (int i = start; i < bytes_read; i++) {
        if (buffer[i] == '\n') {
            if (col != cols) { // Ensure all lines have the same length
                write(2, "map error\n", 10);
                return -1;
            }
            col = 0;
            row++;
            if (row > rows) {
                write(2, "map error\n", 10);
                return -1;
            }
        } else if (buffer[i] != empty && buffer[i] != obstacle && buffer[i] != full) {
            write(2, "map error\n", 10);
            return -1;
        } else {
            col++;
        }
    }

    // Ensure the number of rows matches the given number of rows
    if (row != rows) {
        write(2, "map error\n", 10);
        return -1;
    }

    return 0; // Map is valid
}

// Function to validate the map
int validate_map(const char *filename) {
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

    int rows, cols;
    char empty, obstacle, full;

    // Validate the first line
    int start = validate_first_line(buffer, &rows, &empty, &obstacle, &full);
    if (start == -1) {
        close(fd);
        return -1;
    }

    // Calculate the number of columns based on the first map line
    cols = get_line_length(buffer, start, bytes_read);

    // Validate the map lines
    if (validate_map_lines(buffer, bytes_read, start, rows, cols, empty, obstacle, full) == -1) {
        close(fd);
        return -1;
    }

    close(fd);
    return 0;
}
