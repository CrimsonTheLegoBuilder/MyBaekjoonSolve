#include <iostream>
#include <algorithm>
#include <vector>
typedef long long ll;
int N, x, r;
struct C {
	int x, r, i;
	bool operator < (const C& c) const {
		if (x == c.x) {
			if (i * c.i < 0) return i > c.i;
			if (i < 0) return r > c.r;
			if (i > 0) return r < c.r;
		}
		return x < c.x;
	}
};
std::vector<C> V;// , S;
std::vector<int> s;

int main() {
	std::cin.tie(0)->sync_with_stdio(0);
	std::cout.tie(0);
	std::cin >> N;
	for (int i = 1; i <= N; i++) {
		std::cin >> x >> r;
		V.push_back({ x - r, r, i });
		V.push_back({ x + r, r, -i });
	}
	std::sort(V.begin(), V.end());
	for (const C& c : V) {
		if (c.i < 0) {
			//if (S.size() && S[S.size() - 1].i == -c.i) S.pop_back();
			//else { std::cout << "NO\n"; return 0; }
			if (s.size() && s[s.size() - 1] == -c.i) s.pop_back();
			else { std::cout << "NO\n"; return 0; }
		}
		else if (c.i > 0) {
			//if (S.size() && S[S.size() - 1].x == c.x) {
			//	std::cout << "NO\n"; return 0;
			//}
			//S.push_back(c);
			s.push_back(c.i);
		}
	}
	std::cout << "YES\n"; return 0;
}