#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <algorithm>
#include <vector>
#include <queue>
#include <cmath>
#include <cstring>
typedef long long ll;
typedef long double ld;
const int LEN = 260;
const ld MAX = 1e17;
const ld TOL = 1e-6;//tolerance
const ld R = 100.0;//radius
const ld D = 200.0;//diameter
const ld PI = acos(-1);//pi = 3.14159265...
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
struct Pos {
	ld x, y;
	int pole_i, node_i;
	Pos(ld X, ld Y, int p_i, int n_i = 0) : x(X), y(Y), pole_i(p_i), node_i(n_i) {}
	Pos() : x(0), y(0), pole_i(0), node_i(0) {}
	Pos operator + (const Pos& p) const { return { x + p.x, y + p.y, pole_i, node_i }; }
	Pos operator - (const Pos& p) const { return { x - p.x, y - p.y, pole_i, node_i }; }
	Pos operator * (const ld& n) const { return { x * n, y * n, pole_i, node_i }; }
	Pos operator ~ () const { return { -y, x, pole_i, node_i }; }//rotate PI/2;
	ld operator / (const Pos& p) const { return x * p.y - y * p.x; }//cross product
	bool operator < (const Pos& p) const {//sort CCW
		bool f1 = z(x) ? y > 0 : x > 0;
		bool f2 = z(p.x) ? p.y > 0 : p.x > 0;
		if (f1 != f2) return f1;
		ld ccw = *this / p;
		return ccw > 0;
	}
} S = { 0, 0, 0 }, E, poles[10], nodes[LEN];
std::vector<Pos> rev[10];
int n = 0;//pointer of nodes
Pos mid(const Pos& d1, const Pos& d2) { return (d1 + d2) * .5; }
ld cross(const Pos& d1, const Pos& d2, const Pos& d3) {
	return (d2.x - d1.x) * (d3.y - d2.y) - (d2.y - d1.y) * (d3.x - d2.x);
}
int ccw(const Pos& d1, const Pos& d2, const Pos& d3) {
	ld ret = (d2.x - d1.x) * (d3.y - d2.y) - (d2.y - d1.y) * (d3.x - d2.x);
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
bool close(const Pos& d, const Pos& target) {
	return dist(d, target) < R - TOL;
}
bool close(const Pos& d1, const Pos& d2, const Pos& target) {
	if (between(d1, d2, target)) return dist(d1, d2, target) < R - TOL;
	else return close(d1, target) || close(d2, target);
}
ld get_theta(const Pos& d1, const Pos& d2) {
	ld w = dist(d1, d2);
	return asin(R / w);
}
Pos rotate(const Pos& pivot, const Pos& p, ld theta, int i) {
	Pos v = p - pivot;
	ld ratio = cos(theta);
	ld x = v.x * cos(theta) - v.y * sin(theta);
	ld y = v.x * sin(theta) + v.y * cos(theta);
	return Pos(x, y, i) * ratio + pivot;
}
void connect_node(int n1, int n2) {
	Pos d1 = nodes[n1], d2 = nodes[n2];
	if (!d1.pole_i || d1.pole_i != d2.pole_i) {
		for (int i = 1; i < N + 1; i++) if (close(d1, d2, poles[i])) return;
		G[n1].push_back({ n2, dist(d1, d2) });
		G[n2].push_back({ n1, dist(d1, d2) });
	}
	return;
}
void connect_seg() {
	for (int i = 0; i < n; i++)
		for (int j = i + 1; j < n; j++)
			connect_node(i, j);
	return;
}
ld norm(ld theta) {
	while (theta > 2 * PI + TOL) theta -= 2 * PI;
	while (0 > theta) theta += 2 * PI;
	return theta;
}
void connect_arc() {
	for (int i = 1; i <= N; i++) {
		int sz = rev[i].size();
		for (int j = 0; j < sz; j++) rev[i][j] = rev[i][j] - poles[i];
		std::sort(rev[i].begin(), rev[i].end());
		for (int j = 0; j < sz; j++) rev[i][j] = rev[i][j] + poles[i];
		for (int j = 0; j < sz; j++) {
			Pos cur = rev[i][j], nxt = rev[i][(j + 1) % sz];
			bool f0 = 1;
			for (int k = 1; k <= N; k++) {
				if (i == k) continue;
				bool f1 = ccw(poles[i], cur, poles[k]) > -1; 
				bool f2 = ccw(poles[i], nxt, poles[k]) < 1;
				bool f3 = dist(poles[i], poles[k]) < D;
				bool f4 = dist(cur, poles[k]) < R || dist(nxt, poles[k]) < R;
				if ((f1 && f2 && f3) || f4) { f0 = 0; break; }
			}
			if (f0) {
				Pos p = poles[i];
				ld theta1 = norm(atan2(cur.y - p.y, cur.x - p.x));
				ld theta2 = norm(atan2(nxt.y - p.y, nxt.x - p.x));
				ld theta = norm(theta2 - theta1);
				ld arc = R * theta;
				G[cur.node_i].push_back({ nxt.node_i, arc });
				G[nxt.node_i].push_back({ cur.node_i, arc });
			}
		}
	}
	return;
}
void init() {
	std::cin.tie(0)->sync_with_stdio(0);
	std::cout.tie(0);
	std::cout << std::fixed;
	std::cout.precision(6);
	std::cin >> N >> E.x >> E.y;
	E.pole_i = 0;
	for (int i = 1; i <= N; i++) {
		std::cin >> poles[i].x >> poles[i].y;
		poles[i].pole_i = i;
	}
	return;
}
void pos_init() {
	nodes[n++] = S;
	nodes[n++] = E;
	for (int i = 1; i <= N; i++) {//tangent from S || E
		ld theta1 = get_theta(S, poles[i]);
		ld theta2 = get_theta(E, poles[i]);
		nodes[n++] = rotate(S, poles[i], theta1, i);
		nodes[n++] = rotate(S, poles[i], -theta1, i);
		nodes[n++] = rotate(E, poles[i], theta2, i);
		nodes[n++] = rotate(E, poles[i], -theta2, i);
	}
	for (int i = 1; i <= N; i++) {
		for (int j = i + 1; j <= N; j++) {
			if (i == j) continue;
			ld distance = dist(poles[j], poles[i]);
			Pos v = ~(poles[j] - poles[i]);
			ld scale = R / distance;
			nodes[n++] = poles[i] + v * scale;
			nodes[n++] = poles[j] + v * scale;
			nodes[n++] = poles[i] - v * scale;
			nodes[n++] = poles[j] - v * scale;
			if (distance > D) {//tangent from m
				Pos m = mid(poles[i], poles[j]);
				ld theta = get_theta(m, poles[i]);
				nodes[n++] = rotate(m, poles[i], theta, i);
				nodes[n++] = rotate(m, poles[j], theta + PI, j);
				nodes[n++] = rotate(m, poles[i], -theta, i);
				nodes[n++] = rotate(m, poles[j], -theta - PI, j);
			}
		}
	}
	for (int i = 0; i < n; i++) {
		nodes[i].node_i = i;
		rev[nodes[i].pole_i].push_back(nodes[i]);
	}
	return;
}


//Solve
ld solve() {
	init();
	pos_init();
	connect_seg();
	connect_arc();
	ld ans = dijkstra(0, 1);
	if (ans > 1e16) std::cout << "0.0\n";
	else std::cout << ans << "\n";
	return ans > 1e16 ? 0 : ans;
}
int main() { solve(); return 0; }//boj10337


////Auto judge
//void clear() {
//	for (std::vector<Info>& v : G) v.clear();
//	for (std::vector<Pos>& v : rev) v.clear();
//	n = 0;
//}
//ld solve() {
//	clear();
//	init();
//	pos_init();
//	connect_seg();
//	connect_arc();
//	ld ans = dijkstra(0, 1);
//	return ans > 1e16 ? 0 : ans;
//}
//char inputs[100][100], outputs[100][100];
//int main(int argc, char* argv[]) {//boj10337
//	std::cout << "run\n";
//	std::cout << argc << '\n';
//	std::cout << argv[1] << '\n';
//	std::cout << argv[2] << '\n';
//	freopen(argv[1], "r", stdin);
//	for (int i = 0; i < 100; ++i) std::cin >> inputs[i];
//	
//	freopen(argv[2], "r", stdin);
//	for (int i = 0; i < 100; ++i) std::cin >> outputs[i];
//
//	for (int i = 0; i < 100; ++i) {
//		freopen(inputs[i], "r", stdin);
//		ld ret = solve();
//		freopen(outputs[i], "r", stdin);
//		ld ans;
//		std::cin >> ans;
//		std::cout << (z(ret - ans) ? "AC " : "WC          ");
//		std::cout << i + 1 << "   " << ret << " " << ans << "\n";
//	}
//}
