CXX := g++
CXXFLAGS := -std=c++17 -Wall -Wextra -Iinclude
DEBUG_FLAGS := -g -O0
RELEASE_FLAGS := -O3 -DNDEBUG

SRC_DIR := src
BUILD_DIR := build
INCLUDE_DIR := include
TARGET := $(BUILD_DIR)/FinalAssignment

SRCS := $(wildcard $(SRC_DIR)/*.cpp)
OBJS := $(patsubst $(SRC_DIR)/%.cpp,$(BUILD_DIR)/%.o,$(SRCS))
DEPS := $(OBJS:.o=.d)

all: debug

debug: CXXFLAGS += $(DEBUG_FLAGS)
debug: $(TARGET)

release: CXXFLAGS += $(RELEASE_FLAGS)
release: $(TARGET)

$(TARGET): $(OBJS)
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) $^ -o $@

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) -MMD -MP -c $< -o $@

-include $(DEPS)

clean:
	rm -rf $(BUILD_DIR)

run: debug
	./$(TARGET)

format:
	find $(SRC_DIR) $(INCLUDE_DIR) -name '*.h' -o -name '*.cpp' | xargs clang-format -i

.PHONY: all debug release clean run format