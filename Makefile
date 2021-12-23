CXX = g++ # Compiler

# List all needed directories
SRC_DIR := src
OBJ_DIR := obj
BIN_DIR := .

# Define our output target
TARGET := $(BIN_DIR)/parser

# Gather a list of the source and object files
SRCS := $(wildcard $(SRC_DIR)/*.cpp)
OBJS := $(patsubst $(SRC_DIR)/%.cpp, $(OBJ_DIR)/%.o, $(SRCS))

# Get any flags we'll need
# NOTE: CPP stands for C PreProcessor, not c++ (c++ == CXX)
CPPFLAGS := -Iinclude -MMD -MP
CXXFlags := -Wall -std=c++17 -g

# List things that aren't targets to be created
.PHONY : all clean

# Make sure we have the TARGET before we continue
all: $(TARGET)

# Build the executable from the objects
$(TARGET) : $(OBJS)
		$(CXX) $^ -o $@

# Create the objects from the sources
$(OBJ_DIR)/%.o : $(SRC_DIR)/%.cpp | $(OBJ_DIR)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $< -o $@

# Make sure our directories are created
$(OBJ_DIR) :
	mkdir -p $@

# Cleanup
clean :
	@$(RM) -rv $(TARGET) $(OBJ_DIR)

# Include our dependencies
-include $(OBJ:.o=.d)
