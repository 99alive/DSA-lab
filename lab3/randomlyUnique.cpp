// randomlyUnique.cpp
// Lab 3 - Randomly Unique Integers
//
// Makes an Unsorted List, then keeps generating random integers from
// 0 to 99 and adds each one to the list only if it is not already in
// there, until the list has 50 unique integers. At the end it prints
// the list in rows of 10.

#include <iostream>
#include <iomanip>    // setw
#include <cstdlib>    // rand, srand
#include <ctime>      // time
#include "unsorted.h"

using namespace std;

const int HOW_MANY = 50;     // how many unique numbers we want
const int RANGE = 100;       // numbers will be 0 to 99
const int PER_ROW = 10;      // how many numbers to print on each line

int main()
{
  UnsortedType list;
  ItemType item;
  bool found;
  int number;
  int tries = 0;             // how many random numbers we had to generate

  srand(time(0));            // seed so we get different numbers each run

  // keep going until the list has 50 numbers in it
  while (list.GetLength() < HOW_MANY && !list.IsFull())
  {
    number = rand() % RANGE;           // random number from 0 to 99
    tries++;
    item.Initialize(number);

    // GetItem searches the list for us and sets found
    list.GetItem(item, found);

    if (!found)                        // only add it if it is new
      list.PutItem(item);
  }

  cout << HOW_MANY << " randomly unique integers between 0 and "
       << RANGE - 1 << ":" << endl;
  cout << "(it took " << tries << " random numbers to get "
       << list.GetLength() << " unique ones)" << endl << endl;

  // print the list 10 per row using the list's own iterator
  list.ResetList();
  for (int count = 1; count <= list.GetLength(); count++)
  {
    item = list.GetNextItem();
    cout << setw(4);                   // line the columns up
    item.Print(cout);

    if (count % PER_ROW == 0)          // end the row after every 10th number
      cout << endl;
  }
  cout << endl;

  return 0;
}
