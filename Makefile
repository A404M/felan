PROJECT_NAME := felan

CC := gcc
# CC := tcc

BUILD_DIR := ./build
SRC_DIR := ./src

SRCS := $(shell find $(SRC_DIR) -name "*.c" -type f)
HEADERS := $(shell find $(SRC_DIR) -name "*.h" -type f)
OBJS := $(SRCS:%=$(BUILD_DIR)/%.o)

RED := \033[0;31m
GREEN := \033[0;32m
NC := \033[0m

# INC_DIRS := $(shell find $(SRC_DIRS) -type d)
INC_DIRS := $(SRC_DIR)
INC_FLAGS := $(addprefix -I,$(INC_DIRS))

# CFLAGS := $(INC_FLAGS) -Wall -Wextra -std=gnu23 -O3
# CFLAGS := $(INC_FLAGS) -Wall -Wextra -std=gnu23 -Oz
CFLAGS := $(INC_FLAGS) -Wall -Wextra -std=gnu23 -g

EXEC_FILE := $(BUILD_DIR)/$(PROJECT_NAME)

all: $(EXEC_FILE)

$(EXEC_FILE): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $@ $(LDFLAGS)

$(BUILD_DIR)/%.c.o: %.c $(HEADERS) Makefile
	mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

lsp-files: clean
	bear -- make all

.PHONY: clean
clean:
	rm -rf $(BUILD_DIR)

run: $(EXEC_FILE)
	$(EXEC_FILE) $(args)

val-run: $(EXEC_FILE)
	valgrind --log-file="val.log" --leak-check=full --track-origins=yes --show-leak-kinds=all -s $(EXEC_FILE) $(args)

gdb-run: $(EXEC_FILE)
	gdb $(EXEC_FILE) $(args)

.PHONY: test
test: $(EXEC_FILE)
	$(EXEC_FILE) test/main.felan build/out

val-test: $(EXEC_FILE)
	valgrind --log-file="val.log" --leak-check=full --track-origins=yes --show-leak-kinds=all -s $(EXEC_FILE) test/main.felan build/out

gdb-test: $(EXEC_FILE)
	gdb $(EXEC_FILE) test/main.felan build/out

# $@ = left hand of :
# $< = right hand of : first one of them
# $^ = right hand of : all
