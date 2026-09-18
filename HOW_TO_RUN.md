# How to run each lab (GitHub Codespaces / Ubuntu)

Open the repo in a Codespace and use the terminal. Each lab also has its own README with more detail.

## Lab 0 - MathLib (CMake + Catch2)

```bash
cd lab0
cmake -S . -B build && cmake --build build
cd build && ctest --output-on-failure && ./MathApp
```

## Lab 1 - ShapeMaker

```bash
cd lab1
g++ shapemaker.cpp -o shapemaker && ./shapemaker
```

## Lab 2 - Inheritance + JSON / YAML / CSV + Catch2

This one is on its own branch.

```bash
git checkout lab/inheritance-serialization
cd lab2
cmake -S . -B build && cmake --build build     # first time downloads json, yaml-cpp and Catch2 (a few minutes)
cd build && ctest --output-on-failure && ./app
```

## Lab 3 - Randomly Unique Integers (Unsorted List ADT)

```bash
cd lab3/Array
g++ listDriver.cpp unsorted.cpp ItemType.cpp -o listDriver
g++ randomlyUnique.cpp unsorted.cpp ItemType.cpp -o randomlyUnique
./listDriver        # when it asks, type:  listData   then  out.txt   then  ArrayTest
cat out.txt
./randomlyUnique
```

## Lab 4 - Memory Matching Card Game (linked list, terminal / ncurses / SFML)

```bash
cd lab4
sudo apt update && sudo apt install -y libncurses-dev libsfml-dev
g++ main.cpp -o game -lncurses -lsfml-graphics -lsfml-window -lsfml-system
./game terminal     # type row and col, e.g. 0 0
./game ncurses      # arrow keys, SPACE to flip, Q to quit
```

`./game sfml` opens a graphical window, so it only works on a computer with a screen (not in the
Codespaces browser terminal). On Windows it is built in `lab4/build-mingw/game.exe`.
