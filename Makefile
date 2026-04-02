#vars
TARGET_EXEC := executable
BUILD_DIR := build
SRC_DIR := src
TEST_DIR := tests

SRCS := $(shell find $(SRC_DIR) -name '*.cpp')
OBJS := $(SRCS:$(SRC_DIR)/%.cpp=$(BUILD_DIR)/%.o)
DEPS := $(OBJS:.o=.d)
INCS := $(shell find $(SRC_DIR) -type d)

LEXER_TEST := $(BUILD_DIR)/$(TEST_DIR)/lexer_test
PARSER_TEST := $(BUILD_DIR)/$(TEST_DIR)/parser_test
EVALUATOR_TEST := $(BUILD_DIR)/$(TEST_DIR)/evaluator_test
TEST_DEPS := $(LEXER_TEST).d $(PARSER_TEST).d

INC_FLAGS := $(addprefix -I,$(INCS))
CPPFLAGS := $(INC_FLAGS) -MMD -MP
LDFLAGS :=

CXX := g++

#all
all: $(BUILD_DIR)/$(TARGET_EXEC)

#executable dependencies
$(BUILD_DIR)/$(TARGET_EXEC): $(OBJS)
	@echo "Linking"
	mkdir -p $(BUILD_DIR)
	$(CXX) $(OBJS) -o $@ $(LDFLAGS)

#object dependencies
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp
	@echo "Building dependencies"
	mkdir -p $(dir $@)
	$(CXX) $(CPPFLAGS) -c $< -o $@
	@echo

#for change in header files
-include $(DEPS)
-include $(TEST_DEPS)

.PHONY: all clean run

run: all
	./$(BUILD_DIR)/$(TARGET_EXEC)

clean:
	rm -rf $(BUILD_DIR)
format:
	find $(SRC_DIR)/ -name '*.cpp' -o -name '*.h' | xargs clang-format -i

check-format:
	find $(SRC_DIR)/ -name '*.cpp' -o -name '*.h' | xargs clang-format --dry-run --Werror


# separate tests for lexer, parser, evaluator
test-lexer: $(LEXER_TEST)
	./$(LEXER_TEST)

test-parser: $(PARSER_TEST)
	./$(PARSER_TEST)

test-evaluator: $(EVALUATOR_TEST)
	./$(EVALUATOR_TEST)

# macro for executable and object dependencies for tests
define TEST_RULE
$(1): $(OBJS) $(1).o
	mkdir -p $(BUILD_DIR)/$(TEST_DIR)
	$(CXX) $(CPPFLAGS) $$^ -o $$@

$(1).o: $(TEST_DIR)/$(notdir $(1)).cpp
	mkdir -p $(BUILD_DIR)/$(TEST_DIR)
	$(CXX) $(CPPFLAGS) -c $$< -o $$@
endef

$(eval $(call TEST_RULE,$(LEXER_TEST)))
$(eval $(call TEST_RULE,$(PARSER_TEST)))
$(eval $(call TEST_RULE,$(EVALUATOR_TEST)))