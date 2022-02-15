#include <vector>
#include "Tile.h"

using std::vector;
class Coordinate{

public:
Coordinate(char row, int col);
char getRow();
int getCol();

// returns true if coordinates are the same
bool isEqual(Coordinate coord1);
// returns true if no tile on board at that coordinate
bool isSpaceNull(vector<vector<Tile *>> board);

private:
char row;
int col;

};