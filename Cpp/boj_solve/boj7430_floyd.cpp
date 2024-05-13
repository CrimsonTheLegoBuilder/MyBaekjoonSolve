#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <algorithm>
#include <cmath>
#include <cstring>
#include <cassert>
#include <vector>
typedef long long ll;
typedef double ld;
const ll INF = 1e17;
const int LEN = 55;
const ld TOL = 1e-7;
const ll MOD = 1'000'000'007;
bool zero(const ld& x) { return std::abs(x) < TOL; }
int sign(const ld& x) { return x < -TOL ? -1 : x > TOL; }

int N, M, T, Q;
ld G[LEN][LEN];
struct Pos {
	int x, y;
	ll c;
	Pos(int X = 0, int Y = 0, ll C = 0) : x(X), y(Y), c(C) {}
	bool operator == (const Pos& p) const { return x == p.x && y == p.y; }
	bool operator != (const Pos& p) const { return x != p.x || y != p.y; }
	bool operator < (const Pos& p) const { return x == p.x ? y < p.y : x < p.x; }
	bool operator <= (const Pos& p) const { return x == p.x ? y <= p.y : x <= p.x; }
	Pos operator + (const Pos& p) const { return { x + p.x, y + p.y }; }
	Pos operator - (const Pos& p) const { return { x - p.x, y - p.y }; }
	Pos operator * (const int& n) const { return { x * n, y * n }; }
	Pos operator / (const int& n) const { return { x / n, y / n }; }
	ll operator * (const Pos& p) const { return (ll)x * p.x + (ll)y * p.y; }
	ll operator / (const Pos& p) const { return (ll)x * p.y - (ll)y * p.x; }
	Pos operator ^ (const Pos& p) const { return { x * p.x, y * p.y }; }
	Pos& operator += (const Pos& p) { x += p.x; y += p.y; return *this; }
	Pos& operator -= (const Pos& p) { x -= p.x; y -= p.y; return *this; }
	Pos& operator *= (const int& scale) { x *= scale; y *= scale; return *this; }
	Pos& operator /= (const int& scale) { x /= scale; y /= scale; return *this; }
	Pos operator - () const { return { -x, -y }; }
	Pos operator ~ () const { return { -y, x }; }
	Pos operator ! () const { return { y, x }; }
	ll xy() const { return (ll)x * y; }
	ll Euc() const { return (ll)x * x + (ll)y * y; }
	int Man() const { return std::abs(x) + std::abs(y); }
	ld mag() const { return hypot(x, y); }
	ld rad() const { return atan2(y, x); }
	friend ld rad(const Pos& p1, const Pos& p2) { return atan2l(p1 / p2, p1 * p2); }
	int quad() const { return y > 0 || y == 0 && x >= 0; }
	friend bool cmpq(const Pos& a, const Pos& b) { return (a.quad() != b.quad()) ? a.quad() < b.quad() : a / b > 0; }
	friend std::istream& operator >> (std::istream& is, Pos& p) { is >> p.x >> p.y; return is; }
	friend std::ostream& operator << (std::ostream& os, const Pos& p) { os << p.x << " " << p.y; return os; }
}; const Pos O = Pos(0, 0);
typedef std::vector<Pos> Polygon;
ll cross(const Pos& d1, const Pos& d2, const Pos& d3) { return (d2 - d1) / (d3 - d2); }
ll dot(const Pos& d1, const Pos& d2, const Pos& d3) { return (d2 - d1) * (d3 - d2); }
int ccw(const Pos& d1, const Pos& d2, const Pos& d3) {
	ll ret = cross(d1, d2, d3);
	return !ret ? 0 : ret > 0 ? 1 : -1;
}
ll area(std::vector<Pos>& H) {
	ll ret = 0;
	int sz = H.size();
	for (int i = 0; i < sz; i++) {
		Pos cur = H[i], nxt = H[(i + 1) % sz];
		ret += cross(Pos(0, 0), cur, nxt);
	}
	return ret;
}
void norm(std::vector<Pos>& H) { if (area(H) < 0) std::reverse(H.begin(), H.end()); }
std::vector<Pos> lower_monotone_chain(std::vector<Pos>& C) {
	std::vector<Pos> H;
	std::sort(C.begin(), C.end());
	C.erase(unique(C.begin(), C.end()), C.end());
	if (C.size() <= 2) {
		for (const Pos& pos : C)
			H.push_back(pos);
	}
	else {
		for (int i = 0; i < C.size(); i++) {
			while (H.size() > 1 && ccw(H[H.size() - 2], H[H.size() - 1], C[i]) <= 0)
				H.pop_back();
			H.push_back(C[i]);
		}
	}
	return H;
}
std::vector<Pos> upper_monotone_chain(std::vector<Pos>& C) {
	std::vector<Pos> H;
	//std::sort(C.begin(), C.end());
	//C.erase(unique(C.begin(), C.end()), C.end());
	if (C.size() <= 2) {
		for (const Pos& pos : C)
			H.push_back(pos);
		std::reverse(H.begin(), H.end());
	}
	else {
		for (int i = C.size() - 1; i >= 0; i--) {
			while (H.size() > 1 && ccw(H[H.size() - 2], H[H.size() - 1], C[i]) <= 0)
				H.pop_back();
			H.push_back(C[i]);
		}
	}
	return H;
}
Pos ternary_search(const Polygon& H, Pos u, Pos v, bool f = 0) {
	if (v < u) std::swap(u, v);
	if (f) std::swap(u, v);
	int sz = H.size();
	int s = 0, e = sz - 1, r, l;
	while (e - s >= 3) {
		l = (s + s + e) / 3;
		r = (s + e + e) / 3;
		ll LL = cross(u, v, H[l]);
		ll RR = cross(u, v, H[r]);
		if (LL < RR) e = r;
		else s = l;
	}
	ll MIN = INF;
	Pos ret;
	for (int i = s; i <= e; i++) {
		ll tmp = cross(u, v, H[i]);
		if (tmp < MIN) MIN = tmp, ret = H[i];
	}
	return ret;
}
bool all_pos_on_left(const Polygon& LH, const Polygon& UH, const Pos& u, const Pos& v) {
	if (!M) return 1;
	Pos exl = ternary_search(LH, u, v);
	Pos exu = ternary_search(UH, u, v, 1);
	return ccw(u, v, exl) > 0 && ccw(u, v, exu) > 0;
}
void solve() {
	std::cin.tie(0)->sync_with_stdio(0);
	std::cout.tie(0);
	std::cout << std::fixed;
	std::cout.precision(2);
	std::cin >> N >> M;
	Polygon H(N);
	for (Pos& p : H) std::cin >> p;
	norm(H);

	if (!M) {
		ld ret = INF;
		for (int i = 0; i < N; i++) {//brute
			for (int j = i + 1; j < N; j++) {
				for (int k = j + 1; k < N; k++) {
					Pos& I = H[i], J = H[j], K = H[k];
					if (ccw(I, J, K)) {
						Pos v1 = I - J, v2 = J - K, v3 = K - I;
						ld dist = v1.mag() + v2.mag() + v3.mag();
						ret = std::min(ret, dist);
					}
				}
			}
		}
		std::cout << ret << "\n";
		return;
	}

	Polygon mon(M);
	for (Pos& p : mon) std::cin >> p;
	Polygon LH = lower_monotone_chain(mon);
	Polygon UH = upper_monotone_chain(mon);

	for (int i = 0; i < N; i++)
		for (int j = 0; j < N; j++)
			G[i][j] = INF;

	Polygon pair;
	for (int i = 0, j; i < N; i++) {
		for (j = i + 1; j < i + N - 1; j++) {
			if (all_pos_on_left(LH, UH, H[i], H[j % N])) {
				pair.push_back({ i, j % N });
				G[i][j % N] = (H[i] - H[j % N]).mag();
			}
			else break;
		}
	}

	for (int k = 0; k < N; k++) {//floyd_warshall
		for (int i = 0; i < N; i++) {
			for (int j = 0; j < N; j++) {
				G[i][j] = std::min(G[i][j], G[i][k] + G[k][j]);
			}
		}
	}

	ld ret = INF;
	for (const Pos& p : pair) {
		int s = p.x, e = p.y;
		if (!sign(G[s][e] - G[e][s]) &&
			!sign(G[s][e] - (H[s] - H[e]).mag())) 
			continue;
		ret = std::min(ret, G[s][e] + G[e][s]);
	}
	std::cout << ret << "\n";
}
int main() { solve(); return 0; }//boj7430
