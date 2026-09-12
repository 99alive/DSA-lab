// shapemaker.h
// Lab 1 - ShapeMaker class
// Specification file for the ShapeMaker class. This class draws
// simple shapes on the screen using text characters.

#ifndef SHAPEMAKER_H
#define SHAPEMAKER_H

class ShapeMaker
{
public:
    // Sets the canvas to the defaults (21 x 21 and '*')
    void Initialize();

    // Sets the canvas to the values passed in
    void Initialize(int width, int height, char symbol);

    // Getters
    int GetCanvasWidth() const;
    int GetCanvasHeight() const;
    char GetDrawingSymbol() const;

    // Setters
    void SetCanvasWidth(int width);
    void SetCanvasHeight(int height);
    void SetDrawingSymbol(char symbol);

    // Drawing functions
    void DrawHorizontalLine() const;   // line across the middle of the canvas
    void DrawVerticalLine() const;     // line down the middle of the canvas
    void DrawFilledSquare() const;     // solid square, canvas width sized
    void DrawOpenSquare() const;       // square outline, canvas width sized
    void DrawSmileyFace() const;       // smiley face inside a square head
    void DrawOpenCircle() const;       // BONUS: circle outline, canvas wide

private:
    int canvasWidth;
    int canvasHeight;
    char drawingSymbol;
};

#endif
