#ifndef ASSIGN2_PLAYER_H
#define ASSIGN2_PLAYER_H

#include <string>

#include "Tile.h"
#include "LinkedList.h"

using std::string;
using std::to_string;

class Player
{
public:
    Player(string playerName);
    ~Player();
    string getName();
    LinkedList* getHand();
    int getScore();
    void addScore(int score);
    void addTile(Tile* tile);
    string viewHand();
    int getHandSize();
    Tile *checkTile(Colour checkColour, Shape checkShape);

    // Method to check if the tile exists in the players hand.
    // If tile exists, removes it from the player's hand and returns
    // pointer to it.
    Tile* playTile(Colour colour, Shape shape);

private:
    string playerName;
    int playerScore;
    LinkedList* playerHand;
};

#endif // ASSIGN2_PLAYER_H