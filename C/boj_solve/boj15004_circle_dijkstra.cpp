#include <iostream>
#include <algorithm>
#include <vector>
#include <queue>
#include <cmath>
#include <cstring>
typedef long double ld;
const int LEN = 700;
const ld MAX = 1e19;
const ld TOL = 1e-9;
const ld PI = acos(-1);
int N;


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
ld norm(ld theta) {
	while (theta > 2 * PI + TOL) theta -= 2 * PI;
	while (0 > theta) theta += 2 * PI;
	return theta;
}
struct Pos {
	ld x, y;
	Pos(ld X, ld Y) : x(X), y(Y) {}
	Pos() : x(0), y(0) {}
	Pos operator + (const Pos& p) const { return { x + p.x, y + p.y }; }
	Pos operator - (const Pos& p) const { return { x - p.x, y - p.y }; }
	Pos operator * (const ld& n) const { return { x * n, y * n }; }
	Pos operator ~ () const { return { -y, x }; }//rotate PI/2;
	bool operator == (const Pos& p) const {
		return z(x - p.x) && z(y - p.y);
	}
	bool operator != (const Pos& p) const {
		return !z(x - p.x) || !z(y - p.y);
	}
	bool operator < (const Pos& p) const {
		return z(x - p.x) ? y < p.y : x < p.x;
	}
	bool operator <= (const Pos& p) const {
		return (*this < p) || (*this == p);
	}
	ld meg() { return hypot(x, y); }
} S, E, nodes[LEN]; int n = 0;//pointer of nodes
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
bool between(const Pos& d1, const Pos& d2, const Pos& target) {
	ld dot1 = dot(d1, d2, target), dot2 = dot(d2, d1, target);
	return ((z(dot1) || dot1 < 0) && (z(dot2) || dot2 < 0));
}
ld dist(const Pos& d1, const Pos& d2) {
	return hypot((d1.x - d2.x), (d1.y - d2.y));
}
ld dist(const Pos& d1, const Pos& d2, const Pos& target) {
	return std::abs(cross(d1, d2, target) / dist(d1, d2));
}


struct Seg {
	Pos L, R;
	Seg(Pos l, Pos r) : L(l), R(r) {}
	Seg() : L(), R() {}
	bool operator < (const Seg& s) const {
		return L < s.L;
	}
};
void norm(Seg& s) {
	if (s.R < s.L) std::swap(s.L, s.R);
}
//bool on_seg(const Seg& s, const Pos& target) {
//	Pos d1 = s.L, d2 = s.R;
//	bool f1 = ccw(d1, d2, target) ? 0 : 1;
//	ld dot1 = dot(d1, d2, target), dot2 = dot(d2, d1, target);
//	bool f2 = ((z(dot1) || dot1 < 0) && (z(dot2) || dot2 < 0));
//	return f1 && f2;
//}


