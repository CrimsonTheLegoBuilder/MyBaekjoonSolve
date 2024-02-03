#include <iostream>
#include <algorithm>
#include <cmath>
#include <cstring>
typedef long long ll;
const ll INF = 1e9;
const int LEN = 3e3 + 1;
int N, K;
ll A, B;
ll min_a, min_b;

struct Pos {
	ll x, y;
	Pos(ll X, ll Y) : x(X), y(Y) {}
	Pos() : x(0), y(0) {}
	ll f() const { return A * x + B * y; }
	bool operator < (const Pos& p) const { return f() < p.f(); }
	bool operator == (const Pos& p) const { return x == p.x && y == p.y; }
	//bool operator < (const Pos& p) const { return x == p.x ? y < p.y : x < p.x; }
	Pos operator + (const Pos& p) const { return { x + p.x, y + p.y }; }
	Pos operator - (const Pos& p) const { return { x - p.x, y - p.y }; }
	ll operator * (const Pos& p) const { return { x * p.x + y * p.y }; }
	ll operator / (const Pos& p) const { return { x * p.y - y * p.x }; }
	Pos& operator += (const Pos& p) { x += p.x; y += p.y; return *this; }
	//Pos& operator *= (const ll& scale) { x *= scale; y *= scale; return *this; }
	ll operator ! () const { return x * y; }
} O = { 0, 0 }, MIN, P[LEN];
ll cross(const Pos& d1, const Pos& d2, const Pos& d3) { return (d2 - d1) / (d3 - d2); }
//ll dot(const Pos& d1, const Pos& d2, const Pos& d3) { return (d2 - d1) * (d3 - d2); }
int ccw(const Pos& d1, const Pos& d2, const Pos& d3) {
	ll ret = cross(d1, d2, d3);
	return !ret ? 0 : ret > 0 ? 1 : -1;
}
Pos area(int a, int b) {
	A = a, B = b;
	Pos ret = O;
	std::sort(P, P + N);
	for (int i = 0; i < K; i++) O += P[i];
	return ret;
}
void find(const Pos& s, const Pos& e) {
	Pos d = s - e;
	d.x *= -1;
	Pos m = area(d.y, d.x);
	std::cout << m.x << " " << m.y << "\n";
	if (!m < !MIN) MIN = m, min_a = d.y, min_b = d.x;
	if (ccw(s, e, m) >= 0) return;
	find(s, m); find(m, e);
	return;
}
void solve() {
	std::cin.tie(0)->sync_with_stdio(0);
	std::cout.tie(0);
	std::cin >> N >> K;
	for (int i = 0; i < N; i++)
		std::cin >> P[i].x >> P[i].y;

	Pos s = area(3001, 1);
	Pos e = area(1, 3001);
	if (!s < !e) MIN = s, min_a = 3001, min_b = 1;
	else MIN = e, min_a = 1, min_b = 3001;
	find(s, e);
	std::cout << !MIN;

	return;
}
int main() { solve(); return 0; }//boj5257 - wnwoghd22
