# Compiler and flags
CXX = g++
CXXFLAGS = -std=c++17 -Wall -Iinclude -Ijson
LDFLAGS = 

# Directories
SRC_DIR = src
OBJ_DIR = obj
INC_DIR = include

# Files
SOURCES = $(wildcard $(SRC_DIR)/*.cpp)
OBJECTS = $(SOURCES:$(SRC_DIR)/%.cpp=$(OBJ_DIR)/%.o)
TARGET = router-basic

# Default target
all: $(TARGET)

# Link the executable
$(TARGET): $(OBJECTS)
	$(CXX) $(OBJECTS) -o $@ $(LDFLAGS)

# Compile source files into object files
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp | $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Create object directory if it doesn't exist
$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

# Clean logic
clean:
	rm -rf $(OBJ_DIR) $(TARGET)

.PHONY: all clean