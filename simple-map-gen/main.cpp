#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>

using namespace std;

// Structure to define a room's position and size
struct Room {
    size_t xStart, yStart, width, height;
};

// Constants
const size_t hallWidth = 1;             // Hallway width
const char wallChar = 'W';              // Wall character
const char floorChar = 'F';             // Floor character
const int MAX_DEPTH = 3;                // Maximum depth for recursive room generation

// Function declarations
void placeRoom(vector<vector<char>>& grid, const Room& room);
void placeVerticalHall(vector<vector<char>>& grid, size_t x, size_t yStart, size_t yEnd);
void placeHorizontalHall(vector<vector<char>>& grid, size_t y, size_t xStart, size_t xEnd);
void generateRoom(vector<vector<char>>& grid, const Room& room, int depth);
void rpgSimpleGenerator(size_t row, size_t col);

int main() {
    srand(static_cast<unsigned>(time(nullptr))); // Seed RNG
    rpgSimpleGenerator(50, 50);                  // Generate dungeon
    return 0;
}

// Entry point for dungeon generation
void rpgSimpleGenerator(size_t row, size_t col) {
    // Ensure odd dimensions for better centering and symmetry
    if (row % 2 == 0) row += 1;
    if (col % 2 == 0) col += 1;

    // Create grid filled with walls
    vector<vector<char>> grid(row, vector<char>(col, wallChar));

    // Define central room in the middle of the grid
    Room centerRoom = {(col - 9) / 2, (row - 9) / 2, 9, 9};

    // Start recursive room generation
    generateRoom(grid, centerRoom, 0);

    // Print resulting grid to console
    for (const auto& rowVec : grid) {
        for (char tile : rowVec) {
            cout << tile;
        }
        cout << '\n';
    }
}

// Recursively generate rooms with possible side rooms
void generateRoom(vector<vector<char>>& grid, const Room& room, int depth) {
    if (depth > MAX_DEPTH) return; // Stop recursion when max depth is reached

    placeRoom(grid, room); // Draw the current room

    size_t midX = room.xStart + room.width / 2; // Horizontal center of the room
    size_t midY = room.yStart + room.height / 2; // Vertical center of the room

    // Randomly try to place a room above
    if (rand() % 2 == 1) {
        Room top = {room.xStart, room.yStart - 3 - 7, 9, 7};
        if (top.yStart + top.height < room.yStart) {
            placeVerticalHall(grid, midX, top.yStart + top.height, room.yStart - 1);
            generateRoom(grid, top, depth + 1);
        }
    }

    // Randomly try to place a room below
    if (rand() % 2 == 1) {
        Room bottom = {room.xStart, room.yStart + room.height + 3, 9, 7};
        if (bottom.yStart < grid.size() - bottom.height) {
            placeVerticalHall(grid, midX, room.yStart + room.height, bottom.yStart);
            generateRoom(grid, bottom, depth + 1);
        }
    }

    // Randomly try to place a room to the left
    if (rand() % 2 == 1) {
        Room left = {room.xStart - 3 - 9, room.yStart, 9, 7};
        if (left.xStart + left.width < room.xStart) {
            placeHorizontalHall(grid, midY, left.xStart + left.width, room.xStart - 1);
            generateRoom(grid, left, depth + 1);
        }
    }

    // Randomly try to place a room to the right
    if (rand() % 2 == 1) {
        Room right = {room.xStart + room.width + 3, room.yStart, 9, 7};
        if (right.xStart < grid[0].size() - right.width) {
            placeHorizontalHall(grid, midY, room.xStart + room.width, right.xStart);
            generateRoom(grid, right, depth + 1);
        }
    }
}

// Fills a rectangular area in the grid with floor characters
void placeRoom(vector<vector<char>>& grid, const Room& room) {
    for (size_t y = room.yStart; y < room.yStart + room.height; ++y) {
        for (size_t x = room.xStart; x < room.xStart + room.width; ++x) {
            if (y < grid.size() && x < grid[0].size()) {
                grid[y][x] = floorChar;
            }
        }
    }
}

// Creates a vertical hallway between two Y coordinates at column X
void placeVerticalHall(vector<vector<char>>& grid, size_t x, size_t yStart, size_t yEnd) {
    if (yStart > yEnd) swap(yStart, yEnd); // Ensure top-to-bottom direction
    for (size_t y = yStart; y <= yEnd; ++y) {
        for (size_t w = 0; w < hallWidth; ++w) {
            if (y < grid.size() && x + w < grid[0].size())
                grid[y][x + w] = floorChar;
        }
    }
}

// Creates a horizontal hallway between two X coordinates at row Y
void placeHorizontalHall(vector<vector<char>>& grid, size_t y, size_t xStart, size_t xEnd) {
    if (xStart > xEnd) swap(xStart, xEnd); // Ensure left-to-right direction
    for (size_t x = xStart; x <= xEnd; ++x) {
        for (size_t w = 0; w < hallWidth; ++w) {
            if (y + w < grid.size() && x < grid[0].size())
                grid[y + w][x] = floorChar;
        }
    }
}
