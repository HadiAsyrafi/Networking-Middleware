# Define the C++ compiler
CXX = g++

# Define compiler flags
CXXFLAGS = -std=c++17 -pthread -O2
ASM_FLAGS = -S -fverbose-asm -masm=intel  # Generate assembly with comments

# Define build directory
BUILD_DIR = build

# Default target
all:
	@echo "Usage: make <filename_without_extension>"
	@echo "Example: make condition_variable"
	@echo "Example: make asm-condition_variable (for assembly)"
	@echo "Example: make static-condition_variable (for static binary)"
	@echo "Example: make analyze-condition_variable (for analysis)"

# Generic rule to compile any .cpp file
%: %.cpp
	mkdir -p $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) $< -o $(BUILD_DIR)/$@
	@echo "Compiled $< -> $(BUILD_DIR)/$@"

# Assembly generation rule
asm-%: %.cpp
	mkdir -p $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) $(ASM_FLAGS) $< -o $(BUILD_DIR)/$*.s
	@echo "Generated assembly: $(BUILD_DIR)/$*.s"
	@echo "View with: cat $(BUILD_DIR)/$*.s | less"

# Static linking with debug info
static-%: %.cpp
	mkdir -p $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -static -g -O0 $< -o $(BUILD_DIR)/$*_static
	objdump -d -C $(BUILD_DIR)/$*_static > $(BUILD_DIR)/$*_disasm.s
	@echo "Static binary: $(BUILD_DIR)/$*_static"
	@echo "Disassembly: $(BUILD_DIR)/$*_disasm.s"

# Add this to your Makefile
analyze-%: %.cpp
	mkdir -p $(BUILD_DIR)
	# Generate optimized assembly
	$(CXX) $(CXXFLAGS) -S -O2 -fverbose-asm $< -o $(BUILD_DIR)/$*_O2.s
	# Generate unoptimized assembly
	$(CXX) $(CXXFLAGS) -S -O0 -fverbose-asm $< -o $(BUILD_DIR)/$*_O0.s
	# Static link with debug info
	$(CXX) $(CXXFLAGS) -static -g -O0 $< -o $(BUILD_DIR)/$*_static
	# Disassemble static binary
	objdump -d -C $(BUILD_DIR)/$*_static > $(BUILD_DIR)/$*_objdump.s
	@echo "Generated multiple analysis files:"
	@echo "  - $(BUILD_DIR)/$*_O2.s (optimized assembly)"
	@echo "  - $(BUILD_DIR)/$*_O0.s (unoptimized assembly)"  
	@echo "  - $(BUILD_DIR)/$*_objdump.s (full disassembly)"

# Clean target
clean:
	rm -rf $(BUILD_DIR)/*
	@echo "Cleaned build directory"

.PHONY: all clean
