# Define the compiler and compiler flags
CC = gcc
CFLAGS = -Wall -Wextra -Werror

# Define the target executable
TARGET = bsq

# Define the source files
SRCS = main.c map_validator.c map_transform.c 

# Define the object files
OBJS = $(SRCS:.c=.o)

# The default rule (what happens when you run `make`)
all: $(TARGET)

# Rule to link the object files into the final executable
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS)

# Rule to compile source files into object files
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Clean up object files and the executable
clean:
	rm -f $(OBJS) $(TARGET)
