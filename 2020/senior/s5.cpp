/* SPDX-License-Identifier: MIT */
/*
 * CCC 2020 Senior 5 - Josh's Double Bacon Deluxe
 * Copyright (c) 2020 Emmanuel Mathi-Amorim
 */

#include <cstdio>
#include <cstring>

#include <map>

#define MAX_N 1000000
#define MAX_M 500001

int n;                    /* number of people */
int choices[MAX_N];       /* the input, which burger each person chose */
int last[MAX_M] = { -1 }; /* the index of the last person to choose each type of burger */

#define coach choices[0]
#define josh  choices[n - 1]

/* an ordered map of hte data in the last array where the key is the position and the value is the burger */
std::map<int, int> last_by_pos;

void read_input() {
	scanf("%d", &n);
	for (int i = 0; i < n; i++) {
		scanf("%d", &choices[i]);
		last[choices[i]] = i;
	}

	for (int i = 1; i < MAX_M; i++) {
		if (last[i] != -1 && i != coach && i != josh)
			last_by_pos.emplace(last[i], i);
	}
}

double solve() {
	/* short circuit case, if the coach and josh chose the same burger, josh must get his pick */
	if (coach == josh)
		return 1.;

	double *solutions = new double[MAX_N];

	int i = n - 2;
	int n_old, n_new;

	/* solve all of the subproblems starting from the end moving to the beginning */
	for (auto it = last_by_pos.crbegin(); it != last_by_pos.crend(); it++) {
		n_old = n_new;
		n_new = n - it->first;

		double prob = 0.;

		for (; i > it->first; i--) {
			if (choices[i] == coach)
				prob += 1. / (double)n_new;
			else if (choices[i] != josh)
				prob += (1. / (double)n_new) * solutions[last[choices[i]]];
		}

		if (it != last_by_pos.crbegin())
			prob += solutions[n - n_old] * ((double)n_old / (double)n_new);
		else
			prob += 1. / (double)n_new;

		solutions[it->first] = prob;
	}

	/* compute the final probability */
	n_old = n_new;
	n_new = n;

	double prob = 0.;

	for (; i > 0; i--) {
		if (choices[i] == coach)
			prob += 1. / (double)n_new;
		else if (choices[i] != josh)
			prob += (1. / (double)n_new) * solutions[last[choices[i]]];
	}
	
	prob += solutions[n - n_old] * ((double)n_old / (double)n_new);

	delete[] solutions;

	return prob;
}

int main() {
	read_input();
	double prob = solve();
	printf("%.10f", prob);
}

