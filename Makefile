CC = gcc
PP = g++ 
CFLAGS = -Wall -Wextra
CPPFLAGS = -I src -I thirdparty
LDFLAGS =
LDLIBS =

APP_NAME = SimpleComputer
SAT_NAME = SAT
SBT_NAME = SBT
LIB_COMPUTER_NAME = libcomputer
LIB_TERM_NAME = libterm
LIB_BIGCHAR_NAME = libbigchar
LIB_READKEY_NAME = libreadkey
LIB_COMMON_NAME = libcommon
SRC_DIR = src

APP_PATH = $(APP_NAME)
SAT_PATH = $(SAT_NAME)
SBT_PATH = $(SBT_NAME)
LIB_COMPUTER_PATH = $(SRC_DIR)/$(LIB_COMPUTER_NAME)/$(LIB_COMPUTER_NAME).a
LIB_TERM_PATH = $(SRC_DIR)/$(LIB_TERM_NAME)/$(LIB_TERM_NAME).a
LIB_BIGCHAR_PATH = $(SRC_DIR)/$(LIB_BIGCHAR_NAME)/$(LIB_BIGCHAR_NAME).a
LIB_READKEY_PATH = $(SRC_DIR)/$(LIB_READKEY_NAME)/$(LIB_READKEY_NAME).a
LIB_COMMON_PATH = $(SRC_DIR)/$(LIB_COMMON_NAME)/$(LIB_COMMON_NAME).a

SRC_EXT = c
SRC_EXT_CPP = cpp

APP_SOURCES = $(shell find $(SRC_DIR)/$(APP_NAME) -name '*.$(SRC_EXT)')
APP_OBJECTS = $(APP_SOURCES:$(SRC_DIR)/%.$(SRC_EXT)=$(SRC_DIR)/%.o)

SAT_SOURCES = $(shell find $(SRC_DIR)/$(SAT_NAME) -name '*.$(SRC_EXT)')
SAT_OBJECTS = $(SAT_SOURCES:$(SRC_DIR)/%.$(SRC_EXT)=$(SRC_DIR)/%.o)

SBT_SOURCES = $(shell find $(SRC_DIR)/$(SBT_NAME) -name '*.$(SRC_EXT_CPP)')
SBT_OBJECTS = $(SBT_SOURCES:$(SRC_DIR)/%.$(SRC_EXT_CPP)=$(SRC_DIR)/%.o)

LIB_COMPUTER_SOURCES = $(shell find $(SRC_DIR)/$(LIB_COMPUTER_NAME) -name '*.$(SRC_EXT)')
LIB_COMPUTER_OBJECTS = $(LIB_COMPUTER_SOURCES:$(SRC_DIR)/%.$(SRC_EXT)=$(SRC_DIR)/%.o)

LIB_TERM_SOURCES = $(shell find $(SRC_DIR)/$(LIB_TERM_NAME) -name '*.$(SRC_EXT)')
LIB_TERM_OBJECTS = $(LIB_TERM_SOURCES:$(SRC_DIR)/%.$(SRC_EXT)=$(SRC_DIR)/%.o)

LIB_BIGCHAR_SOURCES = $(shell find $(SRC_DIR)/$(LIB_BIGCHAR_NAME) -name '*.$(SRC_EXT)')
LIB_BIGCHAR_OBJECTS = $(LIB_BIGCHAR_SOURCES:$(SRC_DIR)/%.$(SRC_EXT)=$(SRC_DIR)/%.o)

LIB_READKEY_SOURCES = $(shell find $(SRC_DIR)/$(LIB_READKEY_NAME) -name '*.$(SRC_EXT)')
LIB_READKEY_OBJECTS = $(LIB_READKEY_SOURCES:$(SRC_DIR)/%.$(SRC_EXT)=$(SRC_DIR)/%.o)

LIB_COMMON_SOURCES = $(shell find $(SRC_DIR)/$(LIB_COMMON_NAME) -name '*.$(SRC_EXT)')
LIB_COMMON_OBJECTS = $(LIB_COMMON_SOURCES:$(SRC_DIR)/%.$(SRC_EXT)=$(SRC_DIR)/%.o)

DEPS=$(APP_OBJECTS:.o=.d)$(LIB_COMPUTER_SOURCES:.o=.d)$(LIB_TERM_OBJECTS:.o=.d)$(LIB_READKEY_OBJECTS:.o=.d)

.PHONY: all
all: $(APP_PATH) $(SAT_PATH) $(SBT_PATH)

-include $(DEPS)

$(APP_PATH): $(APP_OBJECTS) $(LIB_COMPUTER_PATH) $(LIB_TERM_PATH) $(LIB_BIGCHAR_PATH) $(LIB_READKEY_PATH) $(LIB_COMMON_PATH)
	$(CC) $(CFLAGS) $(CPPFLAGS) $^ -o $@

$(SAT_PATH): $(SAT_OBJECTS) $(LIB_COMPUTER_OBJECTS) $(LIB_COMMON_OBJECTS) $(LIB_TERM_OBJECTS)
	$(CC) $(CFLAGS) $(CPPFLAGS) $^ -o $@

$(SBT_PATH): $(SBT_OBJECTS) $(LIB_COMPUTER_OBJECTS) $(LIB_COMMON_OBJECTS) $(LIB_TERM_OBJECTS)
	$(PP) $(CFLAGS) $(CPPFLAGS) $^ -o $@

$(LIB_COMPUTER_PATH): $(LIB_COMPUTER_OBJECTS)
	ar rcs $@ $^

$(LIB_TERM_PATH): $(LIB_TERM_OBJECTS)
	ar rcs $@ $^

$(LIB_BIGCHAR_PATH): $(LIB_BIGCHAR_OBJECTS)
	ar rcs $@ $^

$(LIB_READKEY_PATH): $(LIB_READKEY_OBJECTS)
	ar rcs $@ $^

$(LIB_COMMON_PATH): $(LIB_COMMON_OBJECTS)
	ar rcs $@ $^

$(OBJ_DIR)/%.o: %.c
	$(CC) -c $(CFLAGS) $(CPPFLAGS) $< -o $@

run: ./$(APP_PATH)
	./$(APP_PATH)

.PHONY: clean
clean:
	$(RM) $(APP_PATH) $(SBT_PATH) $(SAT_PATH) $(LIB_COMPUTER_PATH) $(LIB_TERM_PATH) $(LIB_BIGCHAR_PATH) $(LIB_READKEY_PATH) $(LIB_COMMON_PATH)
	find $(SRC_DIR) -name '*.o' -exec $(RM) '{}' \;
	find $(SRC_DIR) -name '*.d' -exec $(RM) '{}' \;
	find $(TEST_NAME) -name '*.o' -exec $(RM) '{}' \;
