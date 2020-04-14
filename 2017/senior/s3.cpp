/* SPDX-License-Identifier: MIT */
/*
 * CCC 2017 Senior 3 - Nailed It!
 * Copyright (c) 2017 Emmanuel Mathi-Amorim
 */

#include <iostream>
#include <algorithm>
#include <cstring>

int n;
int countingSort[2001];
int lengths[2001];

void read_input()
{
	std::cin >> n;

	memset(countingSort, 0, sizeof(countingSort));

	for (int i = 0; i < n; i++)
	{
		int l;
		std::cin >> l;
		countingSort[l]++;
	}
}

int find_number_of_boards_at_height(int height)
{
	memcpy(lengths, countingSort, sizeof(lengths));

	int a = std::max(1, height - 2000);
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
			int boards = std::min(lengths[a], lengths[b]);
			lengths[a] -= boards;
			lengths[b] -= boards;

			num_of_boards += boards;
		}

		++a;
		--b;
	}

	return num_of_boards;
}

int main()
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

	std::cout << max_boards << " " << heights_at_max_boards;
	std::cin >> n;

    return 0;
}