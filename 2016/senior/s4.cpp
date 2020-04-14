/* SPDX-License-Identifier: MIT */
/*
 * CCC 2016 Senior 4 - Combining Rice Balls
 * Copyright (c) 2017 Emmanuel Mathi-Amorim
 */

#include <iostream>
#include <string>
 
 
//---------------------------------------------------------------------------
// Input
//---------------------------------------------------------------------------
 
int totalBalls;
int riceBalls[400];
 
void read_input()
{
    std::cin >> totalBalls;
    for (int i=0;i<totalBalls;i++)
        std::cin >> riceBalls[i];
}
 
 
//---------------------------------------------------------------------------
// Dynamic Programming
//---------------------------------------------------------------------------
 
int counts[400][400];
 
void init_dp()
{
    for (int start=0;start<totalBalls;start++)
        for (int end=0;end<totalBalls;end++)
            counts[start][end] = -1;
}
  
 
bool can_range_be_combined(int startIndex, int endIndex)
{
    if (startIndex == endIndex)
        return true;
    else
        return counts[startIndex][endIndex] != -1;
}
 
 
int get_range_count(int startIndex, int endIndex)
{
    if (startIndex == endIndex)
        return riceBalls[startIndex];
    else
        return counts[startIndex][endIndex];
}
 
 
void add_solution(int startIndex, int endIndex, int ballCount)
{
    counts[startIndex][endIndex] = ballCount;
}
 
 
  
 
 
//---------------------------------------------------------------------------
// Solvers
//---------------------------------------------------------------------------
 
 
// Looks for two blocks that can be combined and contain equal number of rice_balls
bool search_range_for_two_blocks(int firstStartIndex, int lastEndIndex)
{
    for (int lastStartIndex = firstStartIndex+1; lastStartIndex <= lastEndIndex; lastStartIndex++)
    {
        // Define the first range
        int firstEndIndex = lastStartIndex-1;
 
        // Skip starting ranges that cannot be combined
        if (can_range_be_combined(firstStartIndex,firstEndIndex) == false)
            continue;
 
        // Skip ending ranges that cannot be combined
        if (can_range_be_combined(lastStartIndex,lastEndIndex) == false)
            continue;
 
        // We need to know the first range count
        int firstCount = get_range_count(firstStartIndex,firstEndIndex);
 
        // We need to know the last range count
        int lastCount = get_range_count(lastStartIndex,lastEndIndex);
 
        // We can only combine if the first range count is the same as the last range count
        if (firstCount != lastCount)
            continue;
 
        // Compute the combined number of rice balls
        int rangeCount = firstCount + lastCount;
 
        // We have found a solution - lets add it to the our DP container
        add_solution(firstStartIndex,lastEndIndex,rangeCount);
 
        // Only one solution is important - we don't have to keep looking for another
        return true;
    }
  
    // We did not find a solution
    return false;
} 
     
 
 
// Looks for solutions with a middle block that can be combined
void search_range_for_three_blocks(int firstStartIndex, int lastEndIndex)
{
    for (int middleStartIndex = firstStartIndex+1; middleStartIndex < lastEndIndex; middleStartIndex++)
    {
        // Define the end of the first block
        int firstEndIndex = middleStartIndex-1;
 
        // Skip first range if it cannot be combined
        if (can_range_be_combined(firstStartIndex,firstEndIndex) == false)
            continue;
 
        // We need to know the starting range count
        int firstCount = get_range_count(firstStartIndex,firstEndIndex);
         
        for (int lastStartIndex = middleStartIndex+1; lastStartIndex <= lastEndIndex; lastStartIndex++)
        { 
            // Define the end of them middle block
            int middleEndIndex = lastStartIndex -1;
 
            // Skip middle range that cannot be combined
            if (can_range_be_combined(middleStartIndex,middleEndIndex) == false)
                continue;
 
            // Skip last range that cannot be combined
            if (can_range_be_combined(lastStartIndex,lastEndIndex) == false)
                continue;
 
            // We need to know the last range count
            int lastCount = get_range_count(lastStartIndex,lastEndIndex);
 
            // We can only combine if the first range count is the same as the last range count
            if (firstCount != lastCount)
                continue;
 
            // We need the middle range count so we can compute the full range count
            int middleCount = get_range_count(middleStartIndex,middleEndIndex);
            int rangeCount = firstCount + middleCount + lastCount;
 
            // We have found a solution - lets add it to the our DP container
            add_solution(firstStartIndex,lastEndIndex,rangeCount);
 
            // Only one solution is important - we don't have to keep looking for another
            return;
        } 
    }
}
 
 
// Quick search through all solutions for the one with the largest number of riceBalls
int find_largest_combination()
{
    int max = 0;
 
    for (int start=0;start<totalBalls;start++)
        for (int end=start;end<totalBalls;end++)
            max = std::max( max, get_range_count(start,end) );
 
    return max;
}
 
 
 
void solve()
{
    // Startup
    read_input();
    init_dp();
 
 
    // Solve for ranges of 2 and all the way to the totalBalls
    for (int range=2;range<=totalBalls;range++)
    {
        int maxStart = totalBalls-range+1;
        for (int firstStartIndex=0;firstStartIndex<maxStart;firstStartIndex++)
        {
            int lastEndIndex = firstStartIndex + range - 1;
 
            // Search for two block solutions (first,last)
            bool found = search_range_for_two_blocks(firstStartIndex,lastEndIndex);
 
            // If we found a solution with the simple solver don't do any more work
            if (found) continue;
 
            // Search for solutions with three blocks (first, middle, last) when we have at least 3 balls
            if (range > 2)
                search_range_for_three_blocks(firstStartIndex,lastEndIndex);
        }
    }
 
    // Find largest rice ball
    int largest = find_largest_combination();
 
    // Output Result
    std::cout << largest;
}
 
 
int main(int argc, char* argv[])
{
    solve();
    return 0;
}