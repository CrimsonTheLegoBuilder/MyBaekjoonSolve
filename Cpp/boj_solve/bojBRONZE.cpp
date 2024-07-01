#include <iostream>
#include <algorithm>
#include <cmath>
#include <cstring>
typedef long long ll;
int T, N, W, H, D, L;
inline ll sq(int x) { return (ll)x * x; }

struct Pos {
	int x, y;
	inline Pos(int X = 0, int Y = 0) : x(X), y(Y) {}
	inline bool operator == (const Pos& p) const { return x == p.x && y == p.y; }
	inline bool operator < (const Pos& p) const { return x == p.x ? y < p.y : x < p.x; }
	inline Pos operator + (const Pos& p) const { return { x + p.x, y + p.y }; }
	inline Pos operator - (const Pos& p) const { return { x - p.x, y - p.y }; }
	inline ll operator * (const Pos& p) const { return (ll)x * p.x + (ll)y * p.y; }
	inline ll operator / (const Pos& p) const { return (ll)x * p.y - (ll)y * p.x; }
	inline ll Euc() const { return (ll)x * x + (ll)y * y; }
	inline friend std::istream& operator >> (std::istream& is, Pos& p) { is >> p.x >> p.y; return is; }
	inline friend std::ostream& operator << (std::ostream& os, const Pos& p) { os << p.x << " " << p.y; return os; }
} p;
int main() {
	std::cin.tie(0)->sync_with_stdio(0);
	std::cout.tie(0);
	std::cin >> T;
	while (T--) {
		std::cin >> N;
		ll all = 0;
		while (N--) {
			std::cin >> p;
			ll d = p.Euc();
			if (d <= sq(20)) all += 10;
			else if (d <= sq(40)) all += 9;
			else if (d <= sq(60)) all += 8;
			else if (d <= sq(80)) all += 7;
			else if (d <= sq(100)) all += 6;
			else if (d <= sq(120)) all += 5;
			else if (d <= sq(140)) all += 4;
			else if (d <= sq(160)) all += 3;
			else if (d <= sq(180)) all += 2;
			else if (d <= sq(200)) all += 1;
		}
		//std::cout << all << "\n";
	}
	return 0;
}