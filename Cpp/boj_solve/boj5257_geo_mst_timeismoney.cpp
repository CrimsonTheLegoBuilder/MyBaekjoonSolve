#include <iostream>
#include <algorithm>
#include <cmath>
#include <cstring>
typedef long long ll;
const ll INF = 1e9;
const int E_LEN = 1e4;
const int V_LEN = 201;
int N, M;
ll A, B;
ll min_a, min_b;

struct Pos {
	ll x, y;
	Pos(ll X, ll Y) : x(X), y(Y) {}
	Pos() : x(0), y(0) {}
	bool operator == (const Pos& p) const { return x == p.x && y == p.y; }
	bool operator < (const Pos& p) const { return x == p.x ? y < p.y : x < p.x; }
	Pos operator + (const Pos& p) const { return { x + p.x, y + p.y }; }
	Pos operator - (const Pos& p) const { return { x - p.x, y - p.y }; }
	ll operator * (const Pos& p) const { return { x * p.x + y * p.y }; }
	ll operator / (const Pos& p) const { return { x * p.y - y * p.x }; }
	Pos& operator += (const Pos& p) { x += p.x; y += p.y; return *this; }
	//Pos& operator *= (const ll& scale) { x *= scale; y *= scale; return *this; }
	ll operator ! () const { return x * y; }
} O = { 0, 0 }, MIN;
ll cross(const Pos& d1, const Pos& d2, const Pos& d3) { return (d2 - d1) / (d3 - d2); }
//ll dot(const Pos& d1, const Pos& d2, const Pos& d3) { return (d2 - d1) * (d3 - d2); }
int ccw(const Pos& d1, const Pos& d2, const Pos& d3) {
	ll ret = cross(d1, d2, d3);
	return !ret ? 0 : ret > 0 ? 1 : -1;
}

int P[V_LEN];
int find(int i) { return P[i] < 0 ? i : P[i] = find(P[i]); }
int join(int i, int j) {
	i = find(i), j = find(j);
	if (i == j) return 0;
	if (P[i] < P[j]) P[i] += P[j], P[j] = i;
	else P[j] += P[i], P[i] = j;
	return 1;
}
struct Edge {
	ll t, c;
	int u, v;
	ll operator ! () const { return A * t + B * c; }
	bool operator < (const Edge& E) const { return !*this < !E; }
	Pos pos() const { return { t, c }; }
} edges[E_LEN];
Pos mst(ll a, ll b) {
	A = a, B = b;
	Pos ret = O;
	std::sort(edges, edges + M);
	memset(P, -1, sizeof P);
	for (int i = 0, j = 0; i < M && j < N - 1; i++) 
		if (join(edges[i].u, edges[i].v)) 
			ret += edges[i].pos(), j++;
	return ret;
}
void find(const Pos& s, const Pos& e) {
	Pos d = s - e;
	d.x *= -1;
	Pos m = mst(d.y, d.x);
	if (!m < !MIN) MIN = m, min_a = d.y, min_b = d.x;
	if (ccw(s, e, m) >= 0) return;
	find(s, m); find(m, e); 
	return;
}

void solve() {
	std::cin.tie(0)->sync_with_stdio(0);
	std::cout.tie(0);
	std::cin >> N >> M;
	for (int i = 0; i < M; i++) 
		std::cin >> edges[i].u >> edges[i].v >> edges[i].t >> edges[i].c;

	Pos s = mst(256, 1);
	Pos e = mst(1, 256);
	if (!s < !e) MIN = s, min_a = 256, min_b = 1;
	else MIN = e, min_a = 1, min_b = 256;
	find(s, e);
	std::cout << MIN.x << " " << MIN.y << "\n\n";

	A = min_a, B = min_b;
	std::sort(edges, edges + M);
	memset(P, -1, sizeof P);
	for (int i = 0, j = 0; i < M && j < N - 1; i++)
		if (join(edges[i].u, edges[i].v)) 
			std::cout << edges[i].u << " " << edges[i].v << "\n", j++;
	return;
}
int main() { solve(); return 0; }//boj5257 - wnwoghd22
