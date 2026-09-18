# Lab 4 - Memory Matching Card Game (Unsorted Linked List)

A memory / concentration game. 16 cards (A to H, two of each) are shuffled and laid out in a 4x4 grid. You flip two cards at a time. If they match they stay matched, if not they flip back. The game ends when every pair is matched.

The cards are stored in a singly linked list that I wrote myself (`LinkedList` in `main.cpp`). The list is unsorted, and the game treats it like a grid by using `index = row * cols + col`.

There are three ways to play, all in the same program:

| Mode | Part | How it works |
| --- | --- | --- |
| `terminal` | Part 1 | Plain `cout`. Type the row and column of the card. |
| `ncurses` | Part 2 | Arrow keys move a cursor, SPACE flips, Q quits. Colours if the terminal supports them. |
| `sfml` | Part 1 & 2 | Opens a 400x400 window. Click a card to flip it. |

## Files

- `main.cpp` - everything: `Card`, `Node`, `LinkedList`, `MemoryGame` (with the three display modes) and `main`
- `CMakeLists.txt` - builds it, and downloads SFML so it does not have to be installed
- `arial.ttf` is **not** included. The SFML mode needs a TTF font in the same folder as the program. On Windows copy `C:\Windows\Fonts\arial.ttf` next to `game.exe`; on Ubuntu `sudo apt install fonts-liberation` and copy `/usr/share/fonts/truetype/liberation/LiberationSans-Regular.ttf` as `arial.ttf`. Without it the SFML cards still show, just without letters.

## How to build

### Ubuntu / Codespaces (the way the lab describes)

```
sudo apt install libncurses5-dev libsfml-dev
g++ main.cpp -o game -lncurses -lsfml-graphics -lsfml-window -lsfml-system
```

### With CMake (works on Windows with MSYS2 too)

ncurses needs to be installed (`sudo apt install libncurses5-dev` on Ubuntu, `pacman -S mingw-w64-ucrt-x86_64-ncurses` on MSYS2). SFML is downloaded and built by CMake the first time, so that takes a few minutes.

```
cmake -S . -B build
cmake --build build
```

## How to run

**Note for the VS Code terminal on Windows:** ncurses mode quits straight away with
`Error opening terminal: xterm-256color` because MSYS2's ncurses cannot find its
terminfo files when started from PowerShell. Tell it where they are first:

```powershell
$env:TERMINFO = "C:\msys64\ucrt64\share\terminfo"
.\game.exe ncurses
```

On Linux this does not happen.

```
./game            shows a menu to pick the mode
./game terminal
./game ncurses
./game sfml
```

## How the code is organised

- **`Card`** (Step 1) - a `char` value plus `isFaceUp` and `isMatched` flags.
- **`Node` / `LinkedList`** (Step 2) - a singly linked list with `insert` (adds at the end), `getAt(index)`, `removeAt(index)`, `size()` and `traverse()`. `getAt` and `removeAt` walk from the head every time, so they are O(n).
- **`MemoryGame`** (Step 3) - makes the 8 pairs, shuffles them, and has `flipCard`, `checkMatch` and `allMatched`. Matched cards are marked, not removed from the list, so the grid positions never move.
- **Display** (Step 4) - `displayTerminal` / `playTerminal` for cout, `displayTerminalNcurses` for ncurses, `displaySFML` for the window.
- **`main`** (Step 5) - picks the mode from the command line argument, or shows a little menu.

## Report: challenges and what I learned

### Why a linked list is a bad fit for a grid game

Every time the game needs a card it calls `cards.getAt(index)`, and `getAt` has to start at the head and follow `next` pointers `index` times. That is O(n) for one card. Drawing the grid calls `getAt` 16 times, so drawing is O(n^2) - 16 * 16 = 256 pointer hops just to show the board, every single redraw. With a vector or an array, `cards[index]` is O(1) and drawing would be 16 steps.

For 16 cards nobody can tell the difference, but the point is clear: a linked list is good when you insert and delete in the middle a lot, and bad when you need to jump to position *i*. A memory game never inserts or deletes during play (I mark matched cards instead of removing them), it only does random access, so an array is the natural choice and the linked list is only here because the lab is about linked lists.

### Shuffling

You cannot use `std::shuffle` on a linked list because it needs random access iterators. I did what the lab suggested: copy all the cards into a `std::vector`, shuffle that, `removeAt(0)` until the list is empty, then `insert` the cards back in the new order. The `removeAt(0)` loop is another place where the list is awkward - there is no `clear()`.

### Bug in the sample flow: the second card never showed

In the sample code the terminal loop was `if (flipCard(row, col)) checkMatch();` and then it redrew the grid at the top of the loop. So when the two cards did **not** match, `checkMatch` flipped them back down before the grid was ever drawn - you could never see the second card, which makes the game impossible to play. The ncurses and SFML versions had the same order (check first, then draw / delay).

I fixed it the same way in all three modes: after the second flip, **draw the grid first**, wait a moment (ncurses: `napms(1000)`, SFML: `sf::sleep`), and only then call `checkMatch()`. In the plain terminal mode it just prints the grid again with both cards up and then says "Match!" or "No match".

### Terminal input

In terminal mode `cin >> row >> col` will loop forever if someone types a letter, because `cin` goes into a fail state. I check the result of the `>>` and quit cleanly if the input is bad. Rows and columns outside 0-3 are ignored by `flipCard`.

### ncurses vs plain cout (Part 2)

Pros of ncurses: the screen is redrawn in place instead of scrolling, `getch()` gives a key immediately without pressing enter, arrow keys work, and you get colours and a highlighted cursor. It feels like a real game instead of a form to fill in.

Cons: more setup (`initscr`, `noecho`, `keypad`, `curs_set`, colour pairs), you must call `endwin()` on every exit path or the terminal is left in a broken state, and it is another library that has to be installed and linked. It also does not run inside some IDE output panes, only in a real terminal.

### SFML

The window mode was mostly the sample code. Things I had to handle: the mouse click has to be ignored while a pair is waiting to be checked (otherwise a third click during the one second pause could flip a third card), and the font has to be loaded from a file, so the program checks whether `arial.ttf` loaded and still works without it.

### Memory

The `LinkedList` destructor walks the list and deletes every node, so the 16 nodes are freed when the game ends. I could not run valgrind on Windows, but the destructor is the same as the sample and nothing else in the program uses `new`.
