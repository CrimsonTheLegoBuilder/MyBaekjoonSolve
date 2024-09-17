#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <algorithm>
#include <cmath>
#include <cstring>
#include <cassert>
#include <vector>
#include <queue>
#include <set>
typedef long long ll;
typedef std::vector<int> Vint;
typedef std::vector<ll> Vll;
typedef std::priority_queue<int> HQint;
typedef std::priority_queue<ll> HQll;
const int LEN = 1e5 + 5;

int N;
int P[LEN];
int find(int i) { return P[i] < 0 ? i : P[i] = find(P[i]); }
bool join(int i, int j) {
	i = find(i), j = find(j);
	if (i == j) return 0;
	if (P[i] < P[j]) P[i] += P[j], P[j] = i;
	else P[j] += P[i], P[i] = j;
	return 1;
}

int idx_bi_search(const Vint& A, const int& x) {
	int s = 0, e = A.size() - 1, m;
	while (s <= e) {
		m = s + e >> 1;
		if (A[m] == x) return m;
		else if (A[m] > x) e = m - 1;
		else s = m + 1;
	}
	return -1;
}