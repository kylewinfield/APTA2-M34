#include <iostream>
#include <string>
#include <cctype>
#include <strings.h>
#include <string.h>
#include <vector>
#include <fstream>
#include <sstream>

#include "Game.h"

using std::cin;
using std::cout;
using std::endl;
using std::ifstream;
using std::ofstream;
using std::string;
using std::stringstream;
using std::vector;

Game::Game()
{
    // initialise tileBag
    this->tileBag = new LinkedList;

    // intialise checks to false
    this->gameOver = false;
    this->qwirkleScored = false;
    this->cellOccupied = false;
    this->notAdjacent = false;
    this->tileMismatch = false;
    this->lineTooLong = false;
    this->sameTileInLine = false;
    this->invalidCommand = false;
    this->tileNotInHand = false;
    this->playerTurn = true;

    this->ai = false;
    this->disableColour = false;
};

Game::Game(int argc, char **argv)
{
    // initialise tileBag
    this->tileBag = new LinkedList;

    // intialise checks to false
    this->gameOver = false;
    this->qwirkleScored = false;
    this->cellOccupied = false;
    this->notAdjacent = false;
    this->tileMismatch = false;
    this->lineTooLong = false;
    this->sameTileInLine = false;
    this->invalidCommand = false;
    this->tileNotInHand = false;
    this->playerTurn = true;

    checkArgs(argc, argv);
}

Game::~Game()
{
    delete tileBag;
    delete player1;
    delete player2;
};

void Game::gameLoop()
{
    bool player1MoveAvailable = true;
    bool player2MoveAvailable = true;
    bool validPrompt = false;
    while (this->gameOver == false && !cin.eof())
    {
        if (playerTurn == true)
        {
            // player 1
            clearScreen();
            // forfeit turn if no tiles in player's hand or the tileBag can be placed
            if (!checkForPlayableTiles(this->player1->getHand()) && !checkForPlayableTiles(this->tileBag))
            {
                player1MoveAvailable = false;
            }
            else
            {
                // notify player2 that there are no more moves available to them after skipping their turn
                if (player1MoveAvailable && !player2MoveAvailable)
                {
                    cout << "No moves possible for " << this->player2->getName() << ", forfeiting turn" << endl;
                }
                while (validPrompt == false)
                {
                    try
                    {
                        getPrompt(this->player1);
                        validPrompt = true;
                    }
                    catch (...)
                    {
                        cout << "Invalid input!" << endl;
                    }
                }
                validPrompt = false;
                if (tileBag->getSize() > 0)
                {
                    this->player1->addTile(this->tileBag->getTile(0));
                    this->tileBag->deleteElement(0);
                }
                playerTurn = false;
            }
        }
        else
        {
            if (!cin.eof())
            {
                // player 2
                clearScreen();
                // forfeit turn if no tiles in player's hand or the tileBag can be placed
                if (!checkForPlayableTiles(this->player2->getHand()) && !checkForPlayableTiles(this->tileBag))
                {
                    player2MoveAvailable = false;
                }
                else
                {
                    // notify player1 that there are no more moves available to them after skipping their turn
                    if (player2MoveAvailable && !player1MoveAvailable)
                    {
                        cout << "No moves possible for " << this->player1->getName() << ", forfeiting turn" << endl;
                    }
                    while (validPrompt == false)
                    {
                        if (ai == true)
                        {
                            AITurn();
                            validPrompt = true;
                        }
                        else
                        {

                            try
                            {
                                getPrompt(this->player2);
                                validPrompt = true;
                            }
                            catch (...)
                            {
                                cout << "Invalid input!" << endl;
                            }
                        }
                    }
                    validPrompt = false;
                    if (tileBag->getSize() > 0)
                    {
                        this->player2->addTile(this->tileBag->getTile(0));
                        this->tileBag->deleteElement(0);
                    }
                    if (player2->getHandSize() == 0)
                    {
                        gameOver = true;
                    }
                    playerTurn = true;
                }
            }
        }
    }

    // game end results display
    if (this->gameOver == true && !cin.eof())
    {
        printBoard(board);
        cout << "Final scores" << endl;
        cout << player1->getName() << ": " << player1->getScore() << " points" << endl;
        cout << player2->getName() << ": " << player2->getScore() << " points" << endl;
        if (player1->getScore() > player2->getScore())
        {
            cout << player1->getName() << " wins!" << endl;
            cout << endl;
        }
        else if (player2->getScore() > player1->getScore())
        {
            cout << player2->getName() << " wins!" << endl;
            cout << endl;
        }
        else
        {
            cout << "You're both losers!" << endl;
            cout << endl;
        }
    }
}

