#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
typedef long long ll;
int T, N, e;

struct Pos {
	int x, y, i;
	bool operator < (const Pos& p) { return (x == p.x ? y < p.y : x < p.x); }
}P;
std::vector<Pos> H;
int cross(const Pos& p1, const Pos& p2, const Pos& p3) {
	return (p2.x - p1.x) * (p3.y - p2.y) - (p3.x - p2.x) * (p2.y - p1.y);
}
int dist(const Pos& p1, const Pos& p2) {
	return (p2.x - p1.x) * (p2.x - p1.x) + (p2.y - p1.y) * (p2.y - p1.y);
}
bool cmp(const Pos& a, const Pos& b) {
	int A = cross(P, a, b);
	if (!A) return dist(P, a) < dist(P, b);
	return A > 0;
}

int main() {
	std::cin.tie(0)->sync_with_stdio(0);
	std::cout.tie(0);
	std::cin >> T;
	while (T--) {
		std::cin >> N;
		H.resize(N);
		for (int i = 0; i < N; i++) {
			std::cin >> H[i].x >> H[i].y;
			H[i].i = i;
		}
		int x = std::min_element(H.begin(), H.end()) - H.begin();
		std::swap(H[0], H[x]);
		P = H[0];
		std::sort(H.begin() + 1, H.end(), cmp);
		int i = H.size() - 1;
		for (i; i >= 0; i--) {
			int pre = (i + 1) % H.size(), nxt = i - 1;
			if (cross(H[pre], H[i], H[nxt]) != 0) break;
		}
		std::reverse(H.begin() + i, H.end());
		for (const Pos& p : H) {
			std::cout << p.i << " ";
		}
		std::cout << "\n";
	}
	return 0;
}