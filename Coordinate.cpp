
#include <vector>
#include <iostream>
#include "Coordinate.h"
#include "Tile.h"

using std::vector;

Coordinate::Coordinate(char row, int col){

    this->row = row;
    this->col = col;

};

char Coordinate::getRow(){
    return row;
}

int Coordinate::getCol(){
    return col;
}

bool Coordinate::isEqual(Coordinate coord1){
    bool equal = false;
    if(coord1.getCol() == col && coord1.getRow() == row){
        equal = true;
    }

    return equal;
}

bool Coordinate::isSpaceNull(vector<vector<Tile *>> board){
    bool check = false;
    if (board[row][col] == nullptr){
        check = true;
    }

    return check;

}