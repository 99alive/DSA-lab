// randomlyUnique.cpp
// Lab 3 - Randomly Unique Integers
// Fills an Unsorted List with 50 different random integers between
// 0 and 99, then prints them out 10 per row.

#include <iostream>
#include <cstdlib>     // rand and srand
#include <ctime>       // time
#include "unsorted.h"

using namespace std;

int main()
{
  UnsortedType list;
  ItemType item;
  bool found;
  int number;

  srand(time(0));      // so the numbers are different every run

  // keep adding random numbers until the list has 50 in it
  while (list.GetLength() < 50)
  {
    number = rand() % 100;         // random number from 0 to 99
    item.Initialize(number);

    list.GetItem(item, found);     // is it already in the list?

    if (!found)
      list.PutItem(item);          // only add it if it is new
  }

  // print the list 10 numbers per row
  cout << "50 randomly unique integers between 0 and 99:" << endl;

  list.ResetList();
  for (int i = 1; i <= list.GetLength(); i++)
  {
    item = list.GetNextItem();
    item.Print(cout);
    cout << "\t";

    if (i % 10 == 0)               // start a new row after every 10th number
      cout << endl;
  }

  return 0;
}
