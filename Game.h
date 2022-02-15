#ifndef ASSIGN2_GAME_H
#define ASSIGN2_GAME_H

#include <string>
#include <vector>

#include "Node.h"
#include "LinkedList.h"
#include "TileCodes.h"
#include "Player.h"
#include "Coordinate.h"

#define ROWS 'Z'
#define COLS 26

using std::ifstream;
using std::string;
using std::vector;

class Game
{
public:
   Game();
   Game(int argc, char** argv);
   ~Game();

   // create a new game
   void newGame();
   // save the current game to a file
   void saveGame(string filename);
   // loads game from file
   bool loadFile(string filename);
   // main loop handling gameplay and player turns
   void gameLoop();
   // populate the tile bag with a new set of tiles for a new game
   void newTileBag(LinkedList *tileBag);
   // output the current gameboard to terminal
   void printBoard(vector<vector<Tile *>> board);
   // checks if list contains tile that is able to be placed on game board
   bool checkForPlayableTiles(LinkedList *list);
   // verify usernames are valid
   bool validateName(string name);
   // request input from one of the players
   void getPrompt(Player *player);
   // checks if two strings are equivalent - case insensitive
   bool sameCaseInsensitive(string s1, string s2);

   // place a player held tile on the board - returns score from move or zero if move invalid
   int placeTile(Tile *tile, char row, int col);
   // calculate and return score from tile placed at coordinates
   int calculateScore(char row, int col);
   // check if player move is valid under Qwirkle rules
   bool checkValid(Tile *tile, char row, int col);

   // methods for reading data from save file:
   // reads in a player hand returns linked list
   void readHand(ifstream &stream, Player *player);
   // reads board type line in and returns 2D vector of board
   vector<vector<Tile *>> readBoard(ifstream &stream);
   // reads in board size, returns in int array
   int *readBSize(ifstream &stream);
   // reads in tile bag data from file
   void readBag(ifstream &stream);

   // clears the current contents of the terminal
   void clearScreen();
   // displays error messages to the player
   void displayErrors();

   //checks arguement for enhancement disable
   void checkArgs(int argc, char** argv);

   // colour board tiles
   string printColour(vector<vector<Tile *>> board, int i, int j);
   
   //AI related functions:
   void AITurn();
   void addNullAdjacentSpaces(char row, int col);

private:
   vector<vector<Tile *>> board;
   LinkedList *tileBag;
   Player *player1;
   Player *player2;

   // check whether first tile has been placed yet
   bool firstMove;
   // check whether the game is still ongoing
   bool gameOver;
   // check whether player scored a 'Qwirkle'
   bool qwirkleScored;

   // true = player 1 turn, false = player 2 turn
   bool playerTurn;

   // error checks
   // check if cell on board is available
   bool cellOccupied;
   // check if cell on board contains tile in adjacent cells
   bool notAdjacent;
   // check if tile matches adjacent tiles
   bool tileMismatch;
   // check if tile won't exceed max line length of 6 for 'Qwirkle'
   bool lineTooLong;
   // check if tile is in player hand
   bool tileNotInHand;
   // check if there are any duplicate tiles in the line already
   bool sameTileInLine;
   // generic error
   bool invalidCommand;

   // enhancments states for disabling/enabling via command line
   bool ai;
   bool disableColour;

   // list of spaces around placed tiles that are vacant (used for AI implementation)
   vector <Coordinate> AIList;
};

#endif // ASSIGN2_GAME_H