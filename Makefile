CC = gcc
CFLAGS = -Wall -Wextra -Werror
CPPFLAGS = -I src -I thirdparty
# LDFLAGS =
# LDLIBS =

APP_NAME = SimpleComputer
LIB_NAME = libcomputer
LIB_TERM_NAME = libterm

OBJ_DIR = obj
OBJ_SRC_DIR = $(OBJ_DIR)/src
OBJ_TEST_DIR = $(OBJ_DIR)/test

LIB_DIR = lib
BIN_DIR = bin
SRC_DIR = src

APP_PATH = $(BIN_DIR)/$(APP_NAME)
LIB_PATH = $(LIB_DIR)/$(LIB_NAME).a
LIB_TERM_PATH = $(LIB_DIR)/$(LIB_TERM_NAME).a

SRC_EXT = c

APP_SOURCES = $(shell find $(SRC_DIR)/$(APP_NAME) -name '*.$(SRC_EXT)')
APP_OBJECTS = $(APP_SOURCES:$(SRC_DIR)/%.$(SRC_EXT)=$(OBJ_DIR)/$(SRC_DIR)/%.o)

LIB_SOURCES = $(shell find $(SRC_DIR)/$(LIB_NAME) -name '*.$(SRC_EXT)')
LIB_OBJECTS = $(LIB_SOURCES:$(SRC_DIR)/%.$(SRC_EXT)=$(OBJ_DIR)/$(SRC_DIR)/%.o)

LIB_TERM_SOURCES = $(shell find $(SRC_DIR)/$(LIB_TERM_NAME) -name '*.$(SRC_EXT)')
LIB_TERM_OBJECTS = $(LIB_TERM_SOURCES:$(SRC_DIR)/%.$(SRC_EXT)=$(OBJ_DIR)/$(SRC_DIR)/%.o)

DEPS=$(APP_OBJECTS:.o=.d)$(LIB_OBJECTS:.o=.d)$(LIB_TERM_OBJECTS:.o=.d)

.PHONY: all
all: $(APP_PATH) 

-include $(DEPS)

$(APP_PATH): $(APP_OBJECTS) $(LIB_PATH) $(LIB_TERM_PATH) 
	$(CC) $(CFLAGS) $(CPPFLAGS) $^ -o $@

$(LIB_PATH): $(LIB_OBJECTS)
	ar rcs $@ $^

$(LIB_TERM_PATH): $(LIB_TERM_OBJECTS)
	ar rcs $@ $^

$(OBJ_DIR)/%.o: %.c
	$(CC) -c $(CFLAGS) $(CPPFLAGS) $< -o $@

run: $(APP_PATH)
	$(APP_PATH)

.PHONY: clean
clean:
	$(RM) $(APP_PATH) $(LIB_PATH) $(LIB_TERM_PATH)
	find $(OBJ_DIR) -name '*.o' -exec $(RM) '{}' \;
	find $(OBJ_DIR) -name '*.d' -exec $(RM) '{}' \;
