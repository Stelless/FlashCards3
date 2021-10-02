#include "Card.h"
#define dbgln(x) std::cout << #x << ": " << (x) << std::endl;

void const Card::printId() {
   std::cout << "ID: " + id + "\n";
}
void const Card::printQuestion() {
   std::cout << "Question: ";
   for (const auto& ques : question) {
      std::cout << ques + ' ';
   }
   std::cout << "\n";
}
void const Card::printAnswer() {
   std::cout << "Answer: ";
   for (const auto& ans : answer) {
      std::cout << ans + ' ';
   }
   std::cout << "\n";
}
void const Card::printCard() {

   std::cout << "\n--------------------------------------------------------------------------\n";
   printId();
   printQuestion();
   printAnswer();
}
Card Card::loadCard(pt::ptree::value_type& card) {
   //maybe turn this into an overloaded constructor so we can just create the cards instead
   // of making them and then loading them.
   Card new_card{ }; // tmp card to load up
   new_card.id = card.second.get<std::string>("id", "<no id>");

   for (pt::ptree::value_type& question_elem : card.second.get_child("question"))
   {
      // FOR loop goes through each string in the array of strings (need to push each one on the vector)
      const auto& question_line{ question_elem.second.data() };
      new_card.question.push_back(question_line);
   }

   // Print out the lines of the answer (for each string variable in the JSON)
   for (pt::ptree::value_type& answer_elem : card.second.get_child("answer"))
   {
      // FOR loop goes through each string in the array of strings (need to push each one on the vector)
      const auto& answer_line{ answer_elem.second.data() };
      new_card.answer.push_back(answer_line);
   }
   return new_card;
}