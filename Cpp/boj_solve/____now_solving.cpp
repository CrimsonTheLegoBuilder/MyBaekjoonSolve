#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <algorithm>
#include <cmath>
#include <vector>
#include <map>
typedef long long ll;
//typedef long double ld;
typedef double ld;
const ld TOL = 1e-6;
inline int sign(const ld& x) { return x < -TOL ? -1 : x > TOL; }
inline bool zero(const ld& x) { return !sign(x); }
struct C {
	bool operator()(const ld& a, const ld& b) const { return sign(b - a) > 0; }
};

int L, N, X, xl, xr;
struct Pos { int x, y; };
std::vector<Pos> P;
std::vector<ld> D, D2;
std::map<ld, int, C> M, R;
void solve() {
	std::cin.tie(0)->sync_with_stdio(0);
	std::cout.tie(0);
	std::cout << std::fixed;
	std::cout.precision(15);
	std::cin >> L >> N >> X;
	xl = -X, xr = L - X + L;
	P.resize(N); for (Pos& p : P) std::cin >> p.x >> p.y;
	for (Pos& p : P) {
		int dx = p.x - X;
		int dy = p.y;
		if (!zero(dx)) {
			ld d = 1. * dy / dx;
			D.push_back(d);
			if (M.find(d) == M.end()) M[d] = 1;
			else M[d]++;
		}
		for (const ld& x : { xl, xr }) {
			dx = p.x - x;
			dy = p.y;
			if (!zero(dx)) {
				ld d = 1. * dy / dx;
				D.push_back(d);
				if (R.find(d) == R.end()) R[d] = 1;
				else R[d]++;
			}
		}
	}
	std::sort(D.begin(), D.end());
	D.erase(unique(D.begin(), D.end()), D.end());
	int ret = 0;
	for (const ld& d : D) {
		int c = 0;
		auto it0 = M.find(d);
		if (it0 != M.end()) c += it0->second;
		int dx = d > 0 ? (L - X + L) : (X + L);
		auto it1 = R.find(-d);
		if (it1 != R.end()) c += it1->second;
		int dx = d > 0 ? (L - X + L) : (X + L);
		ld dy = std::abs(dx * d);
		dx = d > 0 ? -X : L - X;
		ld r = dy / dx;
		auto it2 = M.find(r);
		if (it2 != M.end()) c += it2->second;
		ret = std::max(ret, c);
	}
	std::cout << ret << "\n";
	return;
}
int main() { solve(); return 0; }//boj7273

/*

500000 5
200000
100000 100000
100000 400000
400000 200000
300000 300000
200000 600000

*/

//7391 29675 29688

//#define _CRT_SECURE_NO_WARNINGS
//#include <iostream>
//#include <algorithm>
//#include <vector>
//#include <cmath>
//#include <cstring>
//#include <cassert>
//typedef long long ll;
////typedef long double ld;
//typedef double ld;
//
//int X, Y, N, W, D;
//std::string s;
//int main() {
//	std::cin.tie(0)->sync_with_stdio(0);
//	std::cout.tie(0);
//	std::cout << std::fixed;
//	std::cout.precision(10);
//	std::cin >> X >> Y >> N;
//	while (N--) {
//		std::cin >> W;
//		D = W >= 1000 ? W - 1000 : 0;
//		std::cout << W << " " << std::min(1000, W) * X + D * Y << "\n";
//	}
//	return 0;
//}
