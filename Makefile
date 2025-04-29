PROJECT_NAME := felan

CC := gcc
# CC := tcc
# CC := clang

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

# OP_FLAG := -Ofast
# OP_FLAG := -O3
# OP_FLAG := -Oz
OP_FLAG := -g

# CFLAGS := $(INC_FLAGS) -Wall -Wextra -lm -std=gnu23 -DPRINT_STATISTICS -DPRINT_COMPILE_TREE $(OP_FLAG)
CFLAGS := $(INC_FLAGS) -Wall -Wextra -lm -std=gnu23 -DPRINT_STATISTICS $(OP_FLAG)
# CFLAGS := $(INC_FLAGS) -Wall -Wextra -lm -std=gnu23 $(OP_FLAG)

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
	$(EXEC_FILE) code/main.felan build/out

val-test: $(EXEC_FILE)
	valgrind --log-file="val.log" --leak-check=full --track-origins=yes --show-leak-kinds=all -s $(EXEC_FILE) code/main.felan build/out

gdb-test: $(EXEC_FILE)
	gdb $(EXEC_FILE) code/main.felan build/out

test-all: $(EXEC_FILE) test/big.felan
	$(EXEC_FILE) test/big.felan

test/big.felan: Makefile
	mkdir -p test
	echo "main :: () -> void {" > $@
	for((n = 0;n < 1000000;n++)); do echo "  print('1');" >> $@; done
	echo "};" >> $@
	echo "print :: (value:u8) -> void {" >> $@
	echo "  putc value;" >> $@
	echo "};" >> $@

# $@ = left hand of :
# $< = right hand of : first one of them
# $^ = right hand of : all
