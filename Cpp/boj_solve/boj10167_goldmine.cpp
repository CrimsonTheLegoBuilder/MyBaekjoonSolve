#include <iostream>
#include <vector>
#include <algorithm>
typedef long long ll;
const int LEN = 3'000;
int N;

struct Pos {
	int x, y;
	ll w;
}M[LEN]; std::vector<Pos> M2[LEN];

struct Coord {
	int c, i;
	bool operator < (const Coord& x) const {
		return c < x.c;
	}
}X[LEN], Y[LEN];

struct Seg {
	ll max, lsum, rsum, sum;
	Seg operator + (const Seg& r) {
		return {
			std::max({max, r.max, rsum + r.lsum}),
			std::max({lsum, sum + r.lsum}),
			std::max({rsum + r.sum, r.rsum}),
			sum + r.sum
		};
	}
}T[LEN << 2];

void update(int x, int v, int s = 0, int e = N - 1, int n = 1) {
	if (s > x || x > e) return;
	if (s == e) {
		ll cur = T[n].max;
		T[n] = { cur + v, cur + v, cur + v, cur + v };
		return;
	}
	int m = s + e >> 1;
	update(x, v, s, m, n << 1);
	update(x, v, m + 1, e, n << 1 | 1);
	T[n] = T[n << 1] + T[n << 1 | 1];
}
void init() {
	for (int i = 0; i < LEN << 2; i++) {
		T[i] = { 0, 0, 0, 0 };
	}
}



int main() {
	std::cin >> N;
	for (int i = 0; i < N; i++) {
		std::cin >> X[i].c >> Y[i].c >> M[i].w;
		X[i].i = Y[i].i = i;
	}

	std::sort(X, X + N);
	int x = -1;
	for (int i = 0, j = -1; i < N; i++) {
		if (X[i].c != x) {
			x = X[i].c, j++;
		}
		M[X[i].i].x = j;
	}
	std::sort(Y, Y + N);
	int y = -1;
	for (int i = 0, j = -1; i < N; i++) {
		if (Y[i].c != y) {
			y = Y[i].c, j++;
		}
		M[Y[i].i].y = j;
	}

	int E = 0;
	for (int i = 0; i < N; i++) {
		M2[M[i].y].push_back(M[i]);
		E = std::max(E, M[i].y);
	}

	ll F = 0;
	for (int i = 0; i <= E; i++) {
		init();
		for (int j = i; j <= E; j++) {
			for (const Pos& g : M2[j]) {
				update(g.x, g.w);
			}
			F = std::max(F, T[1].max);
		}
	}

	std::cout << F << "\n";
	return 0;
}