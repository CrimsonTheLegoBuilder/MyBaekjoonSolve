#include <iostream>
#include <vector>
#include <algorithm>

struct Pos { 
	int x, y;
	int dist(const Pos& h) {
		return (x - h.x) * (x - h.x) + (y - h.y) * (y - h.y);
	}
} a, b;
std::vector<Pos> H;
int h, q, r1, r2, ai, bi;
std::vector<int> A, B;

int bi_search(std::vector<int>& D, int& d) {
	int s = 0, e = D.size() - 1;
	while (s < e) {
		int m = s + e >> 1;
		if (D[m] > d) e = m;
		else s = m + 1;
	}
	return s;
}

int main() {
	std::cin.tie(0)->sync_with_stdio(0);
	std::cout.tie(0);
	int Q = 1;
	while (1) {
		std::cin >> h;
		if (!h) return 0;
		std::cout << "Case " << Q++ << ":\n";
		H.clear(); A.clear(); B.clear();
		H.resize(h);
		for (int i = 0; i < h; i++) {
			std::cin >> H[i].x >> H[i].y;
		}
		std::cin >> a.x >> a.y >> b.x >> b.y >> q;
		for (const Pos& i : H) {
			A.push_back(a.dist(i));
			B.push_back(b.dist(i));
		}
		std::sort(A.begin(), A.end());
		std::sort(B.begin(), B.end());
		for (int i = 0; i < q; i++) {
			std::cin >> r1 >> r2;
			int R1 = r1 * r1, R2 = r2 * r2;
			ai = bi_search(A, R1);
			bi = bi_search(B, R2);
			if (h <= ai + bi) std::cout << "0\n";
			else std::cout << h - ai - bi << "\n";
		}
	}
}
