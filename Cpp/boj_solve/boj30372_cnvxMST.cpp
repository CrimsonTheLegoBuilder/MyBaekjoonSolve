#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <algorithm>
#include <cmath>
#include <cstring>
#include <cassert>
#include <vector>
#include <queue>
typedef long long ll;
typedef double ld;
const ll INF = 1e17;
const int LEN = 120'000;
const ld TOL = 1e-7;
const ll MOD = 1e17;
int N, M, T, Q;
bool zero(const ld& x) { return std::abs(x) < TOL; }
int sign(const ld& x) { return x < -TOL ? -1 : x > TOL; }
ll gcd(ll a, ll b) { while (b) { ll tmp = a % b; a = b; b = tmp; } return a; }

int p[LEN];
int find(int i) { return p[i] < 0 ? i : p[i] = find(p[i]); }
int join(int a, int b) {
	a = find(a), b = find(b);
	if (a == b) return 0;
	p[a] += p[b]; p[b] = a;
	return 1;
}

struct Pos {
	int x, y, i;
	Pos(int X = 0, int Y = 0, int I = 0) : x(X), y(Y), i(I) {}
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
} pos[LEN]; const Pos O = Pos(0, 0);
bool cmpx(const Pos& p, const Pos& q) { return p.x == q.x ? p.y < q.y : p.x < q.x; }
bool cmpy(const Pos& p, const Pos& q) { return p.y == q.y ? p.x < q.x : p.y < q.y; }
//bool cmpi(const Pos& p, const Pos& q) { return p.i < q.i; }
typedef std::vector<Pos> Polygon;
ll cross(const Pos& d1, const Pos& d2, const Pos& d3) { return (d2 - d1) / (d3 - d2); }
ll cross(const Pos& d1, const Pos& d2, const Pos& d3, const Pos& d4) { return (d2 - d1) / (d4 - d3); }
ll dot(const Pos& d1, const Pos& d2, const Pos& d3) { return (d2 - d1) * (d3 - d2); }
ll dot(const Pos& d1, const Pos& d2, const Pos& d3, const Pos& d4) { return (d2 - d1) * (d4 - d3); }
int ccw(const Pos& d1, const Pos& d2, const Pos& d3) {
	ll ret = cross(d1, d2, d3);
	return !ret ? 0 : ret > 0 ? 1 : -1;
}
int ccw(const Pos& d1, const Pos& d2, const Pos& d3, const Pos& d4) {
	ll ret = cross(d1, d2, d3, d4);
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
	return H;
}
int target[LEN];
ll target_dist[LEN];
void rotating_calipers(const Polygon& A, const Polygon& B) {
	int sza = A.size();
	int szb = B.size();
	auto r_side = [&](const int& i, const int& t) -> bool {
		int i2 = (i + 1) % sza;
		int t2 = (t + 1) % szb;
		int CCW = ccw(A[i], A[i2], B[t], B[t2]);
		ll DOT = dot(A[i], A[i2], B[t], B[t2]);
		return (CCW > 0) &&
			(A[i] - B[t]).Euc() < (A[i] - B[t2]).Euc();
		};
	auto l_side = [&](const int& i, const int& t) -> bool {
		int i2 = (i + 1) % sza;
		int t2 = (t + 1) % szb;
		int CCW = ccw(A[i], A[i2], B[t], B[t2]);
		ll DOT = dot(A[i], A[i2], B[t], B[t2]);
		return (CCW < 0) &&
			(A[i] - B[t]).Euc() < (A[i] - B[t2]).Euc();
		};

	int s = 0;
	int R = 0, U = 0, L = 0;
	while (cross(A[0], A[1], B[s], B[(s + 1) % szb]) <= 0) s = (s + 1) % szb;
	R = s;
	while (r_side(0, R)) R = (R + 1) % szb;
	for (int k = R; k < szb + s; k++) if (cross(A[0], A[1], B[k % szb], B[(k + 1) % szb]) <= 0) { U = k; break; }
	L = U;
	while (l_side(0, L)) L = (L + 1) % szb;

	for (int i = 0; i < sza; i++) {
		while (r_side(i, R)) R = (R + 1) % szb;
		while (cross(A[i], A[(i + 1) % sza], B[U], B[(U + 1) % szb])) U = (U + 1) % szb;
		while (l_side(i, L)) L = (L + 1) % szb;
		ll RR = (A[i] - B[R]).Euc();
		ll UU = (A[i] - B[U]).Euc();
		ll LL = (A[i] - B[L]).Euc();
		Pos pa = A[i], pb = B[U];
		ll dist = UU;
		if (RR >= UU) pb = B[R], dist = RR;
		if (LL >= UU) pb = B[L], dist = LL;
		if (RR >= LL) pb = B[R], dist = RR;
		//else pb = B[L], dist = LL;
		int ga = find(pa.i);
		int gb = find(pb.i);
		if (dist > target_dist[ga]) {
			target[ga] = gb;
			target_dist[ga] = dist;
		}

		if (dist > target_dist[gb]) {
			target[gb] = ga;
			target_dist[gb] = dist;
		}
	}
	return;
}
void sweep_naive(const std::vector<Pos>& a, const std::vector<Pos>& b) { // N^2
	for (const Pos& pa : a) {
		for (const Pos& pb : b) {
			ll dist = (pa - pb).Euc();
			int ga = find(pa.i);
			int gb = find(pb.i);
			if (dist > target_dist[ga]) {
				target[ga] = gb;
				target_dist[ga] = dist;
			}

			if (dist > target_dist[gb]) {
				target[gb] = ga;
				target_dist[gb] = dist;
			}
		}
	}
}
void sweep(const std::vector<Pos>& a, const std::vector<Pos>& b) { // N ~ N log N
	int sza = a.size();
	int szb = b.size();
	if (sza <= 2 || szb <= 2) {
		//std::cout << "	small_case\n";
		sweep_naive(a, b);
		return;
	}
	//std::cout << "	large case\n";
	rotating_calipers(a, b);
	rotating_calipers(b, a);
	return;
}
struct E {
	int u, v;
	ll d;
	bool operator<(const E& r) const { return d > r.d; }
};

