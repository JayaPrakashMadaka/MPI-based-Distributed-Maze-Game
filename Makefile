CC = mpic++
CFLAGS = -std=c++11
SRC_DIR_GEN = src/generator
SRC_DIR_SOLVER = src/solver
BUILD_DIR_GEN = src/generator
BUILD_DIR_SOLVER = src/solver

SRCS_GEN = $(wildcard $(SRC_DIR_GEN)/*.cpp)
OBJS_GEN = $(patsubst $(SRC_DIR_GEN)/%.cpp,$(BUILD_DIR_GEN)/%.o,$(SRCS_GEN))

SRCS_SOLVER = $(wildcard $(SRC_DIR_SOLVER)/*.cpp)
OBJS_SOLVER = $(patsubst $(SRC_DIR_SOLVER)/%.cpp,$(BUILD_DIR_SOLVER)/%.o,$(SRCS_SOLVER))

TARGET = maze.out

.PHONY: all clean

all: $(TARGET)

$(TARGET): $(OBJS_GEN) $(OBJS_SOLVER) $(BUILD_DIR_GEN)/maze.o
	$(CC) $(CFLAGS) $^ -o $@

$(BUILD_DIR_GEN)/%.o: $(SRC_DIR_GEN)/%.cpp
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR_SOLVER)/%.o: $(SRC_DIR_SOLVER)/%.cpp
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR_GEN)/maze.o: src/maze.cpp
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(BUILD_DIR_GEN)/*.o $(BUILD_DIR_SOLVER)/*.o $(TARGET) src/MAZE_BFS.txt src/MAZE_KRUSKAL.txt