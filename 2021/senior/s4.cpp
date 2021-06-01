/* SPDX-License-Identifier: MIT */
/*
 * CCC 2021 Senior 4 - Daily Commute
 * Copyright (c) 2021 Emmanuel Mathi-Amorim
 */

#include <cstdio>
#include <queue>
#include <set>
#include <vector>

constexpr int MAX_N = 200'000;

/* num stations, num edges, num days */
int n, w, d;

/* edges in reverse */
std::vector<int> edges[MAX_N + 1];

/* the subway stations */
int subway[MAX_N + 1];

/* the distance from a station to the destination using walkways */
int distance[MAX_N + 1];

/* stores a subway station and the time to use that station */
struct subway_time {
	int station;
	int time;

	subway_time(int station, int time) :
		station(station),
		time(time)
	{}

	bool operator <(const subway_time &other) const {
		if (time < other.time)
			return true;

		if (time == other.time)
			return station < other.station;

		return false;
	}
};

/* ordered set of all the subway stations with their times */
std::set<subway_time> subway_times;

/* read the input */
void read_input() {
	scanf("%d %d %d", &n, &w, &d);

	for (int i = 0; i < w; i++) {
		int a, b;
		scanf("%d %d", &a, &b);
		edges[b].push_back(a);
	}

	for (int i = 1; i <= n; i++)
		scanf(" %d", &subway[i]);
}

/* compute the distance from all stations to station N */
void bfs() {
	for (int i = 1; i <= n; i++)
		distance[i] = -1;

	std::queue<int> q;

	distance[n] = 0;
	q.push(n);
	while (!q.empty()) {
		int vertex = q.front();
		q.pop();
		for (int edge : edges[vertex]) {
			if (distance[edge] == -1) {
				distance[edge] = distance[vertex] + 1;
				q.push(edge);
			}
		}
	}
}

/* add subway station to the subway time set */
void add_subway_time(int station) {
	subway_times.emplace(subway[station], distance[subway[station]] + station - 1);
}

/* remove a subway station from the subway time set */
void delete_subway_time(int station) {
	subway_time to_delete(subway[station], distance[subway[station]] + station - 1);
	subway_times.erase(to_delete);
}

/* read in each of the days swaps and solve */
void solve() {
	/* load all of the stations into the ordered set */
	for (int i = 1; i <= n; i++)
		if (distance[subway[i]] != -1)
			add_subway_time(i);

	/* go through all of the swaps and compute the minimum time */
	for (int i = 0; i < d; i++) {
		int x, y;
		scanf("%d %d", &x, &y);

		if (distance[subway[x]] != -1)
			delete_subway_time(x);
		if (distance[subway[y]] != -1)
			delete_subway_time(y);

		int tmp;
		tmp = subway[x];
		subway[x] = subway[y];
		subway[y] = tmp;

		if (distance[subway[x]] != -1)
			add_subway_time(x);
		if (distance[subway[y]] != -1)
			add_subway_time(y);

		int min_time = subway_times.begin()->time;
		printf("%d\n", min_time);
	}
}

/* main entrypoint */
int main() {
	read_input();
	bfs();
	solve();
	return 0;
}
