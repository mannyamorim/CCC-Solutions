/* SPDX-License-Identifier: MIT */
/*
 * CCC 2021 Senior 5 -  Math Homework
 * Copyright (c) 2023 Emmanuel Mathi-Amorim
 */

#include <cstdint>
#include <cstdio>
#include <cstring>
#include <cinttypes>
#include <map>
#include <set>
#include <vector>

/* prime factors */
constexpr uint8_t NUM_PRIMES = 6;
constexpr uint8_t primes[NUM_PRIMES] = { 2, 3, 5, 7, 11, 13 };

/* struct to represent array of prime factors */
struct prime_factor {
	uint8_t power[NUM_PRIMES];
};

/* struct to represent a range */
struct range {
	uint32_t start;
	uint32_t end;
	prime_factor factor;
};

/* struct to represent the primes that have been used in a range */
struct range_factors {
	prime_factor factor_min;
	prime_factor factor_max;
};

/* number of integers to output, number of ranges */
uint32_t n, m;

/* array of ranges */
range ranges[150000];

/* ordered map of range start and end indices to index of range within ranges array */
std::multimap<uint32_t, uint32_t> range_map;

/* compute prime factors for an integer */
void factorize(uint8_t z, prime_factor *factor)
{
	memset(factor, 0, sizeof(*factor));

	while (z > 0 && z % 2 == 0) {
		factor->power[0]++;
		z /= 2;
	}

	while (z > 0 && z % 3 == 0) {
		factor->power[1]++;
		z /= 3;
	}

	if (z == 0)
		return;

	for (uint8_t i = 2; i < NUM_PRIMES; i++)
		if (z % primes[i] == 0)
			factor->power[i] = 1;
}

/* compute value from factors */
uint32_t multiply(prime_factor *factor)
{
	uint32_t val = 1;

	for (uint8_t i = 0; i < NUM_PRIMES; i++)
		for (uint8_t j = 0; j < factor->power[i]; j++)
			val *= primes[i];

	return val;
}

/* read the input */
void read_input()
{
	scanf("%" SCNu32 " %" SCNu32, &n, &m);

	for (uint32_t i = 0; i < m; i++) {
		uint8_t z;
		scanf("%" SCNu32 " %" SCNu32 " %" SCNu8, &ranges[i].start, &ranges[i].end, &z);
		factorize(z, &ranges[i].factor);

		range_map.emplace(ranges[i].start, i);
		if (ranges[i].start != ranges[i].end)
			range_map.emplace(ranges[i].end, i);
	}
}

/* prime factor min/max */
void prime_factor_max(prime_factor *factor_a, prime_factor *factor_b)
{
	for (uint8_t i = 0; i < NUM_PRIMES; i++)
		if (factor_b->power[i] > factor_a->power[i])
			factor_a->power[i] = factor_b->power[i];
}

void prime_factor_min(prime_factor *factor_a, prime_factor *factor_b)
{
	for (uint8_t i = 0; i < NUM_PRIMES; i++)
		if (factor_b->power[i] < factor_a->power[i])
			factor_a->power[i] = factor_b->power[i];
}

bool prime_factor_eq(prime_factor *factor_a, prime_factor *factor_b)
{
	for (uint8_t i = 0; i < NUM_PRIMES; i++)
		if (factor_a->power[i] != factor_b->power[i])
			return false;

	return true;
}

/* solver */
void solve()
{
	uint32_t output[150001];
	std::map<uint32_t, prime_factor> current_ranges;

	auto it = range_map.begin();

	prime_factor curr_factor;
	memset(&curr_factor, 0, sizeof(curr_factor));
	uint32_t curr_value = 1;
	uint32_t last_pos = 1;

	while (it != range_map.end()) {
		/* populate lists of ranges starting and ending */
		std::vector<uint32_t> starting;
		std::vector<uint32_t> ending;

		uint32_t pos = it->first;
		if (current_ranges.count(it->second)) {
			ending.push_back(it->second);
		} else {
			starting.push_back(it->second);
			if (ranges[it->second].start == ranges[it->second].end)
				ending.push_back(it->second);
		}

		/* write out values since the last position */
		for (uint32_t i = last_pos; i < pos; i++)
			output[i] = curr_value;

		/* update all ranges with the previous value */
		for (auto &it : current_ranges) {
			prime_factor_min(&it.second, &curr_factor);
		}

		/* look for other ranges starting or ending at pos */
		while ((++it)->first == pos) {
			if (current_ranges.count(it->second)) {
				ending.push_back(it->second);
			} else {
				starting.push_back(it->second);
				if (ranges[it->second].start == ranges[it->second].end)
					ending.push_back(it->second);
			}
		}

		/* process starting ranges */
		for (auto &it : starting) {
			prime_factor_max(&curr_factor, &ranges[it].factor);
			prime_factor factor_min;
			memset(&factor_min, 255, sizeof(factor_min));
			current_ranges.emplace(it, factor_min);
		}

		/* update all current ranges */
		for (auto &it : current_ranges) {
			prime_factor_min(&it.second, &curr_factor);
		}

		output[pos] = multiply(&curr_factor);

		/* process ending ranges */
		for (auto &it : ending) {
			if (!prime_factor_eq(&current_ranges[it], &ranges[it].factor)) {
				printf("Impossible");
				return;
			}
			current_ranges.erase(it);
		}

		memset(&curr_factor, 0, sizeof(curr_factor));
		for (auto &it : current_ranges)
			prime_factor_max(&curr_factor, &ranges[it.first].factor);

		last_pos = pos + 1;
		curr_value = multiply(&curr_factor);
	}

	/* fill in final numbers */
	for (uint32_t i = last_pos; i <= n; i++)
		output[i] = 1;

	/* print the output */
	for (uint32_t i = 1; i <= n; i++)
		printf("%u ", output[i]);
}

/* entrypoint */
int main()
{
	read_input();
	solve();
}