struct Circle {
	Pos C;//center
	ld R;//radius
} shelves[30];
int dist_check(const Circle& a, const Circle& b) {
	ld distance = dist(a.C, b.C);
	ld radius_sum = a.R + b.R;
	ld diff = std::abs(a.R - b.R);
	if (z(distance - radius_sum) || z(distance - diff)) return 1;
	else if (distance > radius_sum || diff + a.R < b.R || diff + b.R < a.R) return 0;
	else return 2;
}
//bool close(const Pos& target, const Circle& c) {
//	return dist(c.C, target) < c.R + TOL;
//}
bool on_seg(const Circle& c, const Seg& s) {
	if (between(s.L, s.R, c.C)) {
		ld distance = dist(s.L, s.R, c.C);
		return z(distance - c.R) || c.R > distance;
	}
	else return (dist(s.L, c.C) < c.R + TOL || dist(s.R, c.C) < c.R + TOL);
}
Pos get_grediant(const Circle& c, const Seg& s) {
	Pos d1 = s.L, d2 = s.R;
	Pos vec = ~(d2 - d1);
	ld theta = atan2(vec.y, vec.x);
	ld distance = cross(d1, d2, c.C) / dist(d1, d2);
	//if (distance < 0) theta += PI;
	return { -distance * cos(theta), -distance * sin(theta) };
}
bool line_sweeping(const std::vector<Seg>& tmp) {
	int sz = tmp.size();
	for (int i = 0; i < sz - 1; i++) {
		Seg cur = tmp[i], nxt = tmp[i + 1];
		if (cur.R < nxt.L) return 0;
	}
	return 1;
}
void connect_node(int i, int j) {
	Pos v = nodes[i], w = nodes[j];
	if (z(dist(v, w))) {
		G[i].push_back({ j, 0 });
		G[j].push_back({ i, 0 });
		return;
	}
	else {
		std::vector<Seg> tmp;
		Seg bridge = Seg(v, w);
		Pos d1 = bridge.L, d2 = bridge.R;
		Pos vec = ~(d2 - d1);
		ld theta = atan2(vec.y, vec.x);
		norm(bridge);
		for (int i = 0; i < N; i++) {
			Circle C = shelves[i];
			if (!on_seg(C, bridge)) continue;
			Pos grediant = get_grediant(C, bridge);
			ld width = grediant.meg();
			ld delta = acos(width / C.R);
			ld alpha = norm(theta + delta);
			ld beta = norm(theta - delta);

			Pos d1, d2;
			d1 = { C.C.x + C.R * cos(alpha), C.C.y + C.R * sin(alpha) };
			d2 = { C.C.x + C.R * cos(beta), C.C.y + C.R * sin(beta) };
			if (d2 < d1) std::swap(d1, d2);
			if (d1 < bridge.L) d1 = bridge.L;
			if (bridge.R < d2) d2 = bridge.R;
			tmp.push_back({ d1, d2 });
		}
		std::sort(tmp.begin(), tmp.end());
		if (line_sweeping(tmp)) {
			G[i].push_back({ j, dist(v, w) });
			G[j].push_back({ i, dist(v, w) });
		}
		return;
	}
}
void get_node(int i, int j) {
	Circle a = shelves[i], b = shelves[j];
	int F = dist_check(a, b);
	if (!F) return;
	ld A = a.R * a.R, B = b.R * b.R;
	ld c = dist(a.C, b.C);
	ld C = c * c;
	ld cos_b = (A + C - B) / (2 * a.R * c);
	ld theta = acos(cos_b);
	//std::cout << theta << "\n";
	Pos vec = b.C - a.C;
	ld vx1 = vec.x * cos(theta) - vec.y * sin(theta);
	ld vy1 = vec.x * sin(theta) + vec.y * cos(theta);
	ld vx2 = vec.x * cos(-theta) - vec.y * sin(-theta);
	ld vy2 = vec.x * sin(-theta) + vec.y * cos(-theta);
	ld ratio = a.R / c;
	Pos d1 = { a.C.x + vx1 * ratio, a.C.y + vy1 * ratio };
	Pos d2 = { a.C.x + vx2 * ratio, a.C.y + vy2 * ratio };
	if (F == 1) nodes[n++] = d1;
	if (F == 2) nodes[n++] = d1, nodes[n++] = d2;
	return;
}


//Solve
void init() {
	std::cin.tie(0)->sync_with_stdio(0);
	std::cout.tie(0);
	std::cout << std::fixed;
	std::cout.precision(9);
	std::cin >> S.x >> S.y >> E.x >> E.y >> N;
	for (int i = 0; i < N; i++) {
		std::cin >> shelves[i].C.x >> shelves[i].C.y >> shelves[i].R;
	}
	return;
}
void pos_init() {
	nodes[n++] = S, nodes[n++] = E;
	for (int i = 0; i < N; i++) {
		for (int j = i + 1; j < N; j++) {
			get_node(i, j);
		}
	}
	return;
}
void graph_init() {
	for (int i = 0; i < n; i++) {
		for (int j = i + 1; j < n; j++) {
			connect_node(i, j);
		}
	}
	return;
}
void solve() {
	init();
	pos_init();
	graph_init();
	ld ans = dijkstra(0, 1);
	if (ans > 1e18) std::cout << "impossible\n";
	else std::cout << ans;
	return;
}
int main() { solve(); return 0; }//boj15004


//ld operator / (const Pos& p) const { return x * p.y - y * p.x; }//cross product
//bool operator < (const Pos& p) const {//sort CCW
//	bool f1 = z(x) ? y > 0 : x > 0;
//	bool f2 = z(p.x) ? p.y > 0 : p.x > 0;
//	if (f1 != f2) return f1;
//	ld ccw = *this / p;
//	return ccw > 0;
//}
//Pos mid(const Pos& d1, const Pos& d2) { return (d1 + d2) * .5; }