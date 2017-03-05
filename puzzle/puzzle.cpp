#include "puzzle.h"

puzzle::puzzle()
{
    // Start curses
    initscr();                      // Initialize screen
    curs_set(false);                // No blinking cursor
    noecho();
    getmaxyx(stdscr, wRow, wCol);

    // Puzzle settings
    dim = 3;
    instructionIndex = 0;
    animateSolution = false;
    state = PRE_GAME;

    // Pointers to menu functions
    menu[PRE_GAME] = &puzzle::preMenu;
    menu[IN_GAME] = &puzzle::inMenu;
    menu[POST_GAME] = &puzzle::winMenu;

    // Pointers to can move functions
    canMove[0] = &puzzle::canMoveUp;
    canMove[1] = &puzzle::canMoveDown;
    canMove[2] = &puzzle::canMoveLeft;
    canMove[3] = &puzzle::canMoveRight;

    // Pointers to move functions
    moveGrid[0] = &puzzle::moveUp;
    moveGrid[1] = &puzzle::moveDown;
    moveGrid[2] = &puzzle::moveLeft;
    moveGrid[3] = &puzzle::moveRight;
}

puzzle::~puzzle()
{
    clearGrid();
    instructions.clear();
    dim = instructionIndex = 0;
    animateSolution = 0;
}

void puzzle::run()
{
    while(state != QUIT_GAME)
    {
        (this->*menu[state])();
        refresh();
        clear();
    }
}

void puzzle::preMenu()
{
    // Print title
    mvprintw(wRow/9, wCol/2 - 7, "%s", "Sliding Puzzle");

    // Print dimension input
    mvprintw(wRow/5, wCol/2 - 7, "%s", "Dimension: ");
    mvprintw(wRow/5, wCol/2 + 4, "%i", dim);
    mvprintw(wRow/5, wCol/2 + 5, "%s", "x");
    mvprintw(wRow/5, wCol/2 + 6, "%i", dim);

    // Print instructions
    mvprintw(wRow/2, wCol/2 - 23, "%s", "Press left/right arrow keys to modify dimension");
    mvprintw(wRow/2+2, wCol/2 - 10, "%s", "Press enter to begin");

    // Get input
    int input = getch();

    // Process input
    switch(input)
    {
        // Right arrow
        case 67:    if(dim == 3)
                        dim++;
                    break;

        // Left arrow
        case 68:    if(dim == 4)
                        dim--;
                    break;

        // Enter
        case 10:    currState.setDim(dim);
                    generateRand();
                    instructions.push_back(currState);
                    state = IN_GAME;
                    break;

        default:    break;
    }
}

void puzzle::inMenu()
{
    // Print title
    mvprintw(wRow/9, wCol/2 - 7, "%s", "Sliding Puzzle");

    // Print board
    print(currState);

    // Print instructions
    mvprintw(wRow/2 + 4, wCol/2 - 12, "%s", "Press arrow keys to move");
    mvprintw(wRow/2 + 6, wCol/2 - 10, "%s", "Press enter to solve");

    // Get input
    int input = getch();

    // Process input
    switch(input)
    {
        // Up arrow
        case 65:    if(canMoveDown(currState))
                    {
                        moveDown(currState);
                        instructions.push_back(currState);
                    }
                    break;

        // Down arrow
        case 66:    if(canMoveUp(currState))
                    {
                        moveUp(currState);
                        instructions.push_back(currState);
                    }
                    break;

        // Right arrow
        case 67:    if(canMoveLeft(currState))
                    {
                        moveLeft(currState);
                        instructions.push_back(currState);
                    }
                    break;

        // Left arrow
        case 68:    if(canMoveRight(currState))
                    {
                        moveRight(currState);
                        instructions.push_back(currState);
                    }
                    break;

        // Enter
        case 10:    mvprintw(wRow/2 + 8, wCol/2 - 5, "%s", "Solving...");
                    refresh();
                    solve();
                    animateSolution = true;
                    instructionIndex = instructions.size()-1;
                    state = POST_GAME;
                    break;
    }

    // Check for win
    if(isGoal(currState))
    {
        state = POST_GAME;
        instructionIndex = instructions.size()-1;
        refresh();
        erase();
    }
}

