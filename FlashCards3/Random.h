#pragma once
#include <random>

class Random final
{
public:
   static Random& getInstance() {
      static Random instance;
      return instance;
   };

   int draw(int max, int min = 0) {
      std::uniform_int_distribution<int> distrib(min, max);
      return distrib(generator);
   }
   void reseedGenerator(int new_seed) {
      generator.seed(new_seed);
   }
private:
   std::mt19937 generator{ 123 }; // '123' is the 'default' seed. But can be reseed.

   Random() {};
   ~Random() = default;

   Random(const Random&) = delete;
   Random& operator=(const Random&) = delete;
   Random(Random&&) = delete;
   Random& operator=(Random&&) = delete;
};

