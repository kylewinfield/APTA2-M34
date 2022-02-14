#include <iostream>
#include <string>

#include "Player.h"

Player::Player(string playerName)
{
    this->playerName = playerName;
    this->playerScore = 0;
    this->playerHand = new LinkedList();
}

Player::~Player()
{
    delete playerHand;
}

string Player::getName()
{
    return playerName;
}

LinkedList *Player::getHand()
{
    return playerHand;
}

int Player::getScore()
{
    return this->playerScore;
}

void Player::addScore(int score)
{
    playerScore += score;
}

void Player::addTile(Tile *tile)
{
    if (tile != nullptr)
        playerHand->addToBack(tile);
}

int Player::getHandSize()
{
    return playerHand->getSize();
}

Tile *Player::checkTile(Colour checkColour, Shape checkShape)
{
    Tile *returnTile = nullptr;
    for (int i = 0; i < 6; i++)
    {
        // Run through hand, checking Tile only if Tile exists in that position in the hand
        // and if requested tile not already found
        if (playerHand->getTile(i) != nullptr && returnTile == nullptr)
            if (playerHand->getTile(i)->colour == checkColour && playerHand->getTile(i)->shape == checkShape)
            {
                returnTile = playerHand->getTile(i);
            }
    }
    return returnTile;
}

Tile *Player::playTile(Colour checkColour, Shape checkShape)
{
    Tile *returnTile = nullptr;
    for (int i = 0; i < 6; i++)
    {
        // Run through hand, checking Tile only if Tile exists in that position in the hand
        // and if requested tile not already found
        if (playerHand->getTile(i) != nullptr && returnTile == nullptr)
            if (playerHand->getTile(i)->colour == checkColour && playerHand->getTile(i)->shape == checkShape)
            {
                returnTile = playerHand->getTile(i);
                playerHand->deleteElement(i);
            }
    }
    return returnTile;
}

string Player::viewHand(bool disableColour)
{
    string playerHandString = "";
    bool firstLoop = true;
    for (int i = 0; i < 6; i++)
    {
        // Run through hand, checking Tile only if Tile exists in that position in the hand
        if (playerHand->getTile(i) != nullptr)
        {
            // Add formatting if not the first tile
            if (firstLoop == false)
            {
                playerHandString.append(",");
            }
            char colourChar = playerHand->getTile(i)->colour;
            string shapeString = to_string(playerHand->getTile(i)->shape);

            if (disableColour == false)
            {
                playerHandString += printColour(i);
            }
            playerHandString += colourChar;
            playerHandString.append(shapeString);
            if (disableColour == false)
            {
                playerHandString.append("\033[38;5;15m");
            }
            firstLoop = false;
        }
    }
    return playerHandString;
}

string Player::printColour(int i)
{
    string colour;

    char c = playerHand->getTile(i)->colour;
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

    return colour;
}