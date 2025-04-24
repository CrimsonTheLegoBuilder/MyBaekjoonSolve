#include <iostream>
#include <algorithm>
#include <vector>
#include <cmath>
#include <set>
typedef long long ll;
//typedef long double ld;
typedef double ld;
const int LEN = 4e5;
int N, L, cnt{ 0 };
ll x, y;

struct Pos {
	ll x, y;
	bool operator<(const Pos& p) const { return x == p.x ? y < p.y : x < p.x; }
	Pos operator-(const Pos& p) const { return { x - p.x, y - p.y }; }
	ld mag() { return hypot(x, y); }
} P;
std::vector<Pos> C, H;
ll cross(const Pos& d1, const Pos& d2, const Pos& d3) {
	return (d2.x - d1.x) * (d3.y - d2.y) - (d2.y - d1.y) * (d3.x - d2.x);
}
std::vector<Pos> monotone_chain(std::vector<Pos>& C) {
	std::vector<Pos> H;
	std::sort(C.begin(), C.end());
	if (C.size() <= 2) {
		for (const Pos& pos : C) H.push_back(pos);
	}
	else {
		for (int i = 0; i < C.size(); i++) {
			while (H.size() > 1 && cross(H[H.size() - 2], H[H.size() - 1], C[i]) <= 0)
				H.pop_back();
			H.push_back(C[i]);
		}
		H.pop_back();
		int s = H.size() + 1;
		for (int i = C.size() - 1; i >= 0; i--) {
			while (H.size() > s && cross(H[H.size() - 2], H[H.size() - 1], C[i]) <= 0)
				H.pop_back();
			H.push_back(C[i]);
		}
		H.pop_back();
	}
	return H;
}
ld get_round(std::vector<Pos>& H) {
	ld ret = 0;
	int sz = H.size();
	for (int i = 0; i < sz; i++) {
		Pos cur = H[i], nxt = H[(i + 1) % sz];
		ret += (cur - nxt).mag();
	}
	return ret;
}
void solve() {
	std::cin.tie(0)->sync_with_stdio(0);
	std::cout.tie(0);
	std::cout << std::fixed;
	std::cout.precision(7);
	std::cin >> N >> L;
	C.resize(N * 4);
	for (int i = 0; i < N; i++) {
		std::cin >> x >> y;
		C[cnt++] = { x + L, y };
		C[cnt++] = { x, y + L };
		C[cnt++] = { x - L, y };
		C[cnt++] = { x, y - L };
	}
	H = monotone_chain(C);
	ld ans = get_round(H);
	std::cout << ans;
	return;
}
int main() { solve(); return 0; }//boj21624