void Game::AITurn()
{
    // data needed to perform 'best' move
    int highestScore = 0;
    Tile *bestTile = nullptr;
    char bestRow = '-';
    int bestCol = 0;

    // flag if no available moves
    bool noMove = false;

    // ->Iterate through hand (get each tile).
    // -->Calculate possible scores from that tile on each space in AIList, that is
    // a valid move.
    // ->After iterating through all tiles, select highest scoring move and perform it

    for (int i = 0; i < player2->getHandSize(); i++)
    {
        for (unsigned j = 0; j < AIList.size(); j++)
        {
            if (checkValid(player2->getHand()->getTile(i), AIList[j].getRow(),
                           AIList[j].getCol()) == true)
            {
                placeTile(player2->getHand()->getTile(i), AIList[j].getRow(), AIList[j].getCol());
                if (
                    highestScore < calculateScore(AIList[j].getRow(), AIList[j].getCol()))
                {

                    highestScore = calculateScore(AIList[j].getRow(), AIList[j].getCol());
                    bestTile = player2->getHand()->getTile(i);
                    bestRow = AIList[j].getRow();
                    bestCol = AIList[j].getCol();
                }
                board[AIList[j].getRow()][AIList[j].getCol()] = nullptr;
            }
        }
    }

    if (noMove == false)
    {
        player2->addScore(placeTile(bestTile, bestRow, bestCol));
        addNullAdjacentSpaces(bestRow, bestCol);
    }
    else
    {
        // replace tile code
    }
}

void Game::addNullAdjacentSpaces(char row, int col)
{

    // need to add coordinates of space if not already in list AND value on board is NULL
    Coordinate down(row + 1, col);
    Coordinate up(row - 1, col);
    Coordinate right(row, col + 1);
    Coordinate left(row, col - 1);

    if (AIList.size() == 0)
    {
        AIList.push_back(right);
        AIList.push_back(left);
        AIList.push_back(down);
        AIList.push_back(up);
    }
    else
    {
        bool downflag = false;
        bool upflag = false;
        bool rightflag = false;
        bool leftflag = false;

        for (unsigned i = 0; AIList.size() > i; i++)
        {
            if (down.isSpaceNull(board) == false)
            {
                // cout << "AIList[]: " << AIList[i].getRow() << AIList[i].getCol() << endl;
                // cout << "down: " << down.getRow() << down.getCol() << endl;
                downflag = true;
            }
            else
            {
                if (AIList[i].isEqual(down) == true)
                {
                    downflag = true;
                }
            }
            if (up.isSpaceNull(board) == false)
            {
                // cout << "AIList[]: " << AIList[i].getRow() << AIList[i].getCol() << endl;
                // cout << "up: " <<    up.getRow() << up.getCol() << endl;
                upflag = true;
            }
            else
            {
                if (AIList[i].isEqual(up) == true)
                {
                    upflag = true;
                }
            }
            if (right.isSpaceNull(board) == false)
            {
                // cout << "AIList[]: " << AIList[i].getRow() << AIList[i].getCol() << endl;
                // cout << "right: " << right.getRow() << right.getCol() << endl;
                rightflag = true;
            }
            else
            {
                if (AIList[i].isEqual(right) == true)
                {
                    rightflag = true;
                }
            }
            if (left.isSpaceNull(board) == false)
            {
                // cout << "AIList[]: " << AIList[i].getRow() << AIList[i].getCol() << endl;
                // cout << "left: " << left.getRow() << left.getCol() << endl;
                leftflag = true;
            }
            else
            {
                if (AIList[i].isEqual(left) == true)
                {
                    leftflag = true;
                }
            }
        }
        // cout << "left: " << board[row][col - 1]->colour << board[row][col - 1]->shape << endl;

        if (downflag == false)
        {
            cout << "Down added: " << down.getRow() << down.getCol() << endl;
            AIList.push_back(down);
        }
        if (upflag == false)
        {
            cout << "Up added: " << up.getRow() << up.getCol() << endl;
            AIList.push_back(up);
        }
        if (rightflag == false)
        {
            cout << "Right added: " << right.getRow() << right.getCol() << endl;
            AIList.push_back(right);
        }
        if (leftflag == false)
        {
            cout << "Left added: " << left.getRow() << left.getCol() << endl;
            AIList.push_back(left);
        }

        // removes non-null spaces from AIList
        vector<Coordinate> newList;
        for (unsigned i = 0; AIList.size() > i; i++)
        {
            if (AIList[i].isSpaceNull(board) == true)
            {
                newList.push_back(AIList[i]);
            }
        }

        AIList = newList;
    }

    cout << "AIList contains: " << endl;
    for (unsigned i = 0; i < AIList.size(); i++)
    {
        cout << AIList[i].getRow() << AIList[i].getCol() << endl;
    }
    cout << "--------" << endl;
}

