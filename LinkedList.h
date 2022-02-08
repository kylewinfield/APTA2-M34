#ifndef ASSIGN2_LINKEDLIST_H
#define ASSIGN2_LINKEDLIST_H

#include "Node.h"
#include <string>

using std::string;

class LinkedList
{
public:
   LinkedList();
   ~LinkedList();

   int getSize();
   //add new node to front of list
   void addToFront(Tile *front);
   //add new node to back of list
   void addToBack(Tile *back);
   //add new node to position 'i' place in list
   void addToSpecificLocation(Tile *tile, int i);
   //add new node to random position in list
   void addToRandomLocation(Tile *tile);
   //delete element at location 'i' from list
   void deleteElement(int i);
   //return contents of tile from node 'i'
   Tile *getTile(int i);
   //place tile from node 'i' on board and delete from linked list
   Tile *placeTile(int i);
   //extract node 'i' from linked list and transfer it to 'list'
   void transferNode(int i, LinkedList *list);
   //prints contents of tile bag 
   string printTiles();

private:
   //pointer to the first node in the linked list
   Node *head;
   //pointer to the last node in the linked list
   Node *tail;
   //size of list
   int size;
};

#endif // ASSIGN2_LINKEDLIST_H
