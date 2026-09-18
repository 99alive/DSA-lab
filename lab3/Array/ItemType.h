// The following declarations and definitions go into file 
// ItemType.h. 

#include <fstream>
const int MAX_ITEMS = 50;   // changed from 5 so the list can hold 50 numbers
enum RelationType  {LESS, GREATER, EQUAL};

class ItemType 
{ 
public:
  ItemType();
  RelationType ComparedTo(ItemType) const;
  void Print(std::ostream&) const;
  void Initialize(int number);
private:
  int value;
};
 