void puzzle::winMenu()
{
    // Print title
    mvprintw(wRow/9, wCol/2 - 7, "%s", "Sliding Puzzle");

    // Animate solution in the instruction vector
    if(animateSolution)
    {
        // Animate solution
        for(int i = 0; i < instructions.size(); i++)
        {
            // Print grid
            print(instructions[i]);
            refresh();
            usleep(100000);

            // Print index
            mvprintw(wRow/2+2, wCol/2 - 5, "%i", i+1);
            mvprintw(wRow/2+2, wCol/2 - 1, "%s", "/");
            mvprintw(wRow/2+2, wCol/2 + 2, "%i", instructions.size());

            // Turn off animation when finished
            animateSolution = instructions.size() - i - 1;
        }
    }

    else
    {
        // Print grid
        print(instructions[instructionIndex]);

        // Print index
        mvprintw(wRow/2+2, wCol/2 - 5, "%i", instructionIndex+1);
        mvprintw(wRow/2+2, wCol/2 - 1, "%s", "/");
        mvprintw(wRow/2+2, wCol/2 + 2, "%i", instructions.size());

        // Print instructions
        mvprintw(wRow/2+4, wCol/2 - 24, "%s", "Press left/right arrow keys to navigate solution");
        mvprintw(wRow/2+6, wCol/2 - 10, "%s", "Press enter to restart");

        // Get input
        int input = getch();

        // Process input
        switch(input)
        {
            // Right arrow
            case 67:    if(instructionIndex < instructions.size()-1)
                            instructionIndex++;
                        break;

            // Left arrow
            case 68:    if(instructionIndex > 0)
                            instructionIndex--;
                        break;

            // Enter
            case 10:    clearGrid();
                        currState.setDim(dim);
                        generateRand();
                        instructions.push_back(currState);
                        state = IN_GAME;
                        clear();
                        break;
        }

    }
}

int puzzle::posOf(int num, const grid &g)
{
    for(int i = 0; i < g.dim * g.dim; i++)
        if(g.arr[i] == num)
            return i;
    return -1;
}

bool puzzle::isGoal(const grid &g)
{
    for(int i = 0; i < dim*dim - 1; i++)
        if(g.arr[i] != i+1)
            return false;
    return !g.arr[dim*dim - 1];
}

bool puzzle::canMoveUp(const grid &g)
{
    return posOf(0, g) >= dim;
}

bool puzzle::canMoveDown(const grid &g)
{
    return posOf(0, g) < (dim*dim - dim);
}

bool puzzle::canMoveLeft(const grid &g)
{
    return posOf(0, g) % dim;
}

bool puzzle::canMoveRight(const grid &g)
{
    return (posOf(0, g) % dim) != (dim - 1);
}

void puzzle::moveUp(grid &g)
{
    swap(posOf(0, g) - g.dim, posOf(0, g), g);
}

void puzzle::moveDown(grid &g)
{
    swap(posOf(0, g) + g.dim, posOf(0, g), g);
}

void puzzle::moveLeft(grid &g)
{
    swap(posOf(0, g) - 1, posOf(0, g), g);
}

void puzzle::moveRight(grid &g)
{
    swap(posOf(0, g) + 1, posOf(0, g), g);
}

void puzzle::swap(int i, int j, grid &g)
{
    int temp = g[i];
    g[i] = g[j];
    g[j] = temp;
}

void puzzle::generateRand()
{
    // Set seed
    srand(time(NULL));

    // Fill array
    for(int i = 0; i < dim*dim; i++)
        currState[i] = i;

    // Shuffle array
    for(int i = 0; i < dim*dim; i++)
    {
        int j = rand() % dim*dim;
        swap(i, j, currState);
    }

    // Inversion count of the generated puzzle
    int inversions = inversionCount();

    // Odd dimension:  must have even inversions
    // Even dimension: Pos of 0 is on an even row -> must have odd inversions
    //                 Pos of 0 is on an odd row -> must have even inversions
    if((dim%2 && inversions%2) ||
       (!dim%2 && (!inversions%2 && !(posOf(0, currState)/4)%2) || (inversions%2 && (posOf(0, currState)/4)%2)))
        decrementInversionCount();
}

int puzzle::inversionCount()
{
    int count = 0;
    for(int i = 0; i < dim*dim - 1; i++)
        for(int j = i+1; j < dim*dim; j++)
             if(currState[j] && currState[i] && currState[i] > currState[j])
                  count++;
    return count;
}

void puzzle::decrementInversionCount()
{
    for(int i = 0; i < dim*dim - 1; i++)
        for(int j = i+1; j < dim*dim; j++)
             if(currState[j] && currState[i] && currState[i] > currState[j])
             {
                 swap(i, j, currState);
                 return;
             }
}

void puzzle::solve()
{
    // Clear instruction vector
    instructions.clear();

    // Initial threshhold and f-score
    int threshhold = hammingDistance(currState) + manhattanDistance(currState) + 2*linearConflict(currState);
    int fScore = 1;

    // Perform depth-first searches indefinitely until the goal is found
    while(fScore >= 0)
    {
        fScore = search(currState, 0, threshhold);
        threshhold = fScore;
    }
}

