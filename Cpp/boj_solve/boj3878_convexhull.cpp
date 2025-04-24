#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <numeric>
typedef long long ll;
const int LEN = 100;
int T, N, M, vx, vy;

struct Pos {
	ll x, y;
	bool operator<(const Pos& p) const { return x == p.x ? y < p.y : x < p.x; }
	bool operator==(const Pos& p) const { return x == p.x && y == p.y; }
};
std::vector<Pos> w, b, W, B;

ll cross(const Pos& d1, const Pos& d2, const Pos& d3) {
	return (d2.x - d1.x) * (d3.y - d2.y) - (d2.y - d1.y) * (d3.x - d2.x);
}
ll dot(const Pos& d1, const Pos& d2, const Pos& d3) {
	return (d2.x - d1.x) * (d3.x - d2.x) + (d2.y - d1.y) * (d3.y - d2.y);
}
bool X(const Pos& d1, const Pos& d2, const Pos& d3) {
	return (!cross(d1, d3, d2) && (dot(d1, d3, d2) >= 0));
}
std::vector<Pos> monotone_chain(std::vector<Pos>& C) {
	std::vector<Pos> H;
	std::sort(C.begin(), C.end());
	if (C.size() <= 2) {
		for (const Pos& pos : C) H.push_back(pos);
		return H;
	}
	for (int i = 0; i < C.size(); i++) {
		while (H.size() > 1 && cross(H[H.size() - 2], H[H.size() - 1], C[i]) <= 0) {
			H.pop_back();
		}
		H.push_back(C[i]);
	}
	H.pop_back();
	int s = H.size() + 1;
	for (int i = C.size() - 1; i >= 0; i--) {
		while (H.size() > s && cross(H[H.size() - 2], H[H.size() - 1], C[i]) <= 0) {
			H.pop_back();
		}
		H.push_back(C[i]);
	}
	H.pop_back();
	return H;
}
bool I(std::vector<Pos>& H, const Pos& x) {
	int h = H.size() - 1;
	if (h < 1 || cross(H[0], H[1], x) < 0 || cross(H[0], H[h], x) > 0) return 0;
	if (X(H[0], H[1], x) || X(H[0], H[h], x)) return 1;
	int s = 0, e = h, m;
	while (s + 1 < e) {
		m = s + e >> 1;
		ll C = cross(H[0], H[m], x);
		if (!C) return (dot(H[0], x, H[m]) >= 0);
		else if (C > 0) s = m;
		else e = m;
	}
	//if (X(H[s], H[e], x)) return 1;
	return cross(H[s], H[e], x) > 0 || X(H[s], H[e], x);
}
bool C(const Pos& d1, const Pos& d2, const Pos& d3, const Pos& d4) {
	bool f1 = cross(d1, d2, d3) * cross(d2, d1, d4) > 0;
	bool f2 = cross(d3, d4, d1) * cross(d4, d3, d2) > 0;
	return f1 && f2;
}

int main() {
	std::cin.tie(0)->sync_with_stdio(0);
	std::cout.tie(0);
	std::cin >> T;
	while (T--) {
		bool F = 0;
		std::cin >> N >> M;
		b.resize(N), w.resize(M);
		for (int i = 0; i < N; i++) { std::cin >> b[i].x >> b[i].y; }
		for (int i = 0; i < M; i++) { std::cin >> w[i].x >> w[i].y; }

		B = monotone_chain(b), W = monotone_chain(w);
		for (const Pos& x : B) if (I(W, x)) F = 1;
		for (const Pos& x : W) if (I(B, x)) F = 1;
		for (int i = 0; i < B.size(); i++) {
			for (int j = 0; j < W.size(); j++) {
				if (C(B[i], B[(i + 1) % B.size()], W[j], W[(j + 1) % W.size()])) F = 1;
			}
		}
		std::cout << (F ? "NO\n" : "YES\n");
	}
	return 0;
}


/*
1
2 2
0 0
500 700
1000 1400
1500 2100
*/