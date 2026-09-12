# Lab 1 - ShapeMaker

For this lab I designed a ShapeMaker class with a UML diagram and then wrote it in C++. The class draws shapes on the screen using text characters, kind of like ASCII art.

## Files

- `shapemaker.h` - the specification file with the class declaration
- `shapemaker.cpp` - the implementation of the class plus a `main` function that tests everything
- `CMakeLists.txt` - so the lab builds the same way as lab 0

## How to build and run

```bash
cd lab1
cmake -S . -B build
cmake --build build
./build/ShapeMaker
```

Or just compile it straight with g++:

```bash
g++ shapemaker.cpp -o shapemaker
./shapemaker
```

## What the class does

The class keeps track of three things: the canvas width, the canvas height, and the symbol used for drawing. The defaults are 21 x 21 and `*`.

Member functions (from the CRC card):

- `Initialize()` - sets the defaults
- `Initialize(width, height, symbol)` - sets custom values
- `GetCanvasWidth()`, `GetCanvasHeight()`, `GetDrawingSymbol()`
- `SetCanvasWidth()`, `SetCanvasHeight()`, `SetDrawingSymbol()`
- `DrawHorizontalLine()` - a line across the middle row of the canvas
- `DrawVerticalLine()` - a line down the middle column of the canvas
- `DrawFilledSquare()` - a solid square as wide as the canvas
- `DrawOpenSquare()` - just the outline of the square
- `DrawSmileyFace()` - a face inside a square head, with the eyes, nose and mouth placed at 1/3, 1/2 and 2/3 of the way down so it still looks right at different sizes
- `DrawOpenCircle()` - the bonus one. I used the distance formula from the center of the canvas and printed the symbol wherever the distance was close to the radius.

## Testing

The `main` function in `shapemaker.cpp` is the test driver. It creates one ShapeMaker object, calls `Initialize()`, prints the getters, draws all the shapes, then calls the other `Initialize` and each setter and draws a few shapes again to show the size and symbol actually change.
