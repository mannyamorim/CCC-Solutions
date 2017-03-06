// CCC 2017 Senior 3
// Solution by Emmanuel Mathi-Amorim

import java.util.Scanner;

public class Main
{
	static int n;
	static int[] countingSort = new int[2001];
	static int[] lengths = new int[2001];

	static void read_input()
	{
		Scanner sc = new Scanner(System.in);
		n = sc.nextInt();

		for (int i = 0; i < 2001; i++)
		{
			countingSort[i] = 0;
		}

		for (int i = 0; i < n; i++)
		{
			int l = sc.nextInt();
			countingSort[l]++;
		}
	}

	static int find_number_of_boards_at_height(int height)
	{
		for (int i = 0; i < 2001; i++)
		{
			lengths[i] = countingSort[i];
		}

		int a = Math.max(1, height - 2000);
		int b = height - a;

		int num_of_boards = 0;

		while (a <= (height / 2))
		{
			if (a == b)
			{
				int boards = lengths[a] / 2;
				lengths[a] -= boards * 2;

				num_of_boards += boards;
			}
			else
			{
				int boards = Math.min(lengths[a], lengths[b]);
				lengths[a] -= boards;
				lengths[b] -= boards;

				num_of_boards += boards;
			}

			++a;
			--b;
		}

		return num_of_boards;
	}

	public static void main(String[] args)
	{
		read_input();

		int max_boards = 0;
		int heights_at_max_boards = 0;

		for (int i = 2; i <= 4000; i++)
		{
			int boards = find_number_of_boards_at_height(i);

			if (boards > max_boards)
			{
				max_boards = boards;
				heights_at_max_boards = 0;
			}
			if (boards == max_boards)
			{
				heights_at_max_boards++;
			}
		}

		System.out.println(max_boards + " " + heights_at_max_boards);
	}
}