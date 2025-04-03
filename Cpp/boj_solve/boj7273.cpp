//#define _CRT_SECURE_NO_WARNINGS
//#include <iostream>
//#include <algorithm>
//#include <cmath>
//#include <vector>
//#include <map>
//typedef long long ll;
////typedef long double ld;
//typedef double ld;
//const ld TOL = 1e-6;
//inline int sign(const ld& x) { return x < -TOL ? -1 : x > TOL; }
//inline bool zero(const ld& x) { return !sign(x); }
//struct C {
//	bool operator()(const ld& a, const ld& b) const { return sign(b - a) > 0; }
//};
//
//int L, N, X, xl, xr;
//struct Pos { int x, y; };
//std::vector<Pos> P;
//std::vector<ld> D, D2;
//std::map<ld, int, C> M, R;
//void solve() {
//	std::cin.tie(0)->sync_with_stdio(0);
//	std::cout.tie(0);
//	std::cout << std::fixed;
//	std::cout.precision(15);
//	std::cin >> L >> N >> X;
//	xl = -X, xr = L - X + L;
//	P.resize(N); for (Pos& p : P) std::cin >> p.x >> p.y;
//	for (Pos& p : P) {
//		int dx = p.x - X;
//		int dy = p.y;
//		if (!zero(dx)) {
//			ld d = 1. * dy / dx;
//			D.push_back(d);
//			if (M.find(d) == M.end()) M[d] = 1;
//			else M[d]++;
//		}
//		for (const ld& x : { xl, xr }) {
//			dx = p.x - x;
//			dy = p.y;
//			if (!zero(dx)) {
//				ld d = 1. * dy / dx;
//				D2.push_back(d);
//				if (R.find(d) == R.end()) R[d] = 1;
//				else R[d]++;
//			}
//		}
//	}
//	std::sort(D.begin(), D.end());
//	D.erase(unique(D.begin(), D.end()), D.end());
//	std::sort(D2.begin(), D2.end());
//	D2.erase(unique(D2.begin(), D2.end()), D2.end());
//	int ret = 0;
//	for (const ld& d : D) {
//		int c = M[d];
//		auto it = R.find(-d);
//		if (it != R.end()) c += it->second;
//		int dx = d > 0 ? (L - X + L) : (X + L);
//		ld dy = std::abs(dx * d);
//		dx = d > 0 ? -X : L - X;
//		ld r = dy / dx;
//		auto it2 = M.find(r);
//		if (it2 != M.end()) c += it2->second;
//		ret = std::max(ret, c);
//	}
//	for (ld d : D2) {
//		int c = R[d];
//		auto it = M.find(-d);
//		if (it != M.end()) c += it->second;
//		d *= -1;
//		int dx = d > 0 ? (L - X + L) : (X + L);
//		ld dy = std::abs(dx * d);
//		dx = d > 0 ? -X : L - X;
//		ld r = dy / dx;
//		auto it2 = M.find(r);
//		if (it2 != M.end()) c += it2->second;
//		ret = std::max(ret, c);
//	}
//	std::cout << ret << "\n";
//	return;
//}
//int main() { solve(); return 0; }//boj7273

#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <algorithm>
#include <cmath>
#include <vector>
#include <map>
#include <unordered_map>
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
//std::map<ld, int, C> M, R;
struct Hash {
	size_t operator()(ld x) const {
		return std::hash<ll>()((ll)(x * 10000000));
	}
};
struct Equal {
	bool operator()(ld a, ld b) const {
		return !sign(a - b);
	}
};
std::unordered_map<ld, int, Hash, Equal> M, R;
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