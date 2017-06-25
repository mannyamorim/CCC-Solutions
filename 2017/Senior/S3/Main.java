// CCC 2017 Senior 3
// Solution by Emmanuel Mathi-Amorim
// Refractoring by Tobias Carryer

import java.util.Scanner;

public class Main
{
    final static int maxWoodLength = 2000;

    static int[] amountOf = new int[maxWoodLength+1];

    static void readInput()
    {
        Scanner sc = new Scanner(System.in);
        int numOfWood = sc.nextInt();

        for( int i = 0; i < numOfWood; i++ )
        {
            int length = sc.nextInt();
            amountOf[length]++;
        }
    }

    static int findNumberOfBoardsAtHeight( int height )
    {
        int[] amountOfCopy = new int[amountOf.length];

        for( int i = 0; i < amountOf.length; i++ )
        {
            amountOfCopy[i] = amountOf[i];
        }

        int a = Math.max(1, height - maxWoodLength);
        int b = height - a;

        int numOfBoards = 0;

        while( a <= (height / 2) )
        {
            if (a == b)
            {
                int boards = amountOfCopy[a] / 2;
                amountOfCopy[a] -= boards * 2;

                numOfBoards += boards;
            }
            else
            {
                int boards = Math.min(amountOfCopy[a], amountOfCopy[b]);
                amountOfCopy[a] -= boards;
                amountOfCopy[b] -= boards;

                numOfBoards += boards;
            }

            ++a;
            --b;
        }

        return numOfBoards;
    }

    public static void main(String[] args)
    {
        readInput();

        int maxBoards = 0;
        int heightAtMaxBoards = 0;

        for( int i = 2; i <= 4000; i++ )
        {
            int boards = findNumberOfBoardsAtHeight(i);

            if( boards > maxBoards )
            {
                maxBoards = boards;
                heightAtMaxBoards = 0;
            }
            if( boards == maxBoards )
            {
                heightAtMaxBoards++;
            }
        }

        System.out.println(maxBoards + " " + heightAtMaxBoards);
    }
}
