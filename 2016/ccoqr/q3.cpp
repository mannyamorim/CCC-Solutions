/* SPDX-License-Identifier: MIT */
/*
 * CCC 2016 CCOQR 3 - Data Structure
 * Copyright (c) 2017 Emmanuel Mathi-Amorim
 */

#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
 
//---------------------------------------------------------------------------
// Input
//---------------------------------------------------------------------------
 
typedef struct {
    long row;
    long col;
} point;
 
long totalRows;
long totalPoints;
 
std::vector<point> points;
 
void read_input()
{
    std::cin >> totalRows >> totalPoints;
    points.reserve(totalPoints);
    for (int i=0;i<totalPoints;i++)
    {
        long row,col;
        std::cin >> row >> col;
 
        // Adjust rows and columns
        point p;
        p.row = totalRows - row + 1;
        p.col = col-1;
 
        // Save in vector
        points.push_back(p);
    }
}
 
 
 
//---------------------------------------------------------------------------
// Sorting
//---------------------------------------------------------------------------
  
bool compare_points(point a,point b)
{
    return (a.col<b.col);
}
void sort_points()
{
    // Sort Points
    std::sort(points.begin(),points.end(),compare_points);
}
  
 
//---------------------------------------------------------------------------
// Point Methods
//---------------------------------------------------------------------------
  
int curPoint = 0;
bool is_there_point_at_this_column(long column)
{
    if (curPoint >= totalPoints)
        return false;
 
    if (points[curPoint].col == column)
        return true;
    else
        return false;
}
 
long get_maximum_rows_at_column(long column)
{
    long maxRows = 0;
    while (is_there_point_at_this_column(column) == true)
    {
        if (points[curPoint].row > maxRows)
            maxRows = points[curPoint].row;
 
        curPoint++;
    }
    return maxRows;
}
 
 
  
//---------------------------------------------------------------------------
// Point Methods
//---------------------------------------------------------------------------
void solve()
{
    long long totalSum = 0;
    long prevRow = 0;
 
    // Process the columns
    for (long col=0;col<totalRows;col++)
    {
        // Is there a point on this column
        if (is_there_point_at_this_column(col))
        {
            // Get the maximum rows on this column (there might be multiple points)
            long rows = get_maximum_rows_at_column(col);
 
            // Only if we are higher than previous rows do we store this value
            if (rows > prevRow)
                prevRow = rows;
        }
 
        // Add the row count to the sum
        totalSum += prevRow;
 
        // Reduct the row count by one until we are at zero
        if (prevRow > 0)
            prevRow--;
    }
 
    // Write Result
    std::cout << totalSum;
}
 
int main(int argc, char* argv[])
{
    read_input();
    sort_points();
    solve();
    return 0;
}