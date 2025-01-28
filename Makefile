# Compiler
CXX = g++
# Compiler flags
CXXFLAGS = -Iinclude -O2 -Wall -Wextra -std=c++17

# Directories
SRC_DIR = src
INCLUDE_DIR = include
TEST_DIR = test
BUILD_DIR = build

# Source files
SRCS = $(SRC_DIR)/IdealGas.cpp $(SRC_DIR)/PengRobinson.cpp $(SRC_DIR)/RootFinding.cpp $(SRC_DIR)/GasProperties.cpp $(SRC_DIR)/InteractionParameters.cpp

# Test files
TEST_SRCS = $(TEST_DIR)/PR.cpp $(TEST_DIR)/Root.cpp

# Object files for source files
OBJS = $(SRCS:$(SRC_DIR)/%.cpp=$(BUILD_DIR)/%.o)

# Object files for test files
TEST_OBJS = $(TEST_SRCS:$(TEST_DIR)/%.cpp=$(BUILD_DIR)/%.o)

# Executables for each test
PR_EXEC = PR_Test.exe
ROOT_EXEC = Root_Test.exe

# Default rule to build all executables
all: $(PR_EXEC) $(ROOT_EXEC)

# Rule to build PR_Test executable
$(PR_EXEC): $(OBJS) $(BUILD_DIR)/PR.o
	$(CXX) $(CXXFLAGS) -o $(PR_EXEC) $(OBJS) $(BUILD_DIR)/PR.o

# Rule to build Root_Test executable
$(ROOT_EXEC): $(OBJS) $(BUILD_DIR)/Root.o
	$(CXX) $(CXXFLAGS) -o $(ROOT_EXEC) $(OBJS) $(BUILD_DIR)/Root.o

# Rule to compile source files into object files
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp
	if not exist $(BUILD_DIR) mkdir $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Rule to compile test files into object files
$(BUILD_DIR)/%.o: $(TEST_DIR)/%.cpp
	if not exist $(BUILD_DIR) mkdir $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean build files
clean:
	del /Q $(BUILD_DIR)\*.o $(PR_EXEC) $(ROOT_EXEC)
	rmdir /S /Q $(BUILD_DIR)

# Run the PR_Test executable
run-pr: $(PR_EXEC)
	./$(PR_EXEC)

# Run the Root_Test executable
run-root: $(ROOT_EXEC)
	./$(ROOT_EXEC)

# Run both tests
run: run-pr run-root
