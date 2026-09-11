#include "shapemaker.h"

#include <algorithm>
#include <cmath>
#include <iostream>
#include <stdexcept>

ShapeMaker::ShapeMaker()
{
    Initialize();
}

void ShapeMaker::Initialize()
{
    canvasWidth = 21;
    canvasHeight = 21;
    drawingSymbol = '*';
}

void ShapeMaker::Initialize(int newCanvasWidth, int newCanvasHeight, char newDrawingSymbol)
{
    SetCanvasWidth(newCanvasWidth);
    SetCanvasHeight(newCanvasHeight);
    SetDrawingSymbol(newDrawingSymbol);
}

int ShapeMaker::GetCanvasWidth() const
{
    return canvasWidth;
}

int ShapeMaker::GetCanvasHeight() const
{
    return canvasHeight;
}

char ShapeMaker::GetDrawingSymbol() const
{
    return drawingSymbol;
}

void ShapeMaker::SetCanvasWidth(int newCanvasWidth)
{
    if (newCanvasWidth < 1) {
        throw std::invalid_argument("canvas width must be positive");
    }
    canvasWidth = newCanvasWidth;
}

void ShapeMaker::SetCanvasHeight(int newCanvasHeight)
{
    if (newCanvasHeight < 1) {
        throw std::invalid_argument("canvas height must be positive");
    }
    canvasHeight = newCanvasHeight;
}

void ShapeMaker::SetDrawingSymbol(char newDrawingSymbol)
{
    drawingSymbol = newDrawingSymbol;
}

void ShapeMaker::DrawHorizontalLine() const
{
    std::cout << std::string(canvasWidth, drawingSymbol) << '\n';
}

void ShapeMaker::DrawVerticalLine() const
{
    for (int row = 0; row < canvasHeight; ++row) {
        std::cout << drawingSymbol << '\n';
    }
}

void ShapeMaker::DrawFilledSquare() const
{
    int squareSize = std::min(canvasWidth, canvasHeight);
    std::string row(squareSize, drawingSymbol);
    for (int line = 0; line < squareSize; ++line) {
        std::cout << row << '\n';
    }
}

void ShapeMaker::DrawOpenSquare() const
{
    int squareSize = std::min(canvasWidth, canvasHeight);
    std::string border(squareSize, drawingSymbol);
    std::string middle = drawingSymbol + std::string(squareSize - 2, ' ') + drawingSymbol;

    for (int line = 0; line < squareSize; ++line) {
        if (line == 0 || line == squareSize - 1 || squareSize < 3) {
            std::cout << border << '\n';
        } else {
            std::cout << middle << '\n';
        }
    }
}

void ShapeMaker::DrawSmileyFace() const
{
    int size = std::min(canvasWidth, canvasHeight);
    for (int row = 0; row < size; ++row) {
        for (int column = 0; column < size; ++column) {
            bool border = row == 0 || row == size - 1 || column == 0 || column == size - 1;
            bool leftEye = row == size / 3 && column == size / 3;
            bool rightEye = row == size / 3 && column == size - size / 3 - 1;
            bool nose = row == size / 2 && column == size / 2;
            bool mouth = row == (2 * size) / 3 &&
                         column >= size / 3 && column <= size - size / 3 - 1;

            std::cout << ((border || leftEye || rightEye || nose || mouth) ? drawingSymbol : ' ');
        }
        std::cout << '\n';
    }
}

void ShapeMaker::DrawOpenCircle() const
{
    int size = std::min(canvasWidth, canvasHeight);
    double center = (size - 1) / 2.0;
    double radius = center;

    for (int row = 0; row < size; ++row) {
        for (int column = 0; column < size; ++column) {
            double distance = std::hypot(column - center, row - center);
            std::cout << (std::abs(distance - radius) < 0.65 ? drawingSymbol : ' ');
        }
        std::cout << '\n';
    }
}

int main()
{
    ShapeMaker shapeMaker;

    std::cout << "Horizontal line:\n";
    shapeMaker.DrawHorizontalLine();
    std::cout << "Vertical line:\n";
    shapeMaker.DrawVerticalLine();
    std::cout << "Filled square:\n";
    shapeMaker.DrawFilledSquare();
    std::cout << "Open square:\n";
    shapeMaker.DrawOpenSquare();
    std::cout << "Smiley face:\n";
    shapeMaker.DrawSmileyFace();
    std::cout << "Open circle:\n";
    shapeMaker.DrawOpenCircle();

    shapeMaker.Initialize(9, 9, '#');
    std::cout << "Configured canvas: " << shapeMaker.GetCanvasWidth() << "x"
              << shapeMaker.GetCanvasHeight() << " using "
              << shapeMaker.GetDrawingSymbol() << '\n';
    shapeMaker.SetCanvasWidth(7);
    shapeMaker.SetCanvasHeight(7);
    shapeMaker.SetDrawingSymbol('+');
    shapeMaker.DrawOpenSquare();

    return 0;
}