bool Game::checkForPlayableTiles(LinkedList *list)
{
    bool movePossible = false;

    char i = 'A';
    int j = 0;
    int k = 0;

    if (list->getSize() != 0)
    {
        // iterate through board to check each cell
        while (i <= ROWS && !movePossible)
        {
            while (j < COLS && !movePossible)
            {
                // skip check if cell is already occupied
                if (board[i][j] == nullptr)
                {
                    // check every tile in list to see if any can be placed
                    while (k < list->getSize() && !movePossible)
                    {
                        // terminate function when a match is found
                        if (checkValid(list->getTile(k), i, j))
                        {
                            movePossible = true;
                        }
                        k++;
                    }
                    k = 0;
                }
                j++;
            }
            j = 0;
            i++;
        }

        // reset error checks
        this->cellOccupied = false;
        this->notAdjacent = false;
        this->tileMismatch = false;
        this->lineTooLong = false;
        this->sameTileInLine = false;
    }

    return movePossible;
}

void Game::newGame()
{
    this->firstMove = true;

    // intialise vector with nullptr
    board.resize(ROWS + 2);

    for (char i = 64; i <= ROWS + 1; i++)
    {
        board[i] = vector<Tile *>(COLS);
        for (int j = 0; j < COLS; j++)
        {
            board[i][j] = nullptr;
        }
    }

    // linked list for tiles stored in the tile bag
    newTileBag(tileBag);

    cout << endl;
    cout << "Starting a New Game" << endl;
    string p1;
    string p2;

    do
    {
        cin.ignore();
        cout << endl;
        cout << "Enter a name for player 1 (letters only)" << endl;

        cout << "> ";
        getline(cin, p1);

    } while (validateName(p1) != false);

    if (!cin.eof())
    {
        if (ai == true)
        {
            p2 = "AI";
        }
        else
        {
            do
            {
                cout << endl;
                cout << "Enter a name for player 2 (letters only)" << endl;

                cout << "> ";
                getline(cin, p2);

            } while (validateName(p2) != false);
        }
    }

    if (!cin.eof())
    {
        cout << endl;
        cout << "Let's Play!" << endl;
        // cin.ignore();

        // instantiate players
        player1 = new Player(p1);
        player2 = new Player(p2);

        // ADD FIRST 12 TILES TO THE PLAYERS' HANDs
        for (int i = 0; i < 12; i++)
        {
            player1->addTile(tileBag->getTile(0));
            tileBag->deleteElement(0);
            i++;
            player2->addTile(tileBag->getTile(0));
            tileBag->deleteElement(0);
        }
    }
}

bool Game::validateName(string name)
{
    bool validate = name.find_first_not_of("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ ") != string::npos;

    if (validate != false)
    {
        this->invalidCommand = true;
        displayErrors();
    }

    return validate;
}

