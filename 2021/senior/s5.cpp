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
#include <unordered_map>

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
	uint8_t z;
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
std::multimap<uint32_t, std::pair<uint32_t, bool>> range_map;

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
		scanf("%" SCNu32 " %" SCNu32 " %" SCNu8, &ranges[i].start, &ranges[i].end, &ranges[i].z);
		factorize(ranges[i].z, &ranges[i].factor);

		range_map.emplace(ranges[i].start, std::make_pair(i, true));
		range_map.emplace(ranges[i].end, std::make_pair(i, false));
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
	std::unordered_map<uint32_t, prime_factor> satisfied_ranges;
	std::unordered_map<uint32_t, prime_factor> unsatisfied_ranges;

	auto it = range_map.begin();

	prime_factor curr_factor;
	memset(&curr_factor, 0, sizeof(curr_factor));
	uint32_t curr_factor_count[10] = { 0 };
	uint32_t curr_value = 1;
	uint32_t last_pos = 1;

	while (it != range_map.end()) {
		uint32_t pos = it->first;
		uint32_t range_id = it->second.first;

		/* populate lists of ranges starting and ending */
		std::vector<uint32_t> starting;
		std::vector<uint32_t> ending;

		if (it->second.second)
			starting.push_back(range_id);
		else
			ending.push_back(range_id);

		/* write out values since the last position */
		for (uint32_t i = last_pos; i < pos; i++)
			output[i] = curr_value;

		/* update all current ranges */
		for (auto it = unsatisfied_ranges.begin(); it != unsatisfied_ranges.end();) {
			prime_factor_min(&it->second, &curr_factor);

			/* all we need to do is reach the mimimum once */
			if (prime_factor_eq(&it->second, &ranges[it->first].factor)) {
				satisfied_ranges.emplace(it->first, it->second);
				it = unsatisfied_ranges.erase(it);
			} else {
				++it;
			}
		}

		/* look for other ranges starting or ending at pos */
		while ((++it)->first == pos) {
			if (it->second.second)
				starting.push_back(it->second.first);
			else
				ending.push_back(it->second.first);
		}

		/* process starting ranges */
		for (auto &it : starting) {
			prime_factor_max(&curr_factor, &ranges[it].factor);
			prime_factor factor_min;
			memset(&factor_min, 255, sizeof(factor_min));
			unsatisfied_ranges.emplace(it, factor_min);

			if (ranges[it].factor.power[0] > 0)
				curr_factor_count[ranges[it].factor.power[0] - 1]++;
			if (ranges[it].factor.power[1] > 0)
				curr_factor_count[ranges[it].factor.power[1] + 3]++;
			for (uint8_t i = 2; i < NUM_PRIMES; i++)
				curr_factor_count[i + 4] += ranges[it].factor.power[i];
		}

		/* update all current ranges */
		for (auto it = unsatisfied_ranges.begin(); it != unsatisfied_ranges.end();) {
			prime_factor_min(&it->second, &curr_factor);

			/* all we need to do is reach the mimimum once */
			if (prime_factor_eq(&it->second, &ranges[it->first].factor)) {
				satisfied_ranges.emplace(it->first, it->second);
				it = unsatisfied_ranges.erase(it);
			} else {
				++it;
			}
		}

		output[pos] = multiply(&curr_factor);

		/* process ending ranges */
		for (auto &it : ending) {
			if (ranges[it].factor.power[0] > 0)
				curr_factor_count[ranges[it].factor.power[0] - 1]--;
			if (ranges[it].factor.power[1] > 0)
				curr_factor_count[ranges[it].factor.power[1] + 3]--;
			for (uint8_t i = 2; i < NUM_PRIMES; i++)
				curr_factor_count[i + 4] -= ranges[it].factor.power[i];

			if (satisfied_ranges.count(it) == 0) {
				printf("Impossible");
				return;
			}
			satisfied_ranges.erase(it);
		}

		/* calculate curr_factor */
		curr_factor.power[0] = 0;
		for (int i = 3; i >= 0; i--) {
			if (curr_factor_count[i] > 0) {
				curr_factor.power[0] = i + 1;
				break;
			}
		}
		curr_factor.power[1] = 0;
		for (int i = 1; i >= 0; i--) {
			if (curr_factor_count[i + 4] > 0) {
				curr_factor.power[1] = i + 1;
				break;
			}
		}
		for (uint8_t i = 2; i < NUM_PRIMES; i++)
			curr_factor.power[i] = (curr_factor_count[i + 4] > 0) ? 1 : 0;

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
