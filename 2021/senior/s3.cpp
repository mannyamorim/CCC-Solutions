/* SPDX-License-Identifier: MIT */
/*
 * CCC 2021 Senior 3 - Lunch Concert
 * Copyright (c) 2021 Emmanuel Mathi-Amorim
 */

#include <algorithm>
#include <cstdio>

constexpr int MAX_N = 200'000;

/* number of people */
int n;

/* a cost function with the slope and x-intercept */
struct cost_func {
	long slope;
	long intercept;
};

/* list of all cost functions */
cost_func cost_functions[MAX_N * 2];

/* comparator for cost functions */
bool cost_func_compare(cost_func a, cost_func b) {
	return a.intercept < b.intercept;
}

/* read the input */
void read_input() {
	scanf("%d", &n);

	for (int i = 0; i < n; i++) {
		long p, w, d;
		scanf("%ld %ld %ld", &p, &w, &d);

		cost_functions[i * 2].slope = -w;
		cost_functions[i * 2].intercept = p - d;
		cost_functions[i * 2 + 1].slope = w;
		cost_functions[i * 2 + 1].intercept = p + d;
	}

	std::sort(cost_functions, cost_functions + (n * 2), cost_func_compare);
}

/* find the optimal location */
long find_optimal_location() {
	long curr_slope = 0;
	long curr_location;

	/* sum up all of the negative slopes as we are starting on the far left */
	for (int i = 0; i < n * 2; i++)
		if (cost_functions[i].slope < 0)
			curr_slope += cost_functions[i].slope;

	/* go from left to right until our slope becomes >= 0 */
	int i = 0;
	while (curr_slope < 0) {
		/* jump to the next location */
		curr_location = cost_functions[i].intercept;

		/* iterate through all of the functions at the location */
		while (cost_functions[i].intercept == curr_location) {
			if (cost_functions[i].slope > 0)
				curr_slope += cost_functions[i].slope;
			else
				curr_slope -= cost_functions[i].slope;

			i++;
		}
	}

	return curr_location;
}

/* find the cost at the optimal location */
long compute_cost(long location) {
	long total_cost = 0;

	for (int i = 0; i < n * 2; i++) {
		long dist = location - cost_functions[i].intercept;
		long cost = dist * cost_functions[i].slope;

		if (cost > 0)
			total_cost += cost;
	}

	return total_cost;
}

int main() {
	read_input();
	long location = find_optimal_location();
	long cost     = compute_cost(location);
	printf("%ld", cost);
	return 0;
}
