// The following declarations and definitions go into file 
// ItemType.h. 

#include <fstream>
const int MAX_ITEMS = 50;   // was 5, raised so the list can hold the 50 random numbers
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
 
