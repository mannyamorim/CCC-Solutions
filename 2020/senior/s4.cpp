#include <climits>
#include <cstdio>
#include <cstring>

#include <algorithm>

#define MAX_N 1000000

int n;
char data[MAX_N + 2];

static void read_input() {
	fgets(data, sizeof(data), stdin);
	n = strlen(data);
        if (data[n - 1] == '\n') {
                data[n - 1] == '\0';
                n--;
        }
}

struct counts {
	int a = 0;
	int b = 0;
	int c = 0;
};

#define INC(d, ch) switch((ch)) { case 'A': (d).a++; break; case 'B': (d).b++; break; case 'C': (d).c++; break; default: exit(1); }
#define DEC(d, ch) switch((ch)) { case 'A': (d).a--; break; case 'B': (d).b--; break; case 'C': (d).c--; break; default: exit(1); }

static counts count_range(int start, int end) {
	counts c;

	for (int i = start; i < end; i++)
		INC(c, data[i])

	return c;
}

/*
 * Compute the number of swaps required to rearrange the people so that the A's
 * will be in the range represented by counts a and vice versa for B's and C's.
 */
static int compute_swaps(counts total, counts a, counts b, counts c) {
        int swaps = 0;

        /* move all of the a's to the a range */
        swaps += b.a;
        swaps += c.a;

        if (a.b > b.a) {
                /* the a range has two many b's */
                b.b += b.a;
                c.b += (a.b - b.a);
        } else {
                /* the a range has two many c's */
                c.c += c.a;
                b.c += (a.c - c.a);
        }

        /* swap the b's and c's */
        swaps += b.c;

        return swaps;
}

#define WRAP(i) (((i) >= n) ? ((i) - n) : ((i)))

/*
 * Iterate through all of the possible positions (n) along the circular table
 * and return the minimum number of swaps required to rearrange the people
 * into ranges specified by the starting indices, sa, sb, sc.
 */
static int solve_ranges(counts total, int sa, int sb, int sc) {
	counts a = count_range(sa, sa + total.a);
	counts b = count_range(sb, sb + total.b);
	counts c = count_range(sc, sc + total.c);

	int best = compute_swaps(total, a, b, c);

	for (int i = 0; i < n - 1; i++) {
		DEC(a, data[WRAP(sa)]);
		DEC(b, data[WRAP(sb)]);
		DEC(c, data[WRAP(sc)]);

		INC(a, data[WRAP(sa + total.a)]);
		INC(b, data[WRAP(sb + total.b)]);
		INC(c, data[WRAP(sc + total.c)]);

		sa++;
		sb++;
		sc++;

		best = std::min(best, compute_swaps(total, a, b, c));
	}

	return best;
}

static int solve() {
        counts total = count_range(0, n);

	if (total.a == 0 && total.b == 0)
		return 0;
	else if (total.b == 0 && total.c == 0)
		return 0;
	else if (total.a == 0 && total.c == 0)
		return 0;

        int best = INT_MAX;

	best = std::min(best, solve_ranges(total, 0, total.a, total.a + total.b)); /* ABC also BCA and CAB */
	best = std::min(best, solve_ranges(total, total.c + total.b, total.c, 0)); /* CBA also BAC and ACB */

        return best;
}

int main() {
	read_input();
	int swaps = solve();
	printf("%d", swaps);
}

