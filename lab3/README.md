# Lab 3 - Randomly Unique Integers

This lab uses the Chapter 3 textbook code for the Unsorted List ADT. There are two versions of it, one that uses an array and one that uses a linked list. The lab was to read both, compile and fix the array one, and then write a new main program called `randomlyUnique.cpp` that fills the list with 50 different random numbers.

## Folders

- `Array/` - the array-based version. This is the one I compiled, fixed and used for step 4. `randomlyUnique.cpp` is in here.
- `Linked/` - the linked-list version, exactly as it was given to us. I only read through this one for step 2.

## What the code does (steps 1 and 2)

`UnsortedType` is the list class. It has an array `info[MAX_ITEMS]`, a `length`, and a `currentPos` that is used for going through the list. The operations are `MakeEmpty`, `IsFull`, `GetLength`, `GetItem`, `PutItem`, `DeleteItem`, `ResetList` and `GetNextItem`. The list does not know what it is storing. It only calls `ComparedTo` on the items, which is why `ItemType` is in its own file.

In the array version `PutItem` puts the item in slot `length` and `DeleteItem` moves the last item into the hole, so the list is not in any order. `GetItem` just searches from the start.

The linked-list version has the same functions but keeps the items in `NodeType` nodes. `PutItem` puts the new node at the front and `DeleteItem` unlinks a node. `IsFull` tries to `new` a node to see if there is any memory left. `listDriver.cpp` is the same for both, which shows the driver does not care how the list is stored.

## Compile errors I fixed (step 3)

I compiled the array version with

```
g++ -Wall listDriver.cpp unsorted.cpp ItemType.cpp -o listDriver
```

1. `PrintList.cpp` would not compile. It has no `#include "unsorted.h"` and no `using namespace std`, so it does not know what `ofstream` or `UnsortedType` are. Also `listDriver.cpp` already has the same `PrintList` function at the bottom, so this file is not needed at all. I removed it.
2. `listDriver.cpp` calls `exit(2)` but does not include `<cstdlib>`. It happened to work with g++ but I added the include to be safe.
3. `MAX_ITEMS` in `ItemType.h` was 5. That is fine for `listData` but the new program needs to hold 50 numbers, so I changed it to 50. Because of this, the `IsFull` command in `listData` now prints `List is not full.` instead of `List is full.`
4. A run-time error: if the command file does not end with `Quit` (or you give it the wrong file), `inFile >> command` fails at the end of the file, `command` never changes, and the `while` loop runs forever printing `Command not recognized`. I found this by accident when I answered the file name prompts in the wrong order and it wiped `listData`. I changed the loop to `while (command != "Quit" && inFile)` so it stops when the file runs out.

Running `listDriver` with `listData` gives the expected output (see screenshots in the Word document).

I also noticed the linked-list driver is missing a semicolon after `exit(2)`, but that version was not part of step 3 so I left it as given.

## randomlyUnique.cpp (step 4)

It makes an `UnsortedType`, seeds `rand()` with the time, and loops until `GetLength()` is 50. Each time it makes a random number with `rand() % 100`, puts it in an `ItemType` and calls `GetItem` to see if it is already in the list. If `found` is false it calls `PutItem`. When the loop is done it uses `ResetList` and `GetNextItem` to go through the list and print the numbers, with a new line after every 10th one.

One thing I noticed: if you run it twice in the same second you get the same numbers, because `srand(time(0))` gets the same seed. A second later it is different.

## How to build and run

```
cd Array
g++ listDriver.cpp unsorted.cpp ItemType.cpp -o listDriver
g++ randomlyUnique.cpp unsorted.cpp ItemType.cpp -o randomlyUnique
./listDriver          (it asks for: listData, an output file name, and a label)
./randomlyUnique
```