void Game::newTileBag(LinkedList *tileBag)
{
    for (int i = 0; i < 6; i++)
    {
        for (int j = 0; j < 6; j++)
        {
            Colour colour = 'R';
            Shape shape = 0;

            switch (i)
            {
            case 0:
                colour = RED;
                break;
            case 1:
                colour = ORANGE;
                break;
            case 2:
                colour = YELLOW;
                break;
            case 3:
                colour = GREEN;
                break;
            case 4:
                colour = BLUE;
                break;
            case 5:
                colour = PURPLE;
                break;
            }

            switch (j)
            {
            case 0:
                shape = CIRCLE;
                break;
            case 1:
                shape = STAR_4;
                break;
            case 2:
                shape = DIAMOND;
                break;
            case 3:
                shape = SQUARE;
                break;
            case 4:
                shape = STAR_6;
                break;
            case 5:
                shape = CLOVER;
                break;
            }

            // repeated since there are 2 of each tile
            Tile *tile1 = new Tile(colour, shape);
            Tile *tile2 = new Tile(colour, shape);
            tileBag->addToRandomLocation(tile1);
            tileBag->addToRandomLocation(tile2);
        }
    }
}

void Game::printBoard(vector<vector<Tile *>> board)
{
    // print formatted columns headers
    cout << endl;
    cout << " ";
    for (int x = 0; x < COLS; x++)
    {
        if (x <= 10)
        {
            cout << "  " << x;
        }
        else
        {
            cout << " " << x;
        }
    }
    cout << endl;
    cout << "---------------------------------------------------------------------------------" << endl;

    // print grid
    for (char i = 'A'; i <= ROWS; i++)
    {
        // print row headers
        cout << i << " |";
        for (int j = 0; j < COLS; j++)
        {
            // if the tile is empty, print a double whitespace
            // else, print the colour and shape of the tile
            if (board[i][j] == nullptr)
            {
                cout << "  "
                     << "|";
            }
            else
            {
                cout << printColour(board, i, j) << board[i][j]->colour
                     << board[i][j]->shape << "\033[38;5;15m"
                     << "|";
            }
        }
        cout << endl;
    }
    cout << endl;
}

string Game::printColour(vector<vector<Tile *>> board, int i, int j)
{
    string colour = "";

    char c = board[i][j]->colour;

    if (disableColour == false)
    {

        // cout << [COLOUR] "text" << endl
        switch (c)
        {
        case 'R':
            colour = "\033[38;5;196m";
            break;
        case 'O':
            colour = "\033[38;5;208m";
            break;
        case 'Y':
            colour = "\033[38;5;226m";
            break;
        case 'G':
            colour = "\033[38;5;10m";
            break;
        case 'B':
            colour = "\033[38;5;33m";
            break;
        case 'P':
            colour = "\033[38;5;57m";
            break;
        default:
            colour = "";
        }
    }

    return colour;
}

void Game::saveGame(string filename)
{
    ofstream savefile;
    savefile.open("Saved/" + filename + ".save");
    savefile << player1->getName() << "\n";
    savefile << player1->getScore() << "\n";
    savefile << player1->viewHand(true) << "\n";
    savefile << player2->getName() << "\n";
    savefile << player2->getScore() << "\n";
    savefile << player2->viewHand(true) << "\n";

    // board dimensions
    int rowLength = 0;
    for (char i = 'A'; i <= ROWS; i++)
    {
        rowLength++;
    }
    savefile << COLS << "," << rowLength << "\n";

    // board contents
    string boardCont = "";
    for (char i = 'A'; i <= ROWS; i++)
    {
        for (int j = 0; j < COLS; j++)
        {
            if (board[i][j] != nullptr)
            {
                // Notation is Tile @ Board location (eg. B4@F9)
                boardCont.push_back(board[i][j]->colour);
                boardCont.append(to_string(board[i][j]->shape));
                boardCont.push_back('@');
                boardCont.push_back(i);
                boardCont.append(to_string(j));
                boardCont.push_back(',');
            }
        }
    }

    boardCont.pop_back(); // removes last comma in csv
    savefile << boardCont << "\n";

    savefile << tileBag->printTiles() << "\n";

    if (playerTurn == true)
    {
        savefile << player1->getName() << endl;
    }
    else
    {
        savefile << player2->getName() << endl;
    }

    savefile.close();
}

