# Lab 3 - Randomly Unique Integers (Unsorted List ADT)

This lab uses the Chapter 3 textbook code for the Unsorted List ADT. I looked at both the array-based and the linked-list versions, then compiled and fixed the array-based one and wrote a new main program (`randomlyUnique.cpp`) that fills the list with 50 different random integers.

## Files

| File | What it is |
| --- | --- |
| `ItemType.h` / `ItemType.cpp` | The item stored in the list (just an `int` with `ComparedTo`, `Print` and `Initialize`). `MAX_ITEMS` lives here. |
| `unsorted.h` / `unsorted.cpp` | The array-based `UnsortedType` class from the textbook |
| `listDriver.cpp` | The textbook test driver. Reads commands from a file and writes the results to an output file |
| `listData` | The command file the driver uses |
| `randomlyUnique.cpp` | My new main program for step 4 |
| `Array/` | The array-based code exactly as it was given to us (untouched) |
| `Linked/` | The linked-list code exactly as it was given to us (untouched) |

The files at the top level are my working copy of the array version, after the fixes from step 3. I kept the originals in `Array/` and `Linked/` so it is easy to see what changed.

## How the code is put together (steps 1 and 2)

`UnsortedType` is the list class. It keeps an array `info[MAX_ITEMS]`, a `length`, and a `currentPos` for iterating. The operations are the ones from the book: `MakeEmpty`, `IsFull`, `GetLength`, `GetItem`, `PutItem`, `DeleteItem`, `ResetList` and `GetNextItem`. The list does not know what it is storing, it only calls `ComparedTo` on the items, which is why `ItemType` has to be provided separately.

In the array version `PutItem` just drops the item in slot `length` and `DeleteItem` moves the last item into the hole, so the order is not kept. `GetItem` is a plain linear search.

The linked-list version has exactly the same public interface but stores the items in `NodeType` nodes. `PutItem` inserts at the front, `DeleteItem` unlinks a node, and `IsFull` tries a `new` to see if there is memory left. The point of the lab is that `listDriver.cpp` works with either one without changing.

## Compiling and fixing the array version (step 3)

I compiled with

```
g++ -Wall -Wextra listDriver.cpp unsorted.cpp ItemType.cpp -o listDriver
```

Things I found:

1. **`PrintList.cpp` does not compile.** It has no `#include "unsorted.h"` and no `using namespace std`, so `ofstream` and `UnsortedType` are unknown. But `listDriver.cpp` already has its own `PrintList` at the bottom, so even after fixing the includes it would be a duplicate definition. I left `PrintList.cpp` out of the build (it is not needed).
2. **`exit(2)` without `<cstdlib>`.** g++ lets it through because `<iostream>` pulls it in, but that is not guaranteed on other compilers, so I added `#include <cstdlib>` to `listDriver.cpp`.
3. **`MAX_ITEMS` was 5.** That is fine for `listData` but the new program needs to hold 50 numbers, so I changed it to 50 in `ItemType.h`. This does change one line of the driver's output: after 5 `PutItem`s the driver now says `List is not full.` instead of `List is full.`
4. Not part of step 3, but while reading the linked-list driver (`Linked/listDriver.cpp`) I noticed `exit(2)` is missing its semicolon, so that one will not compile as given either. I tried it with the semicolon added and it runs `listData` fine. Its output is a bit different from the array version: `PrintList` shows `9675` instead of `5769` because the linked `PutItem` inserts at the front, and `IsFull` never says full because it only checks whether `new` still works.

Running the driver with `listData` gives the expected results (see the screenshots in the Word document).

## randomlyUnique.cpp (step 4)

The main function creates an `UnsortedType`, seeds `rand()` with the time, and loops until `GetLength()` is 50. Each time round it makes a random number with `rand() % 100`, puts it in an `ItemType` and calls `GetItem` to see if it is already in the list. Only if `found` is false does it call `PutItem`. I also check `IsFull()` in the loop condition because `PutItem` has "list is not full" as a precondition.

At the end it uses `ResetList` / `GetNextItem` to walk through the list and prints the numbers with `setw(4)`, starting a new line after every 10th one.

I also print how many random numbers it took, just because it was interesting - it is usually somewhere around 65 to 90 because the more numbers are in the list the more often a random one is a repeat.

One thing I noticed while testing: if you run the program twice within the same second you get the same 50 numbers, because `srand(time(0))` gets the same seed. Runs a second apart are different.

## Build and run

```
g++ listDriver.cpp unsorted.cpp ItemType.cpp -o listDriver
g++ randomlyUnique.cpp unsorted.cpp ItemType.cpp -o randomlyUnique
./listDriver        (then type: listData, an output file name, and a label)
./randomlyUnique
```

There is also a `CMakeLists.txt` that builds both programs.
