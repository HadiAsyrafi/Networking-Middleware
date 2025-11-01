# Define the C++ compiler
CXX = g++

# Define compiler flags
CXXFLAGS = -std=c++17 -pthread

# Define build directory
BUILD_DIR = build

# Default target
all:
	@echo "Usage: make <filename_without_extension>"
	@echo "Example: make condition_variable"
	@echo "Example: make class_example"

# Generic rule to compile any .cpp file
%: %.cpp
	$(CXX) $(CXXFLAGS) $< -o $(BUILD_DIR)/$@
	@echo "Compiled $< -> $(BUILD_DIR)/$@"

# Clean target: remove build directory
clean:
	rm -rf $(BUILD_DIR)
	@echo "Cleaned build directory"

# Run target: compile and run
run-%: %.cpp
	$(CXX) $(CXXFLAGS) $< -o $(BUILD_DIR)/$*
	@echo "Running $(BUILD_DIR)/$*..."
	@./$(BUILD_DIR)/$*

.PHONY: all clean
