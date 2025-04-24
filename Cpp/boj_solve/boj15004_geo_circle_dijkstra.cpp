#include <iostream>
#include <algorithm>
#include <vector>
#include <queue>
#include <cmath>
#include <cstring>
typedef double ld;
typedef long long ll;
const int LEN = 605;
const ld MAX = 1e17;
const ld TOL = 1e-7;
int N;
//cki86201 thx!
//https://math.stackexchange.com/questions/311921/get-location-of-vector-circle-intersection

//Graph
struct Info {
	int i;
	ld c;
	bool operator < (const Info& x) const { return c > x.c; }
};
std::priority_queue<Info> Q;
std::vector<Info> G[LEN];
ld dijkstra(int v, int e) {
	ld C[LEN];
	for (int i = 0; i < LEN; i++) C[i] = MAX;
	Q.push({ v, 0 });
	C[v] = 0;
	while (!Q.empty()) {
		Info p = Q.top(); Q.pop();
		if (p.c > C[p.i]) continue;
		for (const Info& w : G[p.i]) {
			ld cost = p.c + w.c;
			if (C[w.i] > cost) {
				C[w.i] = cost;
				Info tmp = { w.i, cost };
				Q.push(tmp);
			}
		}
	}
	return C[e];
}

//Geometry
bool z(const ld& x) { return std::abs(x) < TOL; }
struct Pos {
	ld x, y;
	Pos(ld X, ld Y) : x(X), y(Y) {}
	Pos() : x(0), y(0) {}
	Pos operator + (const Pos& p) const { return { x + p.x, y + p.y }; }
	Pos operator - (const Pos& p) const { return { x - p.x, y - p.y }; }
	Pos operator * (const ld& n) const { return { x * n, y * n }; }
	ld operator * (const Pos& p) const { return x * p.x + y * p.y; }//dot
	ld operator / (const Pos& p) const { return x * p.y - y * p.x; }//cross
	Pos operator ~ () const { return { -y, x }; }//rotate PI/2;
	bool operator < (const Pos& p) const { return z(x - p.x) ? y < p.y : x < p.x; }
	ld mag() { return hypot(x, y); }
} S, E, nodes[LEN]; int n = 0;//pointer of nodes
struct Circle {
	Pos C;
	ld R;
	Circle(ld X, ld Y, ld r) : C(Pos(X, Y)), R(r) {}
	Circle() : C(Pos(0, 0)), R(0) {}
} shelves[30];
ld dist(const Pos& d1, const Pos& d2) {
	return hypot((d1.x - d2.x), (d1.y - d2.y));
}
bool get_node(int i, int j) {
	Circle a = shelves[i], b = shelves[j];
	Pos ca = a.C, cb = b.C;
	Pos vec = cb - ca;
	ld ra = a.R, rb = b.R;
	ld distance = vec.mag();

	if (distance > ra + rb) return 0;
	if (distance < std::abs(ra - rb) + TOL) return -1;

	//2nd hyprblc law of cos
	ld X = (ra * ra - rb * rb + distance * distance) / (2 * distance);
	ld Y = sqrt(ra * ra - X * X);
	Pos m = ca + vec * (X / distance);
	if (Y > 0) {
		ld ratio = Y / distance;
		nodes[n++] = m + ~vec * ratio;
		nodes[n++] = m - ~vec * ratio;
		return 2;
	}
	else {
		nodes[n++] = m;
		return 1;
	}
}
bool connect(int i, int j) {
	Pos s = nodes[i], e = nodes[j];
	Pos vec = e - s;
	std::vector<Pos> tmp;
	tmp.push_back({ 0, 0 });
	tmp.push_back({ 1, 0 });

	for (const Circle& C : shelves) {
		Pos OM = s - C.C;
		ld a = vec * vec;
		ld b = 2 * (vec * OM);
		ld c = OM * OM - C.R * C.R;

		ld J = b * b - 4 * a * c;
		if (J < TOL) continue;
		else {
			ld ret1 = (-b + sqrt(J)) / (2 * a);
			ld ret2 = (-b - sqrt(J)) / (2 * a);
			if (ret2 < ret1) std::swap(ret1, ret2);
			tmp.push_back({ ret1, -1 });
			tmp.push_back({ ret2, 1 });
		}
	}

	ld toggle = 0;
	bool f = 0;
	std::sort(tmp.begin(), tmp.end());
	for (const Pos& d : tmp) {
		toggle -= d.y;
		if (z(d.y)) f = !f;
		if (f && toggle < TOL) return 0;
	}

	ld cost = dist(nodes[i], nodes[j]);
	G[i].push_back({ j, cost });
	G[j].push_back({ i, cost });
	return 1;
}

//Solve
void init() {
	std::cin.tie(0)->sync_with_stdio(0);
	std::cout.tie(0);
	std::cout << std::fixed;
	std::cout.precision(9);
	std::cin >> S.x >> S.y >> E.x >> E.y >> N;
	for (int i = 0; i < N; i++)
		std::cin >> shelves[i].C.x >> shelves[i].C.y >> shelves[i].R;

	n = 0;
	nodes[n++] = S, nodes[n++] = E;
	for (int i = 0; i < N; i++)
		for (int j = i + 1; j < N; j++)
			get_node(i, j);

	for (int i = 0; i < n; i++)
		for (int j = i + 1; j < n; j++)
			connect(i, j);
	return;
}
void solve() {
	init();
	ld ans = dijkstra(0, 1);
	if (ans > 1e16) std::cout << "impossible\n";
	else std::cout << ans;
	return;
}
int main() { solve(); return 0; }//boj15004