bool Game::loadFile(string filename)
{
    bool fileExist;
    ifstream loadfile("Saved/" + filename + ".save");
    if (loadfile.fail())
    {
        fileExist = false;
    }
    else
    {

        fileExist = true;
        string temp; // string holds data as it is read in

        // player 1 name
        getline(loadfile, temp);
        player1 = new Player(temp);

        // player 1 score
        getline(loadfile, temp);
        player1->addScore(stoi(temp));

        // player 1 hand
        readHand(loadfile, player1);

        // player 2 name
        getline(loadfile, temp);
        player2 = new Player(temp);

        // player 2 score
        getline(loadfile, temp);
        player2->addScore(stoi(temp));

        // player 2 hand
        readHand(loadfile, player2);

        // Board shape (CURRENTLY NOT STORED ANYWHERE)
        readBSize(loadfile);

        // Board state
        board = readBoard(loadfile);

        // Tile bag contents
        readBag(loadfile);

        // current player turn name
        getline(loadfile, temp);
        if (temp == player1->getName())
        {
            playerTurn = true;
        }
        else if (temp == player2->getName())
        {
            playerTurn = false;
        }
    }
    loadfile.close();

    return fileExist;
}

void Game::readBag(ifstream &stream)
{

    string line, tileString;
    char delimiter = ',';

    getline(stream, line);
    stringstream sstream(line);
    Tile *tile;

    while (getline(sstream, tileString, delimiter))
    {
        tile = new Tile(tileString.at(0), tileString.at(1) - 48);
        tileBag->addToBack(tile);
    }
}

void Game::readHand(ifstream &stream, Player *player)
{
    string line, tileString;
    char delimiter = ',';

    getline(stream, line);
    stringstream sstream(line);
    Tile *tile;

    while (getline(sstream, tileString, delimiter))
    {
        tile = new Tile(tileString.at(0), tileString.at(1) - 48);
        player->addTile(tile);
    }
}

int *Game::readBSize(ifstream &stream)
{

    int *boardSize = new int[2];
    string line, dim;
    char delimiter = ',';

    getline(stream, line);
    stringstream sstream(line);

    int i = 0;
    while (getline(sstream, dim, delimiter))
    {
        boardSize[i] = stoi(dim);
        i++;
    }

    return boardSize;
}

vector<vector<Tile *>> Game::readBoard(ifstream &stream)
{
    string line, coord;
    vector<vector<Tile *>> board(ROWS + 2);

    for (char i = 64; i <= ROWS + 1; i++)
    {
        board[i] = vector<Tile *>(COLS);
        for (int j = 0; j < COLS; j++)
        {
            board[i][j] = nullptr;
        }
    }

    char delimiter = ',';
    Tile *tile;

    getline(stream, line);
    stringstream sstream(line);

    if (!line.empty())
    {
        while (getline(sstream, coord, delimiter))
        {
            tile = new Tile(coord.at(0), coord.at(1) - 48);

            // if statement here to check for 2 digit int on board coord
            if (coord.length() == 5)
            {
                board[coord.at(3)][coord.at(4) - 48] = tile;
            }
            else
            {
                string twoDigit;
                twoDigit.push_back(coord.at(4));
                twoDigit.push_back(coord.at(5));
                board[coord.at(3)][stoi(twoDigit)] = tile;
            }
        }
    }
    else
    {

        this->firstMove = true;
    }

    return board;
}

int Game::placeTile(Tile *tile, char row, int col)
{
    // calculate and return score earned
    int score = 0;
    board[row][col] = tile;
    score += calculateScore(row, col);

    return score;
}

