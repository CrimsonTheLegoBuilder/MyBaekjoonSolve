#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <algorithm>
#include <cmath>
#include <cstring>
#include <cassert>
#include <vector>
typedef long long ll;
typedef long double ld;
const ll INF = 1e17;
const int LEN = 250'005;
const ld TOL = 1e-7;
int N, MIN = 1e9, MAX = -1e9;
ll EUC;

struct Pos {
	ll x, y;
	Pos(ll X = 0, ll Y = 0) : x(X), y(Y) {}
	bool operator == (const Pos& p) const { return x == p.x && y == p.y; }
	bool operator != (const Pos& p) const { return x != p.x || y != p.y; }
	bool operator < (const Pos& p) const { return x == p.x ? y < p.y : x < p.x; }
	bool operator <= (const Pos& p) const { return x == p.x ? y <= p.y : x <= p.x; }
	Pos operator + (const Pos& p) const { return { x + p.x, y + p.y }; }
	Pos operator - (const Pos& p) const { return { x - p.x, y - p.y }; }
	Pos operator * (const ll& n) const { return { x * n, y * n }; }
	Pos operator / (const ll& n) const { return { x / n, y / n }; }
	ll operator * (const Pos& p) const { return { x * p.x + y * p.y }; }
	ll operator / (const Pos& p) const { return { x * p.y - y * p.x }; }
	Pos& operator += (const Pos& p) { x += p.x; y += p.y; return *this; }
	Pos& operator -= (const Pos& p) { x -= p.x; y -= p.y; return *this; }
	Pos& operator *= (const ll& scale) { x *= scale; y *= scale; return *this; }
	Pos& operator /= (const ll& scale) { x /= scale; y /= scale; return *this; }
	Pos operator ~ () const { return { -y, x }; }
	ll operator ! () const { return x * y; }
	ll Euc() const { return x * x + y * y; }
	ll Man() const { return std::abs(x) + std::abs(y); }
	ld mag() const { return hypot(x, y); }
	friend std::istream& operator >> (std::istream& is, Pos& p) { is >> p.x >> p.y; return is; }
	friend std::ostream& operator << (std::ostream& os, const Pos& p) { os << p.x << " " << p.y; return os; }
}; const Pos O = { 0, 0 };
std::vector<Pos> C[LEN], H[LEN << 2];//container, merge sort tree
ll cross(const Pos& d1, const Pos& d2, const Pos& d3) { return (d2 - d1) / (d3 - d2); }
ll cross(const Pos& d1, const Pos& d2, const Pos& d3, const Pos& d4) { return (d2 - d1) / (d4 - d3); }
ll dot(const Pos& d1, const Pos& d2, const Pos& d3) { return (d2 - d1) * (d3 - d2); }
ll dot(const Pos& d1, const Pos& d2, const Pos& d3, const Pos& d4) { return (d2 - d1) * (d4 - d3); }
int ccw(const Pos& d1, const Pos& d2, const Pos& d3) {
	ll ret = cross(d1, d2, d3);
	return !ret ? 0 : ret > 0 ? 1 : -1;
}
//std::vector<Pos> graham_scan(std::vector<Pos>& C) {
//	std::vector<Pos> H;
//	if (C.size() < 3) {
//		std::sort(C.begin(), C.end());
//		return C;
//	}
//	std::swap(C[0], *min_element(C.begin(), C.end()));
//	std::sort(C.begin() + 1, C.end(), [&](const Pos& p, const Pos& q) -> bool {
//		int ret = ccw(C[0], p, q);
//		if (!ret) return (C[0] - p).Euc() < (C[0] - q).Euc();
//		return ret > 0;
//		}
//	);
//	C.erase(unique(C.begin(), C.end()), C.end());
//	int sz = C.size();
//	for (int i = 0; i < sz; i++) {
//		while (H.size() >= 2 && ccw(H[H.size() - 2], H.back(), C[i]) <= 0)
//			H.pop_back();
//		H.push_back(C[i]);
//	}
//	return H;
//}
void graham_scan(std::vector<Pos>& C, std::vector<Pos>& H) {
	if (C.size() < 3) {
		std::sort(C.begin(), C.end());
		H = C;
		return;
	}
	std::swap(C[0], *min_element(C.begin(), C.end()));
	std::sort(C.begin() + 1, C.end(), [&](const Pos& p, const Pos& q) -> bool {
		int ret = ccw(C[0], p, q);
		if (!ret) return (C[0] - p).Euc() < (C[0] - q).Euc();
		return ret > 0;
		}
	);
	C.erase(unique(C.begin(), C.end()), C.end());
	int sz = C.size();
	for (int i = 0; i < sz; i++) {
		while (H.size() >= 2 && ccw(H[H.size() - 2], H.back(), C[i]) <= 0)
			H.pop_back();
		H.push_back(C[i]);
	}
	return;
}
ll Minkowski_sum(std::vector<Pos>& A, std::vector<Pos>& B) {
	int sza = A.size();
	int szb = B.size();
	if (!sza || !szb) return -INF;
	ll ret = -INF;

	for (Pos& p : B) p *= -1;

	if (sza <= 2 || szb <= 2) {
		for (const Pos& pa : A)
			for (const Pos& pb : B)
				ret = std::max(ret, (pa + pb).Euc());
		for (Pos& p : B) p *= -1;
		return ret;
	}

	std::vector<Pos> MS;
	int a = 0, b = 0;
	for (int i = 0; i < sza; i++) if (A[i] < A[a]) a = i;
	for (int j = 0; j < szb; j++) if (B[j] < B[b]) b = j;
	MS.push_back(A[a] + B[b]);
	ret = std::max(ret, MS.back().Euc());
	//std::cout << sza + szb << "\n";
	for (int k = 1; k < sza + szb; k++) {
		//std::cout << "DEBUG\n";
		Pos a1 = A[(a + 1) % sza] + B[b];
		Pos b1 = B[(b + 1) % szb] + A[a];
		if (ccw(MS.back(), a1, b1) > 0) MS.push_back(a1), a = (a + 1) % sza;// , std::cout << a1;
		else MS.push_back(b1), b = (b + 1) % szb;// , std::cout << b1;
		ret = std::max(ret, MS.back().Euc());
	}
	for (Pos& p : B) p *= -1;

	return ret;
}
ll search(int l = MIN, int r = MAX, int n = 1) {//divide_conquer
	if (l == r) {
		graham_scan(C[l], H[n]);
		return -INF;
	}
	int m = l + r >> 1;
	ll L = search(l, m, n << 1);
	ll R = search(m + 1, r, n << 1 | 1);
	ll S = Minkowski_sum(H[n << 1], H[n << 1 | 1]);
	std::vector<Pos> tmp;
	for (Pos& p : H[n << 1]) tmp.push_back(p);
	for (Pos& p : H[n << 1 | 1]) tmp.push_back(p);
	graham_scan(tmp, H[n]);
	tmp.clear();
	H[n << 1].clear();
	H[n << 1 | 1].clear();
	return std::max({ L, R, S });
}
void solve() {
	std::cin.tie(0)->sync_with_stdio(0);
	std::cout.tie(0);
	std::cin >> N;
	Pos p;
	int idx;
	MIN = 1e9;
	MAX = -1e9;
	for (int i = 0; i < N; i++) {
		std::cin >> p >> idx;
		assert(idx > 0);
		C[idx].push_back(p);
		MIN = std::min(idx, MIN);
		MAX = std::max(idx, MAX);
	}
	std::cout << search() << "\n";
	return;
}
int main() { solve(); return 0; }//boj25002 Board Game