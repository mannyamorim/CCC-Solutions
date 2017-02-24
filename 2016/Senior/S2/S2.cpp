// CCC 2016 Senior 2
// Solution by Emmanuel Mathi-Amorim

#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
 
//---------------------------------------------------------------------------
// Input
//---------------------------------------------------------------------------
  
long question;
long totalPeople;
std::vector<long> A,B;
  
void read_input()
{
    std::cin >> question >> totalPeople;
    A.reserve(totalPeople);
    B.reserve(totalPeople);
    for (int i=0;i<totalPeople;i++)
    {
        int v;
        std::cin >> v;
        A.push_back(v);
    }
    for (int i=0;i<totalPeople;i++)
    {
        long v;
        std::cin >> v;
        B.push_back(v);
    }
}
 
//---------------------------------------------------------------------------
// Sort
//---------------------------------------------------------------------------
  
bool compare_increasing(long a,long b)
{
    return (a<b);
}
  
void sort_increasing(std::vector<long> & vector)
{
    std::sort(vector.begin(),vector.end(),compare_increasing);
}
  
bool compare_decreasing(long a, long b)
{
    return (a>b);
}
  
void sort_decreasing(std::vector<long> & vector)
{
    std::sort(vector.begin(),vector.end(),compare_decreasing);
}
 
//---------------------------------------------------------------------------
// Solve
//---------------------------------------------------------------------------
  
void arrange_bikers()
{
    sort_increasing(A);
  
    // Maximum?
    if (question == 2)
        sort_decreasing(B);
    else
        sort_increasing(B);
}
 
long computeMaxSum()
{
    long sum = 0;
    for (int i=0;i<totalPeople;i++) 
        sum += std::max(A[i],B[i]);
    return sum;
}
 
int main(int argc, char* argv[])
{
    read_input();
    arrange_bikers();
    std::cout << computeMaxSum();
    return 0;
}