bool Game::checkValid(Tile *tile, char row, int col)
{
    // only remains valid if all checks are passed
    bool valid = true;

    // row of cell being tested for tile match
    Colour tempRow = row;
    // column of cell being tested for tile match
    Shape tempCol = col;
    // length of line of tiles
    int rowLength = 1;
    int colLength = 1;
    // bool to confirm colours match
    bool colourCheck = true;
    // bool to confirm shapes match
    bool shapeCheck = true;

    // skip if tile is null
    if (tile == nullptr)
    {
        this->tileNotInHand = true;
        valid = false;
    }

    // first move ignores other checks
    if (!firstMove)
    {
        // ensure cell does not already contain a tile
        if (valid &&
            board[row][col] != nullptr)
        {
            this->cellOccupied = true;
            valid = false;
        }

        // ensure cell is adjacent an existing tile
        if (valid &&
            (row == 'A' || board[row - 1][col] == nullptr) &&
            (row == ROWS || board[row + 1][col] == nullptr) &&
            (col == 0 || board[row][col - 1] == nullptr) &&
            (col == COLS - 1 || board[row][col + 1] == nullptr))
        {
            this->notAdjacent = true;
            valid = false;
        }

        // check vertical line
        // check tiles above
        while (valid && tempRow > 'A' && board[tempRow - 1][tempCol] != nullptr)
        {
            if (board[tempRow - 1][tempCol]->colour != tile->colour)
            {
                colourCheck = false;
            }
            if (board[tempRow - 1][tempCol]->shape != tile->shape)
            {
                shapeCheck = false;
            }
            if (board[tempRow - 1][tempCol]->colour == tile->colour &&
                board[tempRow - 1][tempCol]->shape == tile->shape)
            {
                this->sameTileInLine = true;
                valid = false;
            }
            if (!colourCheck && !shapeCheck)
            {
                this->tileMismatch = true;
                valid = false;
            }
            if (colLength > 5)
            {
                this->lineTooLong = true;
                valid = false;
            }

            colLength++;
            tempRow--;
        }

        // check tiles below
        tempRow = row;
        while (valid && tempRow < ROWS && board[tempRow + 1][tempCol] != nullptr)
        {
            if (board[tempRow + 1][tempCol]->colour != tile->colour)
            {
                colourCheck = false;
            }
            if (board[tempRow + 1][tempCol]->shape != tile->shape)
            {
                shapeCheck = false;
            }
            if (board[tempRow + 1][tempCol]->colour == tile->colour &&
                board[tempRow + 1][tempCol]->shape == tile->shape)
            {
                this->sameTileInLine = true;
                valid = false;
            }
            if (!colourCheck && !shapeCheck)
            {
                this->tileMismatch = true;
                valid = false;
            }
            if (colLength > 5)
            {
                this->lineTooLong = true;
                valid = false;
            }

            colLength++;
            tempRow++;
        }

        // reset and check horizontal line
        // check tiles to left
        tempRow = row;
        colourCheck = true;
        shapeCheck = true;
        while (valid && tempCol > 1 && board[tempRow][tempCol - 1] != nullptr)
        {
            if (board[tempRow][tempCol - 1]->colour != tile->colour)
            {
                colourCheck = false;
            }
            if (board[tempRow][tempCol - 1]->shape != tile->shape)
            {
                shapeCheck = false;
            }
            if (board[tempRow][tempCol - 1]->colour == tile->colour &&
                board[tempRow][tempCol - 1]->shape == tile->shape)
            {
                this->sameTileInLine = true;
                valid = false;
            }
            if (!colourCheck && !shapeCheck)
            {
                this->tileMismatch = true;
                valid = false;
            }
            if (rowLength > 5)
            {
                this->lineTooLong = true;
                valid = false;
            }

            rowLength++;
            tempCol--;
        }

        // check tiles to right
        tempCol = col;
        while (valid && tempCol < COLS && board[tempRow][tempCol + 1] != nullptr)
        {
            if (board[tempRow][tempCol + 1]->colour != tile->colour)
            {
                colourCheck = false;
            }
            if (board[tempRow][tempCol + 1]->shape != tile->shape)
            {
                shapeCheck = false;
            }
            if (board[tempRow][tempCol + 1]->colour == tile->colour &&
                board[tempRow][tempCol + 1]->shape == tile->shape)
            {
                this->sameTileInLine = true;
                valid = false;
            }
            if (!colourCheck && !shapeCheck)
            {
                this->tileMismatch = true;
                valid = false;
            }
            if (rowLength > 5)
            {
                this->lineTooLong = true;
                valid = false;
            }

            rowLength++;
            tempCol++;
        }
    }
    return valid;
};

