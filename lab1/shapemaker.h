#ifndef SHAPEMAKER_H
#define SHAPEMAKER_H

#include <string>

class ShapeMaker {
public:
    ShapeMaker();

    void Initialize();
    void Initialize(int canvasWidth, int canvasHeight, char drawingSymbol);

    int GetCanvasWidth() const;
    int GetCanvasHeight() const;
    char GetDrawingSymbol() const;

    void SetCanvasWidth(int canvasWidth);
    void SetCanvasHeight(int canvasHeight);
    void SetDrawingSymbol(char drawingSymbol);

    void DrawHorizontalLine() const;
    void DrawVerticalLine() const;
    void DrawFilledSquare() const;
    void DrawOpenSquare() const;
    void DrawSmileyFace() const;
    void DrawOpenCircle() const;

private:
    int canvasWidth;
    int canvasHeight;
    char drawingSymbol;

    void PrintCanvasRow(const std::string& row) const;
};

#endif
