# Lab 1 - ShapeMaker

ShapeMaker draws text-art shapes using a configurable canvas size and drawing
symbol. The class implementation and its test driver are in `shapemaker.cpp`;
the class declaration is in `shapemaker.h`.

## Open in Codespaces

Open the `DSA-lab` repository in GitHub Codespaces, then run:

```bash
cd lab1
cmake -S . -B build
cmake --build build
./build/ShapeMaker
```

On Windows with a Visual Studio generator, run:

```powershell
cd lab1
cmake -S . -B build
cmake --build build --config Release
.\build\Release\ShapeMaker.exe
```

## Class responsibilities

The `ShapeMaker` class supports:

- Default initialization to a 21 by 21 canvas using `*`.
- Custom initialization and getters/setters for canvas width, height, and symbol.
- Horizontal and vertical lines.
- Filled and open squares.
- A proportional block-style smiley face.
- An open circle as the bonus operation.

The `main` function calls every member function so it can be used as a simple
test driver while developing the lab.
