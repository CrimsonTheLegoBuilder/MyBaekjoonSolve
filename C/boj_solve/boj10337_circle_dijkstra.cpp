#include <iostream>
#include <algorithm>
#include <vector>
#include <queue>
#include <cmath>
#include <cstring>
typedef long long ll;
typedef long double ld;
const int LEN = 300;
const ld MAX = 1e17;
const ld TOL = 1e-7;
const ld PI = acos(-1);
const ld R = 100.0;
const ld D = 200.0;
int N;
ld C[LEN];

bool z(const ld& x) { return std::abs(x) < TOL; }
struct Pos {
	ld x, y;
	int i;
	Pos(ld X, ld Y, int I) : x(X), y(Y), i(I) {}
	Pos() : x(0), y(0), i(0) {}
	Pos operator + (const Pos& p) const { return { x + p.x, y + p.y, i }; }
	Pos operator - (const Pos& p) const { return { x - p.x, y - p.y, i }; }
	Pos operator * (const ld& n) const { return { x * n, y * n, i }; }
	ld operator / (const Pos& p) const { return x * p.y - y * p.x; }//cross
	Pos operator ~ () const { return { -y, x, i }; }//rotate 90;
} S = { 0, 0, 0 }, E, poles[10], nodes[LEN];
int n = -1;
struct Info {
	int i;
	ld c;
	bool operator < (const Info& x) const {
		return c > x.c;
	}
};

std::priority_queue<Info> Q;
std::vector<Info> G[LEN];
ld dijkstra(int v, int e) {
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
				Q.push({ w.i, cost });
			}
		}
	}
	return C[e];
}

ld cross(const Pos& d1, const Pos& d2, const Pos& d3) {
	return (d2.x - d1.x) * (d3.y - d2.y) - (d2.y - d1.y) * (d3.x - d2.x);
}
int ccw(const Pos& d1, const Pos& d2, const Pos& d3) {
	ld ret = cross(d1, d2, d3);
	return z(ret) ? 0 : ret > 0 ? 1 : -1;
}
ld dot(const Pos& d1, const Pos& d2, const Pos& d3) {
	return (d2.x - d1.x) * (d3.x - d2.x) + (d2.y - d1.y) * (d3.y - d2.y);
}
bool between(const Pos& d1, const Pos& d2, const Pos& d3) {
	ld ret = dot(d1, d2, d3);
	if (z(ret) || ret > 0) return 1;
	return 0;
}
ld dist(const Pos& d1, const Pos& d2) {
	return hypot((d1.x - d2.x), (d1.y - d2.y));
}
ld dist(const Pos& d1, const Pos& d2, const Pos& d3) {
	return std::abs(cross(d1, d2, d3) / dist(d1, d2));
}
bool close(const Pos& d1, const Pos& d2) {
	ld distance = dist(d1, d2);
	//if (distance > D) return 0;
	//return 1;
	return distance < D + TOL;
}
bool close(const Pos& d1, const Pos& d2, const Pos& d3) {
	if (between(d1, d3, d2)) 
		return dist(d1, d2, d3) < D + TOL;
	else 
		return close(d1, d3) || close(d2, d3);
}
Pos mid(const Pos& d1, const Pos& d2) { return (d1 + d2) * .5; }
ld get_theta(ld c, ld b) { return asin(b / c); }
ld get_theta(const Pos& d1, const Pos& d2) {
	ld w = dist(d1, d2); return get_theta(R, w);
}
ld arc(ld theta) { return R * theta; }
ld norm(ld theta) {
	while (theta >= PI) theta -= PI;
	while (0 > theta) theta += PI;
	return theta;
}
Pos rotate(const Pos& p, const Pos& v, ld theta, int i) {
	ld x = v.x * cos(theta) - v.y * sin(theta);
	ld y = v.x * sin(theta) + v.y * cos(theta);
	return p + Pos(x, y, i);
}
bool is_connected(const Pos& d1, const Pos& d2) {
	bool f = 0;
	if (!d1.i || d1.i != d2.i) {
		for (int i = 1; i <= N; i++) {
			if (close(d1, d2, poles[i])) return 0;
		}
		return 1;
	}
	else if (d1.i == d2.i) {
		Pos p1 = d1, p2 = d2, p = poles[d1.i];
		ld theta1 = atan2(d1.y - p.y, d1.x - p.x);
		ld theta2 = atan2(d2.y - p.y, d2.x - p.x);
		ld theta = norm(theta1 - theta2);
		if (theta > PI) std::swap(p1, p2);
		for (int i = 1; i <= N; i++) {
			if (p.i == i) continue;
			bool f1 = between(p, poles[i], p1) && between(p, poles[i], p2);
			bool f2 = (ccw(p, p1, poles[i]) != 1) && (ccw(p, p2, poles[i]) != -1);
			bool f3 = !(dist(p, p1, poles[i]) > R) && !(dist(p, p2, poles[i]) > R);
			bool f4 = !(dist(p1, poles[i]) > D) || !(dist(p2, poles[i]) > D);
			if ((f1 && f2 && f3) || f4) return 0;
		}
		return 1;
	}
}


void init() {
	std::cin.tie(0)->sync_with_stdio(0);
	std::cout.tie(0);
	std::cout << std::fixed;
	std::cout.precision(6);
	std::cin >> N >> E.x >> E.y;
	E.i = 0;
	nodes[n++] = S; //start
	nodes[n++] = E; //goal
	for (int i = 1; i <= N; i++) {
		std::cin >> poles[i].x >> poles[i].y;
		poles[i].i = i;
	}
	return;
}
void pos_init() {
	for (int i = 1; i <= N; i++) {
		Pos p1, p2, p3, p4;
		ld theta = get_theta(S, poles[i]);
		p1 = rotate(S, poles[i] - S, theta, i);
		nodes[n++] = p1;
		p2 = rotate(S, poles[i] - S, -theta, i);
		nodes[n++] = p2;
		p3 = rotate(E, poles[i] - E, theta, i);
		nodes[n++] = p3;
		p4 = rotate(E, poles[i] - E, -theta, i);
		nodes[n++] = p4;
	}
	for (int i = 1; i <= N; i++) {
		for (int j = i + 1; j <= N; j++) {
			if (i == j) continue;
			Pos p1, p2, p3, p4;
			Pos p5, p6, p7, p8;
			Pos m = mid(poles[i], poles[j]);
			ld theta = get_theta(m, poles[i]);
			p1 = rotate(m, poles[i] - m, theta, i);
			nodes[n++] = p1;
			p2 = rotate(m, poles[j] - m, theta + PI, i);
			nodes[n++] = p2;
			p3 = rotate(m, poles[i] - m, -theta, i);
			nodes[n++] = p3;
			p4 = rotate(m, poles[j] - m, -theta - PI, i);
			nodes[n++] = p4;

		}
	}
	return;
}
void graph_init() {

}
void solve() {
	init();
	pos_init();
	graph_init();
	ld ans = dijkstra(0, 1);
	if (ans > 1e16) std::cout << "0.0\n";
	else std::cout << ans << '\n';
	return;
}
int main() { solve(); return 0; }