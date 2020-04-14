/* SPDX-License-Identifier: MIT */
/*
 * CCC 2016 CCOQR 1 - Stupendous Bowties
 * Copyright (c) 2017 Emmanuel Mathi-Amorim
 */

#include <algorithm>
#include <iostream>
 
using namespace std;
typedef long long int64;
 
//---------------------------------------------------------------------------
// Input
//---------------------------------------------------------------------------
 
struct Point
{
    int x, y;
};
 
int numberOfPoints;
Point *points;
 
void readInput()
{
    cin >> numberOfPoints;
    points = new Point[numberOfPoints];
    for (int i = 0; i < numberOfPoints; i++)
    {
        cin >> points[i].x >> points[i].y;
    }
}
 
//---------------------------------------------------------------------------
// Sort
//---------------------------------------------------------------------------
 
int *XSortedIndex;
int *YSortedIndex;
 
bool compareX(int a, int b)
{
    if (points[a].x == points[b].x)
    {
        return points[a].y < points[b].y;
    }
    else
    {
        return points[a].x < points[b].x;
    }
}
 
bool compareY(int a, int b)
{
    if (points[a].y == points[b].y)
    {
        return points[a].x < points[b].x;
    }
    else
    {
        return points[a].y < points[b].y;
    }
}
 
void sortPoints()
{
    XSortedIndex = new int[numberOfPoints];
    YSortedIndex = new int[numberOfPoints];
    for (int i = 0; i < numberOfPoints; i++)
    {
        XSortedIndex[i] = i;
        YSortedIndex[i] = i;
    }
    sort(XSortedIndex, XSortedIndex + numberOfPoints, compareX);
    sort(YSortedIndex, YSortedIndex + numberOfPoints, compareY);
}
 
//---------------------------------------------------------------------------
// Invert Y Index
//---------------------------------------------------------------------------
 
int *InvertedYSortedIndex;
 
void invertYIndex()
{
    InvertedYSortedIndex = new int[numberOfPoints];
    for (int i = 0; i < numberOfPoints; i++)
    {
        InvertedYSortedIndex[YSortedIndex[i]] = i;
    }
}
 
//---------------------------------------------------------------------------
// Pre Solve Y Index Sort
//---------------------------------------------------------------------------
 
int *YSortedIndexLeft;
int *YSortedIndexRight;
 
void preSolveYSortedIndexLeft()
{
    int lastY = points[YSortedIndex[0]].y;
    int firstIndex = 0;
    for (int i = 0; i < numberOfPoints; i++)
    {
        if (points[YSortedIndex[i]].y != lastY)
        {
            lastY = points[YSortedIndex[i]].y;
            firstIndex = i;
            YSortedIndexLeft[i] = 0;
        }
        else
        {
            YSortedIndexLeft[i] = i - firstIndex;
        }
    }
}
 
void preSolveYSortedIndexRight()
{
    int lastY = points[YSortedIndex[0]].y;
    int firstIndex = 0;
    for (int i = numberOfPoints - 1; i >= 0; i--)
    {
        if (points[YSortedIndex[i]].y != lastY)
        {
            lastY = points[YSortedIndex[i]].y;
            firstIndex = i;
            YSortedIndexRight[i] = 0;
        }
        else
        {
            YSortedIndexRight[i] = firstIndex - i;
        }
    }
}
 
void preSolveYSortedIndex()
{
    YSortedIndexLeft = new int[numberOfPoints];
    YSortedIndexRight = new int[numberOfPoints];
    preSolveYSortedIndexLeft();
    preSolveYSortedIndexRight();
}
 
//---------------------------------------------------------------------------
// CleanUp
//---------------------------------------------------------------------------
 
void cleanUp()
{
    delete points;
    delete XSortedIndex;
    delete YSortedIndex;
    delete InvertedYSortedIndex;
    delete YSortedIndexLeft;
    delete YSortedIndexRight;
}
 
//---------------------------------------------------------------------------
// Solvers
//---------------------------------------------------------------------------
 
int64 solveBowties(int index, int64 pointsAbove, int64 pointsBelow)
{
    int yIndex = InvertedYSortedIndex[XSortedIndex[index]];
    int64 pointsLeft = YSortedIndexLeft[yIndex];
    int64 pointsRight = YSortedIndexRight[yIndex];
    return pointsAbove * pointsBelow * pointsLeft * pointsRight * 2;
}
 
int64 solveAllPointsBetween(int start, int end)
{
    int64 bowties = 0;
    for (int i = start; i < end; i++)
    {
        int64 pointsAbove = (i - start) + 1;
        int64 pointsBelow = (end - i);
        bowties += solveBowties(i, pointsAbove, pointsBelow);
    }
    return bowties;
}
 
int64 solve()
{
    int64 bowties = 0;
    int lastX = points[XSortedIndex[0]].x;
    int firstIndex = 0;
    for (int i = 0; i < numberOfPoints; i++)
    {
        if (points[XSortedIndex[i]].x != lastX)
        {
            if ((i - firstIndex) >= 3)
            {
                bowties += solveAllPointsBetween(firstIndex + 1, i - 1);
            }
            firstIndex = i;
            lastX = points[XSortedIndex[i]].x;
        }
    }
    return bowties;
}
 
//---------------------------------------------------------------------------
// Main
//---------------------------------------------------------------------------
 
int main()
{
    readInput();
    sortPoints();
    invertYIndex();
    preSolveYSortedIndex();
    cout << solve();
    cleanUp();
    return 0;
}