int Game::calculateScore(char row, int col)
{
    // length of line of tiles
    int rowLength = 0;
    int colLength = 0;

    // row of cell being tested
    Colour tempRow = row;
    // column of cell being tested
    Shape tempCol = col;

    // vertical line
    // tiles above
    while (board[tempRow - 1][tempCol] != nullptr && tempRow > 'A')
    {
        colLength++;
        tempRow--;
    }
    tempRow = row;

    // tiles below
    while (board[tempRow + 1][tempCol] != nullptr && tempRow < ROWS)
    {
        colLength++;
        tempRow++;
    }
    tempRow = row;

    // horizontal line
    // tiles to left
    while (board[tempRow][tempCol - 1] != nullptr && tempCol > 1)
    {
        rowLength++;
        tempCol--;
    }
    tempCol = col;

    // tiles to right
    while (board[tempRow][tempCol + 1] != nullptr && tempCol < COLS)
    {
        rowLength++;
        tempCol++;
    }

    // add point for first tile in each line (not counted by above algorithm)
    if (rowLength > 0)
    {
        rowLength++;
    }
    if (colLength > 0)
    {
        colLength++;
    }

    // add bonus score for each 'quirkle' completed
    if (rowLength > 5)
    {
        rowLength += 6;
        this->qwirkleScored = true;
    }
    if (colLength > 5)
    {
        colLength += 6;
        this->qwirkleScored = true;
    }

    return rowLength + colLength;
}

void Game::getPrompt(Player *player)
{
    // display board and scores
    printBoard(board);

    // display message for scoring a Qwirkle
    if (this->qwirkleScored)
    {
        cout << "Qwirkle scored!" << endl;
        this->qwirkleScored = false;
    }

    cout << player1->getName() << ": " << player1->getScore() << " points" << endl;
    cout << player2->getName() << ": " << player2->getScore() << " points" << endl;
    cout << endl;
    cout << player->getName() << ", it's your turn, your hand is:" << endl;
    cout << player->viewHand(disableColour) << endl;
    cout << endl;

    string input;

    // flag variable to check if input is valid:
    bool valid = false;

    // user prompt asked until a valid input is found
    while (valid == false && input != "^D" && !cin.eof())
    {
        cout << "> ";
        getline(cin, input);
        cout << endl;

        // place a tile on board
        // if the first world is 'place' and the third world is 'at' and the input
        // is not longer than 15 characters
        if (sameCaseInsensitive(input.substr(0, 5), "place") && input.length() > 13 && sameCaseInsensitive(input.substr(9, 2), "at") && input.length() <= 15)
        {
            Colour colour = toupper(input.at(6));
            Shape shape = input.at(7) - '0';
            char row = toupper(input.at(12));
            int col = 0;

            if (input.length() == 14)
            {
                col = input.at(13) - '0';

                // check if tile is in player's hand and the move is valid
                if (checkValid(player->checkTile(colour, shape), row, col))
                {
                    // place tile on board and award score (excludes first move of game)
                    player->addScore(placeTile(player->playTile(colour, shape), row, col));
                    valid = true;

                    // add squares to AIList if AI enabled
                    if (ai == true)
                    {
                        addNullAdjacentSpaces(row, col);
                    }

                    // unset firstMove after first tile is placed
                    if (this->firstMove)
                    {
                        // first move of game always scores 1 point
                        player->addScore(1);
                        this->firstMove = false;
                    }
                }
                else
                {
                    // display error message and display tiles to player again after a failure
                    displayErrors();
                    cout << endl;
                    cout << "Tiles available:" << endl;
                    cout << player->viewHand(disableColour) << endl;
                }
            }
            else if (input.length() == 15)
            {
                col = stoi(input.substr(13, 2));

                if (col > 25)
                {
                    valid = false;
                    cout << "Column value out of range. Please try again. \n"
                         << endl;
                }
                else
                {
                    // check if tile is in player's hand and the move is valid
                    if (checkValid(player->checkTile(colour, shape), row, col))
                    {
                        // place tile on board and award score (excludes first move of game)
                        player->addScore(placeTile(player->playTile(colour, shape), row, col));
                        valid = true;

                        // unset firstMove after first tile is placed
                        if (this->firstMove)
                        {
                            // first move of game always scores 1 point
                            player->addScore(1);
                            this->firstMove = false;
                        }
                    }
                    else
                    {
                        // display error message and display tiles to player again after a failure
                        displayErrors();
                        cout << endl;
                        cout << "Tiles available:" << endl;
                        cout << player->viewHand(disableColour) << endl;
                    }
                }
            }
        }
        // swap a tile with one from the bag
        // if the first world is 'replace' and the input is not longer than 10 characters
        else if (sameCaseInsensitive(input.substr(0, 7), "replace") && input.length() == 10)
        {
            Colour colour = toupper(input.at(8));
            Shape shape = input.at(9) - '0';

            Tile *toReplace = player->playTile(colour, shape);
            // check if tile is in player's hand, and if it is,
            // replace it with another tile from the tilebag
            if (toReplace != nullptr)
            {
                valid = true;
                tileBag->addToRandomLocation(toReplace);
            }
            else
            {
                // display error
                this->tileNotInHand = true;
                displayErrors();
                cout << endl;
                cout << "Tiles available:" << endl;
                cout << player->viewHand(disableColour) << endl;
            }
        }
        // command to save game
        // if the first world is "save" and the file name has at least one letter
        else if (sameCaseInsensitive(input.substr(0, 4), "save") && input.length() >= 5)
        {
            if (firstMove == false)
            {
                string filename = input.erase(0, 5);
                saveGame(filename);

                // ask for prompt again as save is an extra action that shouldn't
                // prevent the player to make a move
                cout << "Game successfully saved as '" << filename << "'." << endl;
                cout << "What is your next move? \n"
                     << endl;
                valid = false;
            }
            else
            {
                cout << "No game data to save" << endl;
            }
        }
        // command to quit game
        else if (input == "^D" || cin.eof())
        {
            // stop game
            this->gameOver = true;

            // stop loop
            valid = true;
        }
        else
        {
            this->invalidCommand = true;
            displayErrors();
            cout << endl;
            cout << "Tiles available:" << endl;
            cout << player->viewHand(disableColour) << endl;
        }
    }
}