void solve() {
	memset(p, -1, sizeof p);
	int N;
	ll large_part = 0, small_part = 0;
	std::cin >> N;
	for (int i = 0; i < N; ++i) std::cin >> pos[i].x >> pos[i].y, pos[i].i = i;
	int cnt = N;
	while (cnt > 1) {
		//std::cout << "cnt :" << cnt << "\n";
		memset(target, -1, sizeof target);
		memset(target_dist, 0, sizeof target_dist);
		for (int d = 0; d < 20; ++d) {
			std::vector<Pos> a, b;
			for (int j = 0, k; j < N; ++j) {
				k = find(j);
				if (k & 1 << d) a.push_back(pos[j]);
				else b.push_back(pos[j]);
			}
			sweep(a, b);
		}
		std::priority_queue<E> pq;
		for (int i = 0; i < N; ++i)
			if (i == find(i) && ~target[i])
				pq.push({ i, target[i], target_dist[i] });
		//std::cout << "edge found: " << pq.size() << '\n';
		while (pq.size()) {
			E top = pq.top(); pq.pop();
			if (join(top.u, top.v)) {
				small_part += top.d;
				large_part += small_part / MOD;
				small_part %= MOD;
				--cnt;
			}
		}
	}
	if (large_part) {
		printf("%lld%017lld\n", large_part, small_part);
	}
	else printf("%lld\n", small_part);
}

int main() {
	//freopen("../../../input_data/killer_secret/73_circle.in", "r", stdin);
	freopen("../../../input_data/killer_secret/01_killer.in", "r", stdin);
	int T;
	std::cin >> T;
	while (T--) solve();
}