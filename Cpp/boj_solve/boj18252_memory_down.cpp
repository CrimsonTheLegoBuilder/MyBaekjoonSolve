#include <iostream>
#include <algorithm>
#include <vector>
#include <cmath>
typedef long long ll;
//const int LEN = 1e5 + 1;
int N, M, T, Q;

struct Pos {
	int x, y;
	Pos(int X = 0, int Y = 0) : x(X), y(Y) {}
	bool operator < (const Pos& p) const { return x == p.x ? y < p.y : x < p.x; }
	//Pos operator - (const Pos& p) const { return { x - p.x, y - p.y }; }
	//ll operator / (const Pos& p) const { return { x * p.y - y * p.x }; }
	ll Euc() const { return x * x + y * y; }
	friend std::istream& operator >> (std::istream& is, Pos& p);
	friend std::ostream& operator << (std::ostream& os, const Pos& p);
}; const Pos O = { 0, 0 };
std::istream& operator >> (std::istream& is, Pos& p) { is >> p.x >> p.y; return is; }
std::ostream& operator << (std::ostream& os, const Pos& p) { os << p.x << " " << p.y << "\n"; return os; }
std::vector<Pos> C, H;
//ll cross(const Pos& d1, const Pos& d2, const Pos& d3) { return (d2 - d1) / (d3 - d2); }
ll cross(const Pos& d1, const Pos& d2, const Pos& d3) { 
	return (ll)(d2.x - d1.x) * (ll)(d3.y - d2.y) - (ll)(d2.y - d1.y) * (ll)(d3.x - d2.x);
}
ll dist(const Pos& d1, const Pos& d2) {
	return (ll)(d1.x - d2.x) * (ll)(d1.x - d2.x) + (ll)(d1.y - d2.y) * (ll)(d1.y - d2.y);
}
int ccw(const Pos& d1, const Pos& d2, const Pos& d3) {
	ll ret = cross(d1, d2, d3);
	return !ret ? 0 : ret > 0 ? 1 : -1;
}
std::vector<Pos> graham_scan(std::vector<Pos>& C) {
	std::vector<Pos> H;
	if (C.size() < 3) {
		std::sort(C.begin(), C.end());
		return C;
	}
	std::swap(C[0], *min_element(C.begin(), C.end()));
	std::sort(C.begin() + 1, C.end(), [&](const Pos& p, const Pos& q) -> bool {
		int ret = ccw(C[0], p, q);
		//if (!ret) return (C[0] - p).Euc() < (C[0] - q).Euc();
		if (!ret) return dist(C[0], p) < dist(C[0], q);
		return ret > 0;
		}
	);
	int sz = C.size();
	for (int i = 0; i < sz; i++) {
		while (H.size() >= 2 && ccw(H[H.size() - 2], H.back(), C[i]) <= 0)
			H.pop_back();
		H.push_back(C[i]);
	}
	return H;
}
void solve() {
	std::cin.tie(0)->sync_with_stdio(0);
	Pos top, bot;
	ll y, xst, xen;
	std::cin >> N >> top >> bot;
	C = { top, bot };
	for (int i = 0; i < N; i++) {
		std::cin >> y >> xst >> xen;
		if (ccw(bot, top, Pos(xst, y)) < 0) C.push_back(Pos(xst, y));
		if (ccw(bot, top, Pos(xen, y)) > 0) C.push_back(Pos(xen, y));
	}
	H = graham_scan(C);
	int sz = H.size();
	if (sz == 2) { std::cout << "0.0000000\n"; return; }
	if (sz == 3) {
		ll ret = cross(H[0], H[1], H[2]);
		std::cout << (ret >> 1) << (ret & 1 ? ".5000000\n" : ".0000000\n"); return;
	}
	ll pre = 0, ans = 0;
	int i = 0, j = 2, k = 1;
	for (; i < sz; i++) {//O(N)
		pre = 0;
		j = (j - 1 + sz) % sz;
		while ((j + 1) % sz != i) {
			while ((k + 1) % sz != j && cross(H[i], H[(k + 1) % sz], H[j]) > cross(H[i], H[k], H[j])) k = (k + 1) % sz;
			ll cur = cross(H[i], H[k], H[j]);
			if (pre > cur) break;
			pre = cur;
			ans = std::max(ans, pre);
			j = (j + 1) % sz;
		}
	}
	std::cout << (ans >> 1) << (ans & 1 ? ".5000000\n" : ".0000000\n"); return;
}
int main() { solve(); return 0; }//boj18252 The Starry Night