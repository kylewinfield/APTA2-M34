#include <iostream>

#include "Game.h"
#include "LinkedList.h"
#include "Player.h"

using std::cin;
using std::cout;
using std::endl;
using std::ofstream;
using std::string;
using std::vector;

void menu();
void showCredits();
void printBoard(vector<vector<Tile *>> board);
void clearScreen();
string loadGame();

Game *game;
bool quit;

int main(void)
{
   // on launch message
   cout << "Welcome to Qwirkle!" << endl;
   cout << "-------------------" << endl;

   quit = false;
   while (!quit && !cin.eof())
   {
      menu();
   }

   // checks for Control-D / EOF
   if (cin.eof())
   {
      cout << endl;
      cout << "\nGoodbye!" << endl;
      cout << endl;
   }

   return EXIT_SUCCESS;
}

// main selection menu implemented as switch statement
void menu()
{
   cout << "Menu" << endl;
   cout << "----" << endl;
   cout << "1. New Game" << endl;
   cout << "2. Load Game" << endl;
   cout << "3. Credits (Show student information)" << endl;
   cout << "4. Quit" << endl;
   cout << endl;

   int input;

   cout << "> ";
   cin >> input;

   if (!cin.eof())
   {
      switch (input)
      {
      case 1:
         // create new game
         game = new Game();
         game->newGame();
         // start game with loaded parameters
         if (!cin.eof())
         {
            game->gameLoop();
            delete game;
            clearScreen();
         }
         break;
      case 2:
         game = new Game();
         if (game->loadFile(loadGame()) == true)
         {
            // start game with loaded parameters
            cin.ignore();
            game->gameLoop();
            delete game;
         }
         else if (cin.eof())
         {
            cout << endl;
            cout << "Goodbye!" << endl;
            cout << endl;
            quit = true;
         }
         else
         {
            cout << "File does not exist";
            cout << endl;
         }
         // clear screen
         clearScreen();
         break;
      case 3:
         showCredits();
         break;
      case 4:
         cout << endl;
         cout << "Goodbye!" << endl;
         cout << endl;
         quit = true;
         break;
      default:
         cout << "Invalid input. Try again:";
         cout << endl;
      }
   }
}

// prompts user to enter name of game file to load (may need error checking)
string loadGame()
{
   string filename;
   cout << endl;
   cout << "Enter name of save file:";
   cout << endl;
   cout << "> ";
   cin >> filename;
   cout << endl;

   return filename;
}

void showCredits()
{

   cout << endl;
   cout << "----------------------------------" << endl;
   cout << "Name: Jacopo Grabar" << endl;
   cout << "Student ID: s3876518" << endl;
   cout << "Email: s3876518@student.rmit.edu.au" << endl;
   cout << endl;

   cout << "Name: Alasdair Cameron" << endl;
   cout << "Student ID: s3884660" << endl;
   cout << "Email: s3884660@student.rmit.edu.au" << endl;
   cout << endl;

   cout << "Name: Kyle Winfield" << endl;
   cout << "Student ID: s3541867" << endl;
   cout << "Email: s3541867@student.rmit.edu.au" << endl;
   cout << endl;

   cout << "Name: Murray Lowis" << endl;
   cout << "Student ID: s3862651" << endl;
   cout << "Email: s3862651@student.rmit.edu.au" << endl;
   cout << endl;

   cout << "----------------------------------" << endl;
   cout << endl;
}

void clearScreen()
{
   // left out to allow test cases and for lecturer viewing
   // cout << "\x1B[2J\x1B[H";
}