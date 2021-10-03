#include <iostream>
#include <conio.h>
#include <string>
#include <sstream>
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
enum class AnswerMode { AnswerFirst = 0, QuestionFirst };
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
   cout << "N:      Start Flash Card Excersizes" << endl;
   cout << "X:      Make Hold Deck the Current Play Deck" << endl;
   cout << "S:      Save in Hold Deck" << endl;
   cout << "L:      Look up Card by ID in master" << endl;
   cout << "C:      Print All Flash Cards in Current Deck" << endl;
   cout << "F:      Load Different Flash Card Deck " << endl;
   cout << "T:      Toggle Question/Answer first " << endl;
   cout << "H:      Program Help (TBI)" << endl;
   cout << "R:      Reset Play Deck to Master" << endl;
   cout << "Q:      Exit the Program" << endl;
   return;
}

// Purpose: Displays the contents of card.
void displayCard(CardPtr& current_card, AnswerMode _answer_mode)
{
   system("CLS");
   current_card->printId();
   if (_answer_mode == AnswerMode::QuestionFirst) {
      current_card->printQuestion();
      static_cast<void>(_getch()); // may prduce a warning, should be okay :D
      current_card->printAnswer();
   }
   else {
      current_card->printAnswer();
      static_cast<void>(_getch()); // may prduce a warning, should be okay :D
      current_card->printQuestion();
   }
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
   Deck hold_deck{};
   CardPtr current_card{};
   AnswerMode answer_mode{ AnswerMode::QuestionFirst };

   bool done{ false };
   while (!done) {
      auto key = _getch();
      char cmd = static_cast<char>(key);
      cmd = std::toupper(cmd);
      cout << "CMD: " << cmd << endl;
      switch (cmd) {
      case 'R':
      {
         play_deck = master_deck;
         cout << "Play deck has been reset to master deck" << endl;
      }
      break;
      case 'T':
      {
         answer_mode = (answer_mode == AnswerMode::QuestionFirst) ? AnswerMode::AnswerFirst : AnswerMode::QuestionFirst;
         cout << "The answer_mode is: " << ((answer_mode == AnswerMode::QuestionFirst) ? "Question First Mode" : "Answer First Mode") << endl;
      }
      break;
      case 'L':
      {
         cout << "Enter the key (ex: AAA.001) [type carefully] of the card you would like to search for: ";
         string response{};
         getline(cin, response);
         auto find_iter = master_deck.find(response);
         if (find_iter == master_deck.end()) {
            cout << "Could not find card" << endl;
         }
         else {
            current_card = find_iter->second;
            displayCard(current_card, answer_mode);
         }
      }
      break;
      case 'S':
         if (current_card != nullptr) {
            hold_deck.insert({ current_card->id, current_card });
            cout << "Card: " << current_card->id << " has been added to the hold deck. Hold Deck size: " << hold_deck.size() << endl;
         }
         else {
            cout << "No current card selected." << endl;
         }
         break;
      case 'X':
         if (!hold_deck.empty()) {
            play_deck = std::move(hold_deck);
            hold_deck.clear();
            cout << "Hold Deck is now empty the Play Deck has: " << play_deck.size();
         }
         else {
            cout << "Hold Deck empty" << endl;
         }
         break;
      case 'N':
         //Advance to next card and print current card
         if (drawNextCard(play_deck, current_card)) {
            displayCard(current_card, answer_mode);
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