#include <iostream>
#include <random>
#include "LinkedList.h"
#include <string>

using std::string;
using std::to_string;

LinkedList::LinkedList()
{
   this->head = nullptr;
   this->tail = nullptr;
   this->size = 0;
};

LinkedList::~LinkedList()
{
   // current node to be deleted - start at head
   Node *node = head;
   // next node to be deleted
   Node *nextNode;
   while (this->size > 0)
   {
      // mark node to be deleted in next pass
      nextNode = node->next;
      // delete contents pointed to by temp node
      delete node;
      // set next node to be deleted
      node = nextNode;
      this->size--;
   }
};

void LinkedList::deleteElement(int i)
{
   // if more than one node has been created
   if (this->size > 0)
   {
      Node *node = head;

      // if deleting head
      if (i == 0)
      {
         // mark next node as new head
         this->head = node->next;
         node->tile = nullptr;
         delete node;
         // this->head = nullptr;
      }
      // if deleting tail
      else if (i == this->size - 1)
      {
         // std::cout << "deleting tail" << std::endl;
         Node *secondLast = nullptr;
         while (node->next != nullptr)
         {
            secondLast = node;
            node = node->next;
         }
         // mark 2nd last node as new tail
         this->tail = node;
         node->tile = nullptr;
         delete node;
         this->tail = nullptr;
         this->tail = secondLast;
      }
      // if deleting other node
      else
      {
         // identify node prior to node to be deleted
         while (i > 1)
         {
            node = node->next;
            i--;
         }
         // mark node for deletion
         Node *deletedNode = node->next;
         // update previous node's 'next' pointer
         node->next = node->next->next;
         // delete node
         deletedNode->tile = nullptr;
         delete deletedNode;
      }
      // reduce size of list
      this->size--;
   }
   // if no nodes have been created
   else
   {
      std::cout << "No nodes to delete" << std::endl;
   }
};

void LinkedList::addToFront(Tile *front)
{
   Node *headNode = new Node(front, NULL);

   // also set as tail if first node
   if (this->size == 0)
   {
      this->head = headNode;
      this->tail = headNode;
      this->head->next = tail;
   }
   // otherwise replace head
   else
   {
      headNode->next = this->head;
      this->head = headNode;
   }
   this->size++;
};

void LinkedList::addToBack(Tile *back)
{
   Node *tailNode = new Node(back, NULL);

   // also set as head if first node
   if (this->size == 0)
   {
      this->head = tailNode;
      this->tail = tailNode;
      this->head->next = tailNode;
   }
   // otherwise replace existing tail
   else
   {
      this->tail->next = tailNode;
      this->tail = tailNode;
   }
   this->size++;
};

void LinkedList::addToSpecificLocation(Tile *tile, int i)
{
   // only allow if the list size is <= i
   if (i <= this->size)
   {
      // new node to be added
      Node *node = new Node(tile, NULL);

      // set as both head and tail if first node to be created
      if (this->size == 0)
      {
         this->head = node;
         this->tail = node;
         this->head->next = tail;
      }
      // replace head if i == 0
      else if (i == 0)
      {
         node->next = this->head;
         this->head = node;
      }
      else
      {
         // node after which the new node is to be placed
         Node *previous = this->head;

         // iterate until finding preceding node
         while (i > 1)
         {
            previous = previous->next;
            i--;
         }

         // point new node to node whose position it's taking
         node->next = previous->next;
         // point previous node to new node
         previous->next = node;
      }
      this->size++;
   }
   else
   {
      std::cout << "Out of bounds" << std::endl;
   }
};

void LinkedList::addToRandomLocation(Tile *tile)
{
   int min = 0;
   int max = this->size;
   std::random_device engine;
   std::uniform_int_distribution<int> distribution(min, max);

   int value = distribution(engine);

   addToSpecificLocation(tile, value);
};

Tile *LinkedList::getTile(int i)
{
   Tile *returnTile = nullptr;
   if (i < size)
   {
      // node containing tile to return
      Node *node = this->head;

      // iterate until finding node
      while (i > 0)
      {
         node = node->next;
         i--;
      }
      returnTile = node->tile;
   }

   return returnTile;
}

string LinkedList::printTiles()
{
   string tiles = "";
   string colour, TEST;
   int shape;
   // node containing tile to return
   Node *node = this->head;

   int i = 0;

   // iterate until finding node
   while (i < size)
   {
      node = node->next;

      colour = this->getTile(i)->colour;
      shape = this->getTile(i)->shape;

      i++;

      tiles.append(colour);
      tiles.append(to_string(shape));
      if (i < size)
      {
         tiles.append(",");
      }
   }

   return tiles;
}

int LinkedList::getSize()
{
   return this->size;
}