bool Game::sameCaseInsensitive(string s1, string s2)
{
    if (strcasecmp(s1.c_str(), s2.c_str()) == 0)
    {
        return true;
    }
    return false;
}

void Game::clearScreen()
{
    // left out to allow test cases and for lecturer viewing
    // cout << "\x1B[2J\x1B[H";
}

void Game::checkArgs(int argc, char **argv)
{

    // iterate through argv and check each argument for a related command

    for (int i = 1; i < argc; i++)
    {
        // ENABLE ai
        if (strcmp(argv[i], "--ai") == 0)
        {
            this->ai = true;
        }

        // disable ALL enhancements
        if (strcmp(argv[i], "--enh") == 0)
        {
            this->disableColour = true;
        }

        // disable coloured tiles
        if (strcmp(argv[i], "--col") == 0)
        {
            this->disableColour = true;
        }
    }
}

void Game::displayErrors()
{
    // display error message to player and reset error
    if (this->cellOccupied)
    {
        cout << "Cell already contains a tile, please try again" << endl;
        this->cellOccupied = false;
    }
    if (this->notAdjacent)
    {
        cout << "Tile must be placed next to another tile, please try again" << endl;
        this->notAdjacent = false;
    }
    if (this->tileMismatch)
    {
        cout << "Tile does not match, please try again" << endl;
        this->tileMismatch = false;
    }
    if (this->lineTooLong)
    {
        cout << "Line cannot exceed 6 tiles, please try again" << endl;
        this->lineTooLong = false;
    }
    if (this->tileNotInHand)
    {
        cout << "No such tile in hand, please try again" << endl;
        this->tileNotInHand = false;
    }
    if (this->sameTileInLine)
    {
        cout << "Can't have duplicate tiles in the same line, please try again" << endl;
        this->sameTileInLine = false;
    }
    // generic error if you don't want to create a new one
    if (this->invalidCommand)
    {
        cout << "Invalid input, please try again" << endl;
        this->invalidCommand = false;
    }
}