int puzzle::search(grid &g, int gScore, int threshhold)
{
    // Obtain f-score of current node
    int fScore = gScore + hammingDistance(g) + manhattanDistance(g) + 2*linearConflict(g);

    // Threshhold reached
    if(fScore > threshhold)
        return fScore;

    // Goal reached
    if(isGoal(g))
    {
        // Fill instruction stack
        grid* gridPtr = &g;
        while(gridPtr)
        {
            instructions.insert(instructions.begin(), *gridPtr);
            gridPtr = gridPtr->parent;
        }
        return -1;
    }

    // Perform searches on the current grid's children and find the lowest f-score
    int minFScore = INT32_MAX;
    for(int i = 0; i < 4; i++)
    {
        // Possible child
        if((this->*canMove[i])(g))
        {
            // Create child
            if(g.child[i] == nullptr)
            {
                g.child[i] = new grid(g);
                (this->*moveGrid[i])(*g.child[i]);
                g.child[i]->parent = &g;
            }

            // Is a unique child
            if(!g.parent || (g.parent && *g.child[i] != *g.parent))
            {
                int childFScore = search(*g.child[i], gScore+1, threshhold);

                // Goal reached
                if(childFScore < 0)
                    return -1;

                // Find lowest f-score of the children
                if(childFScore < minFScore)
                    minFScore = childFScore;
            }
        }
    }
    return minFScore;
}

int puzzle::hammingDistance(const grid &g)
{
    int hamming = 0;
    for(int i = 0; i < dim*dim - 1; i++)
        if(g.arr[i] != i+1)
            hamming++;
    return hamming;
}

int puzzle::manhattanDistance(const grid& g)
{
    int distance = 0;
    for(int i = 1; i < g.dim*g.dim; i++)
    {
        int xDistance = abs((posOf(i, g))/g.dim - (i-1)/g.dim);
        int yDistance = abs((posOf(i, g))%g.dim - (i-1)%g.dim);
        distance += xDistance + yDistance;
    }
    return distance;
}

int puzzle::linearConflict(const grid &g)
{
    int conflicts = 0;
    for(int firstOfRow = 0; firstOfRow < dim; firstOfRow++)
    {
        for(int i = firstOfRow * dim; i < (firstOfRow*dim) + dim - 1; i++)
        {
            for(int j = i+1; j < firstOfRow*dim + dim; j++)
            {
                // Non-last rows
                if(firstOfRow < dim-1 && currState[j] && currState[i])
                {
                    // Same line, same goals in the line, and the goal of j is left of goal of i
                    if((currState[i] > firstOfRow*dim && currState[i] <= firstOfRow*dim + dim) &&
                       (currState[j] > firstOfRow*dim && currState[j] <= firstOfRow*dim + dim) &&
                       (currState[i]/(dim+1) == currState[j]/(dim+1)) &&
                       (currState[i]/(dim+1) == currState[j]/(dim+1)) &&
                       (currState[j] < currState[i]))
                    {
                        conflicts++;
                    }
                }
                else if(firstOfRow == dim-1)
                {
                    // Same line, goal is in the same line
                    if(((currState[i] > firstOfRow*dim || !currState[i]) && (currState[j] > firstOfRow*dim || !currState[j])) &&
                        (currState[j] && ((currState[j] < currState[i]) || !currState[i])))
                    {
                        conflicts++;
                    }
                }
            }
        }
    }
    return conflicts;
}

void puzzle::print(const grid &g)
{
    for(int i = 0, j = dim-3; i < dim*dim; i++)
    {
        if(i%dim == 0 && i > dim-1)
            j += 2;
        if(g.arr[i])
            mvprintw(wRow/3 - dim + j, wCol/2 - (dim+dim-1) + (i%dim)*4, "%i", g.arr[i]);
        else
            mvprintw(wRow/3 - dim + j, wCol/2 - (dim+dim-1) + (i%dim)*4, "%s", " ");
    }
}

void puzzle::clearGrid()
{
    for(int i = 0; i < 4; i++)
        clearGrid(currState.child[i]);
    instructions.clear();
    instructionIndex = 0;
    animateSolution = false;
}

void puzzle::clearGrid(grid* &root)
{
    if(root)
    {
        for(int i = 0; i < 4; i++)
        {
            clearGrid(root->child[i]);
            if(root->child[i])
                delete root->child[i];
            root->child[i] = nullptr;
        }
        delete root;
        root = nullptr;
    }

}
