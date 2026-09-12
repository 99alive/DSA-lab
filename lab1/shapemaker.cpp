// shapemaker.cpp
// Lab 1 - ShapeMaker class
// Implementation file for the ShapeMaker class plus a main function
// that acts as a test driver and calls every member function.

#include <iostream>
#include <cmath>
#include "shapemaker.h"

using namespace std;

// ---------------- Initialize ----------------

void ShapeMaker::Initialize()
{
    canvasWidth = 21;
    canvasHeight = 21;
    drawingSymbol = '*';
}

void ShapeMaker::Initialize(int width, int height, char symbol)
{
    canvasWidth = width;
    canvasHeight = height;
    drawingSymbol = symbol;
}

// ---------------- Getters ----------------

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

// ---------------- Setters ----------------

void ShapeMaker::SetCanvasWidth(int width)
{
    canvasWidth = width;
}

void ShapeMaker::SetCanvasHeight(int height)
{
    canvasHeight = height;
}

void ShapeMaker::SetDrawingSymbol(char symbol)
{
    drawingSymbol = symbol;
}

// ---------------- Drawing functions ----------------

// Draws a horizontal line across the middle row of the canvas.
// The rows above and below the line are left blank.
void ShapeMaker::DrawHorizontalLine() const
{
    int middleRow = canvasHeight / 2;

    for (int row = 0; row < canvasHeight; row++)
    {
        if (row == middleRow)
        {
            for (int col = 0; col < canvasWidth; col++)
            {
                cout << drawingSymbol;
            }
        }
        cout << endl;
    }
}

// Draws a vertical line down the middle column of the canvas.
void ShapeMaker::DrawVerticalLine() const
{
    int middleCol = canvasWidth / 2;

    for (int row = 0; row < canvasHeight; row++)
    {
        // print spaces up to the middle, then the symbol
        for (int col = 0; col < middleCol; col++)
        {
            cout << ' ';
        }
        cout << drawingSymbol << endl;
    }
}

// Draws a solid square that is canvasWidth wide and canvasWidth tall.
void ShapeMaker::DrawFilledSquare() const
{
    for (int row = 0; row < canvasWidth; row++)
    {
        for (int col = 0; col < canvasWidth; col++)
        {
            cout << drawingSymbol;
        }
        cout << endl;
    }
}

// Draws just the outline of a square that is canvasWidth wide.
void ShapeMaker::DrawOpenSquare() const
{
    for (int row = 0; row < canvasWidth; row++)
    {
        for (int col = 0; col < canvasWidth; col++)
        {
            // only print the symbol on the edges
            if (row == 0 || row == canvasWidth - 1 ||
                col == 0 || col == canvasWidth - 1)
            {
                cout << drawingSymbol;
            }
            else
            {
                cout << ' ';
            }
        }
        cout << endl;
    }
}

// Draws a smiley face inside a square "block head".
// The eyes are about 1/3 of the way down, the nose is in the
// center and the mouth is about 2/3 of the way down, so the face
// still looks right when the canvas size changes.
void ShapeMaker::DrawSmileyFace() const
{
    int size = canvasWidth;
    int eyeRow = size / 3;
    int leftEyeCol = size / 3;
    int rightEyeCol = size - size / 3 - 1;
    int noseRow = size / 2;
    int noseCol = size / 2;
    int mouthRow = (size * 2) / 3;

    for (int row = 0; row < size; row++)
    {
        for (int col = 0; col < size; col++)
        {
            bool onBorder = (row == 0 || row == size - 1 ||
                             col == 0 || col == size - 1);
            bool onEyes = (row == eyeRow &&
                           (col == leftEyeCol || col == rightEyeCol));
            bool onNose = (row == noseRow && col == noseCol);
            bool onMouth = (row == mouthRow &&
                            col >= leftEyeCol && col <= rightEyeCol);

            if (onBorder || onEyes || onNose || onMouth)
            {
                cout << drawingSymbol;
            }
            else
            {
                cout << ' ';
            }
        }
        cout << endl;
    }
}

// BONUS: Draws the outline of a circle as wide as the canvas.
// For every position I find its distance from the center using the
// distance formula. If that distance is close to the radius the
// position is on the edge of the circle so I print the symbol.
void ShapeMaker::DrawOpenCircle() const
{
    int size = canvasWidth;
    double center = (size - 1) / 2.0;
    double radius = center;

    for (int row = 0; row < size; row++)
    {
        for (int col = 0; col < size; col++)
        {
            double dx = col - center;
            double dy = row - center;
            double distance = sqrt(dx * dx + dy * dy);

            if (fabs(distance - radius) < 0.65)
            {
                cout << drawingSymbol;
            }
            else
            {
                cout << ' ';
            }
        }
        cout << endl;
    }
}

// ---------------- Test driver ----------------

int main()
{
    ShapeMaker shape;

    // start with the default canvas (21 x 21 and '*')
    shape.Initialize();

    cout << "Canvas width: " << shape.GetCanvasWidth() << endl;
    cout << "Canvas height: " << shape.GetCanvasHeight() << endl;
    cout << "Drawing symbol: " << shape.GetDrawingSymbol() << endl;
    cout << endl;

    cout << "Horizontal line:" << endl;
    shape.DrawHorizontalLine();
    cout << endl;

    cout << "Vertical line:" << endl;
    shape.DrawVerticalLine();
    cout << endl;

    cout << "Filled square:" << endl;
    shape.DrawFilledSquare();
    cout << endl;

    cout << "Open square:" << endl;
    shape.DrawOpenSquare();
    cout << endl;

    cout << "Smiley face:" << endl;
    shape.DrawSmileyFace();
    cout << endl;

    cout << "Open circle (bonus):" << endl;
    shape.DrawOpenCircle();
    cout << endl;

    // now try a different canvas using the other Initialize
    shape.Initialize(11, 11, '#');
    cout << "After Initialize(11, 11, '#'):" << endl;
    cout << "Canvas width: " << shape.GetCanvasWidth() << endl;
    cout << "Canvas height: " << shape.GetCanvasHeight() << endl;
    cout << "Drawing symbol: " << shape.GetDrawingSymbol() << endl;
    cout << endl;

    cout << "Smiley face:" << endl;
    shape.DrawSmileyFace();
    cout << endl;

    // and change things one at a time with the setters
    shape.SetCanvasWidth(9);
    shape.SetCanvasHeight(5);
    shape.SetDrawingSymbol('+');
    cout << "After setters (9 x 5 and '+'):" << endl;
    cout << "Canvas width: " << shape.GetCanvasWidth() << endl;
    cout << "Canvas height: " << shape.GetCanvasHeight() << endl;
    cout << "Drawing symbol: " << shape.GetDrawingSymbol() << endl;
    cout << endl;

    cout << "Horizontal line:" << endl;
    shape.DrawHorizontalLine();
    cout << endl;

    cout << "Open square:" << endl;
    shape.DrawOpenSquare();
    cout << endl;

    return 0;
}
