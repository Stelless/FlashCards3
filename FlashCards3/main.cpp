#include <iostream>
#include <conio.h>
#include <string>
#include <map>
#include <stdlib.h>

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include "Card.h"

using namespace std;
namespace pt = boost::property_tree; // Short alias for this namespace

#define dbgln(x) cout << #x << ": " << (x) << endl;
#define dbg(x) cout << #x << ": " << (x) << "   ";
using CardId = string;
using CardPtr = shared_ptr<Card>;
using Deck = std::map<CardId, CardPtr>;

// Purpose: LoadDeck
void loadDeck(std::string deck_file_name, Deck& deck) {

   namespace pt = boost::property_tree; // Short alias for this namespace

   pt::ptree root; // Creates a property tree called root
   try {
      pt::read_json(deck_file_name, root); // Uses boost library to load JSON file into ptree

      std::cout << "=================Starting Loading Deck=================\n";
      for (pt::ptree::value_type& card : root.get_child("cards"))
      {
         //Loading info from JSON file
         Card new_card{};
         new_card = new_card.loadCard(card);
         // Putting info into deck map
         std::shared_ptr<Card> new_card_ptr = std::make_shared<Card>(new_card); // This uses the copy constructor?, can also be moved potentially
         deck.insert(std::pair<std::string, std::shared_ptr<Card>>(new_card_ptr->id, new_card_ptr));
      }
   }
   catch (const std::exception& e) {
      std::cout << "Error loading cards: " << e.what() << "\n";
      cin.ignore();
      exit(1); // Exit the program now with an non-zero exit code, indicating a problem
   }
   std::cout << "=================Finished Loading Deck=================\n";
}

// Purpose: Print Menu Options to Screen
void printOptionMenu() {
   cout << "\nHello! Please select an option:" << endl;
   cout << "X:      Start Flash Card Excersizes" << endl;
   cout << "C:      Print All Flash Cards in Current Deck" << endl;
   cout << "F:      Load Different Flash Card Deck " << endl;
   cout << "H:      Program Help (TBI)" << endl;
   cout << "Q:      Exit the Program" << endl;
   return;
}

// Purpose: Displays the contents of card.
void displayCard(CardPtr& current_card)
{
   system("CLS");
   current_card->printId();
   current_card->printQuestion();
   static_cast<void>(_getch()); // may prduce a warning, should be okay :D
   current_card->printAnswer();
}

// Purpose: Remove top card from play_deck and point to the next card in the serieas
bool drawNextCard(Deck& play_deck, CardPtr& current_card) {
   bool success{ false };
   if (play_deck.empty()) {
      cout << "Play deck is empty" << endl;
   }
   else {
      current_card = play_deck.begin()->second;//points to the first card of play_deck;
      play_deck.erase(play_deck.begin());
      success = true;
   }
   return success;
}

// Purpose: Main
int main()
{
   std::string default_file_name{ "defaultcards.json" };

   std::cout << "Welcome!  (defaultcards.json is run by default).\n";
   printOptionMenu();

   Deck master_deck{}; 
   loadDeck(default_file_name, master_deck);
   Deck play_deck = master_deck; // important that we're using shared pointers otherwaise we would have to worry about shallow vs deep copy.
   CardPtr current_card{};



   bool done{ false };
   while (!done) {
      auto key = _getch();
      char cmd = static_cast<char>(key);
      cmd = std::toupper(cmd);
      cout << "CMD: " << cmd << endl;
      switch (cmd) {
      case 'N':
         //Advance to next card and print current card

         if (drawNextCard(play_deck, current_card)) {
            displayCard(current_card);
         }
         
         break;
      case 'H':
         printOptionMenu();
         break;
      case 'Q':
         done = true;
         break;
      default:
         break;
      }
   }
}