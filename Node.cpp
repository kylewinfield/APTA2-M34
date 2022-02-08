#include "Node.h"

Node::Node(Tile *tile, Node *next)
{
   //TODO - define how new tiles are created
   this->tile = tile;
   this->next = next;
}

Node::Node(Node &other)
{
   // TODO
}
