#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#endif
#include <iostream>
#include <vector>
#include <algorithm>
typedef long long ll;

struct Pos {
	ll x, y;
	bool operator<(const Pos& p) const {
		if (x == p.x) return (y < p.y);
		return x < p.x;
	}
};
ll cross(const Pos& d1, const Pos& d2, const Pos& d3, const Pos& d4) { return (d2.x - d1.x) * (d4.y - d3.y) - (d2.y - d1.y) * (d4.x - d3.x); }
ll dot(const Pos& d1, const Pos& d2, const Pos& d3, const Pos& d4) { return (d2.x - d1.x) * (d4.x - d3.x) + (d2.y - d1.y) * (d4.y - d3.y); }
bool is_cross(const Pos& d1, const Pos& d2, const Pos& d3, const Pos& d4) {
	bool f11 = cross(d1, d2, d2, d3) * cross(d2, d1, d1, d4) > 0;
	bool f12 = cross(d3, d4, d4, d1) * cross(d4, d3, d3, d2) > 0;
	bool f1 = f11 && f12;
	bool f2 = (!cross(d1, d3, d3, d2) && dot(d1, d3, d3, d2) >= 0) ||
		(!cross(d1, d4, d4, d2) && dot(d1, d4, d4, d2) >= 0) ||
		(!cross(d3, d1, d1, d4) && dot(d3, d1, d1, d4) >= 0) ||
		(!cross(d3, d2, d2, d4) && dot(d3, d2, d2, d4) >= 0);
	return f1 || f2;
}
bool is_cross_weak(const Pos& d1, const Pos& d2, const Pos& d3, const Pos& d4) {
	bool f11 = cross(d1, d2, d2, d3) * cross(d2, d1, d1, d4) > 0;
	bool f12 = cross(d3, d4, d4, d1) * cross(d4, d3, d3, d2) > 0;
	return f11 && f12;
}
bool NCVXHIN(std::vector<Pos>& H, Pos& d) {
	int cnt = 0, L = H.size();
	for (int i = 0; i < L; i++) {
		Pos d1 = H[i], d2 = H[(i + 1) % L];
		if (d1.y > d2.y) std::swap(d1, d2);
		if (!cross(d1, d, d, d2) && dot(d1, d, d, d2) >= 0) return 1;
		if (d.x > std::max(d1.x, d2.x)) continue;
		if (d.y <= d1.y) continue;
		if (d.y > d2.y) continue;
		if (d.y == d1.y && d.y == d2.y) continue;
		if (cross(d1, d2, d2, d) < 0) continue;
		if (d1.y < d.y && d.y <= d2.y) cnt++;
		else if (cross(d1, d2, d2, d) > 0) cnt++;
	}
	return cnt % 2;
}


int main() {
	std::cin.tie(0)->sync_with_stdio(0);
	setbuf(stdout, NULL);
	freopen("boj6632.txt", "r", stdin);
	freopen("result.txt", "w", stdout);
	while (1) {
		int N, x, y, cnt = 0, L;
		std::vector<Pos> H;
		std::cin >> N;
		if (!N) return 0;
		//std::cout << N;
		for (int i = 0; i < N; i++) {
			std::cin >> x >> y;
			H.push_back({ x * 2, y * 2 });
		}
		L = H.size();
		//std::cout << L << "\n";
		for (int i = -199; i <= 199; i += 2) {
			for (int j = -199; j <= 199; j += 2) {
				Pos d = { i, j }, dr1 = { i + 1, j + 1 }, dr2 = { i - 1, j - 1 }, dl1 = { i + 1,j - 1 }, dl2 = { i - 1,j + 1 };
				if (NCVXHIN(H, d)) {
					//std::cout << "I";
					bool f = 1;
					for (int k = 0; k < L; k++) {
						//std::cout << is_cross(H[k], H[(k + 1) % L], dr1, dr2) << is_cross(H[k], H[(k + 1) % L], dl1, dl2) << "\n";
						if (is_cross(H[k], H[(k + 1) % L], d, d) || is_cross_weak(H[k], H[(k + 1) % L], dr1, dr2) || (is_cross_weak(H[k], H[(k + 1) % L], dl1, dl2))) f = 0;
					}
					if (f) cnt += 1;
				}
			}
		}
		std::cout << cnt << "\n";
	}
}