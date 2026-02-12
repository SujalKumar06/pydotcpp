#vars
TARGET_EXEC := executable
BUILD_DIR := build
SRC_DIR := src

SRCS := $(shell find $(SRC_DIR) -name '*.cpp')
OBJS := $(SRCS:$(SRC_DIR)/%.cpp=$(BUILD_DIR)/%.o)
DEPS := $(OBJS:.o=.d)
INCS := $(shell find $(SRC_DIR) -type d)

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

.PHONY: all clean run

run: all
	./$(BUILD_DIR)/$(TARGET_EXEC)

clean:
	rm -rf $(BUILD_DIR)
