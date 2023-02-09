CC = gcc
CFLAGS = -Wall -Wextra -Werror
CPPFLAGS = -I src -I thirdparty
# LDFLAGS =
# LDLIBS =

APP_NAME = SimpleComputer
LIB_COMPUTER_NAME = libcomputer
LIB_TERM_NAME = libterm
LIB_BIGCHAR = libbigchar
TEST_NAME = test

OBJ_DIR = obj
OBJ_SRC_DIR = $(OBJ_DIR)/src
OBJ_TEST_DIR = $(OBJ_DIR)/test

LIB_DIR = lib
BIN_DIR = bin
SRC_DIR = src
TEST_DIR = test

APP_PATH = $(BIN_DIR)/$(APP_NAME)
LIB_COMPUTER_PATH = $(LIB_DIR)/$(LIB_COMPUTER_NAME).a
LIB_TERM_PATH = $(LIB_DIR)/$(LIB_TERM_NAME).a
LIB_BIGCHAR_PATH = $(LIB_DIR)/$(LIB_BIGCHAR_NAME).a
TEST_PATH = $(BIN_DIR)/$(TEST_NAME)

SRC_EXT = c

APP_SOURCES = $(shell find $(SRC_DIR)/$(APP_NAME) -name '*.$(SRC_EXT)')
APP_OBJECTS = $(APP_SOURCES:$(SRC_DIR)/%.$(SRC_EXT)=$(OBJ_DIR)/$(SRC_DIR)/%.o)

LIB_COMPUTER_SOURCES = $(shell find $(SRC_DIR)/$(LIB_COMPUTER_NAME) -name '*.$(SRC_EXT)')
LIB_COMPUTER_OBJECTS = $(LIB_COMPUTER_SOURCES:$(SRC_DIR)/%.$(SRC_EXT)=$(OBJ_DIR)/$(SRC_DIR)/%.o)

LIB_TERM_SOURCES = $(shell find $(SRC_DIR)/$(LIB_TERM_NAME) -name '*.$(SRC_EXT)')
LIB_TERM_OBJECTS = $(LIB_TERM_SOURCES:$(SRC_DIR)/%.$(SRC_EXT)=$(OBJ_DIR)/$(SRC_DIR)/%.o)

LIB_BIGCHAR_SOURCES = $(shell find $(SRC_DIR)/$(LIB_BIGCHAR_NAME) -name '*.$(SRC_EXT)')
LIB_BIGCHAR_OBJECTS = $(LIB_BIGCHAR_SOURCES:$(SRC_DIR)/%.$(SRC_EXT)=$(OBJ_DIR)/$(SRC_DIR)/%.o)

TEST_SOURCES = $(shell find $(TEST_DIR) -name '*.$(SRC_EXT)')
TEST_OBJECTS = $(TEST_SOURCES:$(TEST_DIR)/%.$(SRC_EXT)=$(OBJ_TEST_DIR)/%.o)

DEPS=$(APP_OBJECTS:.o=.d)$(LIB_COMPUTER_SOURCES:.o=.d)$(LIB_TERM_OBJECTS:.o=.d)$(TEST_OBJECTS:.o=.d)

.PHONY: all
all: $(APP_PATH)

.PHONY: test
test: $(APP_PATH) $(TEST_PATH)

-include $(DEPS)

$(APP_PATH): $(APP_OBJECTS) $(LIB_COMPUTER_PATH) $(LIB_TERM_PATH) $(LIB_BIGCHAR_PATH)
	$(CC) $(CFLAGS) $(CPPFLAGS) $^ -o $@

$(TEST_PATH): $(TEST_OBJECTS) $(LIB_COMPUTER_PATH) $(LIB_TERM_PATH) $(LIB_BIGCHAR_PATH)
	$(CC) $(CFLAGS) $(CPPFLAGS) $^ -o $@

$(LIB_COMPUTER_PATH): $(LIB_COMPUTER_OBJECTS)
	ar rcs $@ $^

$(LIB_TERM_PATH): $(LIB_TERM_OBJECTS)
	ar rcs $@ $^

$(LIB_BIGCHAR_PATH): $(LIB_BIGCHAR_OBJECTS)
	ar rcs $@ $^

$(OBJ_DIR)/%.o: %.c
	$(CC) -c $(CFLAGS) $(CPPFLAGS) $< -o $@

run: $(APP_PATH)
	$(APP_PATH)

test_run: $(TEST_PATH)
	$(TEST_PATH)

.PHONY: clean
clean:
	$(RM) $(APP_PATH) $(LIB_COMPUTER_PATH) $(LIB_TERM_PATH) $(TEST_PATH) $(LIB_BIGCHAR_PATH)
	find $(OBJ_DIR) -name '*.o' -exec $(RM) '{}' \;
	find $(OBJ_DIR) -name '*.d' -exec $(RM) '{}' \;
