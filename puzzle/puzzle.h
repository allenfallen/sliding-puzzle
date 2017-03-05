#ifndef PUZZLE_H
#define PUZZLE_H

// Curses library
#include <ncurses.h>
#include <unistd.h>

// Containers
#include <vector>
#include <queue>
#include <stack>

// RNG
#include <cstdlib>
#include <ctime>

#include <string>

// Grid
#include "../grid/grid.h"

class puzzle
{
    public:
        puzzle();
        ~puzzle();
        void run();

    private:
        // Puzzle data
        grid currState;                     // Current state of the grid as well as the head of the tree of a solved puzzle
        unsigned int dim;                   // Dimension of the puzzle
        std::vector<grid> instructions;     // Instructions from currState to the goal grid
        int instructionIndex;
        bool animateSolution;

        // Window data
        int wRow, wCol;

        // Game state for menus
        enum gameState { PRE_GAME, IN_GAME, POST_GAME, QUIT_GAME };
        gameState state;

        // Menus
        typedef void (puzzle::*menuPtr)();
        menuPtr menu[3];
        void preMenu();
        void inMenu();
        void winMenu();

        // General grid functions
        int posOf(int num, const grid &g);
        bool isGoal(const grid &g);

        // Able to move
        typedef bool (puzzle::*canMovePtr)(const grid &g);
        canMovePtr canMove[4];
        bool canMoveUp(const grid &g);
        bool canMoveDown(const grid &g);
        bool canMoveLeft(const grid &g);
        bool canMoveRight(const grid &g);

        // Move
        typedef void (puzzle::*movePtr)(grid &g);
        movePtr moveGrid[4];
        void moveUp(grid &g);
        void moveDown(grid &g);
        void moveLeft(grid &g);
        void moveRight(grid &g);
        void swap(int i, int z, grid &g);

        // Random grid generation
        void generateRand();
        int inversionCount();
        void decrementInversionCount();

        // Solve the puzzle
        void solve();
        int search(grid &g, int gScore, int threshhold);

        // Heuristics
        int hammingDistance(const grid &g);
        int manhattanDistance(const grid &g);
        int linearConflict(const grid &g);

        // Print
        void print(const grid &g);

        // Clear the tree from currState
        void clearGrid();
        void clearGrid(grid* &root);
};

#endif // PUZZLE_H
