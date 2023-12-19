#include <iostream>
#include <algorithm>
#include <vector>
#include <queue>
#include <cmath>
#include <cstring>
typedef double ld;
typedef long long ll;
const int LEN = 700;
const ld MAX = 1e25;
const ld TOL = 1e-6;
const int SCALE = 1000;
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
	Pos(ld X, ld Y) : x(X), y(Y) {}
	Pos() : x(0), y(0) {}
	Pos operator + (const Pos& p) const { return { x + p.x, y + p.y }; }
	Pos operator - (const Pos& p) const { return { x - p.x, y - p.y }; }
	Pos operator * (const ld& n) const { return { x * n, y * n }; }
	Pos operator ~ () const { return { -y, x }; }//rotate PI/2;
	bool operator < (const Pos& p) const {
		return z(x - p.x) ? y < p.y : x < p.x;
	}
	Pos& operator *= (const int scale) {
		x *= scale; y *= scale;
		return *this;
	}
	ld mag() { return hypot(x, y); }
} S, E, nodes[LEN], zero = { 0 ,0 }; int n = 0;//pointer of nodes
struct Circle {
	Pos C;
	ld R;
	Circle& operator *= (const int scale) {
		C *= scale; R *= scale;
		return *this;
	}
} shelves[30];
ld dist(const Pos& d1, const Pos& d2) {
	return hypot((d1.x - d2.x), (d1.y - d2.y));
}
ld func(const Pos& d1, const Pos& d2) {
	return d1.x * d2.x + d1.y * d2.y;
}
bool get_node(int i, int j) {
	Circle a = shelves[i], b = shelves[j];
	Pos ca = a.C, cb = b.C;
	Pos vec = cb - ca;
	ld ra = a.R, rb = b.R;
	ld distance = vec.mag();

	if (distance > ra + rb) return 0;
	if (distance <= std::abs(ra - rb)) return -1;

	ld X = (ra * ra - rb * rb + distance * distance) / (2 * distance);
	ld Y = sqrt(ra * ra - X * X);
	Pos m = ca + vec * (X / distance);
	if (Y > 0) {
		nodes[n++] = m + ~vec * (Y / distance);
		nodes[n++] = m - ~vec * (Y / distance);
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
	//https://math.stackexchange.com/questions/311921/get-location-of-vector-circle-intersection
	for (const Circle& C : shelves) {
		Pos OM = s - C.C;
		ld a = func(vec, vec);
		ld b = 2 * func(vec, OM);
		ld c = func(OM, OM) - C.R * C.R;

		ld J = b * b - 4 * a * c;
		if (J < 0 || z(J)) continue;
		else {
			ld ret1 = (-b + sqrt(J)) / (2 * a);
			ld ret2 = (-b - sqrt(J)) / (2 * a);
			if (ret2 < ret1) std::swap(ret1, ret2);
			tmp.push_back({ ret1, -1 });
			tmp.push_back({ ret2, 1 });
		}
	}

	ld depth = 0;
	bool f = 0;
	std::sort(tmp.begin(), tmp.end());
	for (const Pos& d : tmp) {
		depth -= d.y;
		if (z(d.y)) f = !f;
		if (f && (depth < 0 || z(depth))) return 0;
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

	S *= SCALE; E *= SCALE;
	for (int i = 0; i < N; i++) shelves[i] *= SCALE;

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
	if (ans > 1e24) std::cout << "impossible\n";
	else std::cout << ans / SCALE;
	return;
}
int main() { solve(); return 0; }//boj15004


/*
0 0 1 1
2
0 0 2
2 2 2

0 0 2 0
2
0 0 1
2 0 1

0 0 0 2
2
0 2 1
0 0 1

0 0 6 0
1
3 0 2


0 0 10 0
5
1 0 1
3 0 1
5 0 1
7 0 1
9 0 1

0 0 1 3
3
0 -1 2
4 -1 3
2 3 2
4.269334912857045697

0 0 7 0
2
1 1 2
6 1 2
i

0 0 6 0
2
1 1 2
5 1 2
6.32455532034

*/


//void pos_init() {
//	nodes[n++] = S, nodes[n++] = E;
//	for (int i = 0; i < N; i++)
//		for (int j = i + 1; j < N; j++) get_node(i, j);
//	for (int i = 0; i < n; i++)
//		std::cout << "nodes[" << i << "] : " << nodes[i].x << " " << nodes[i].y << "\n";
//	return;
//}

//struct Pos {
//	ld x, y;
//	Pos(ld X, ld Y) : x(X), y(Y) {}
//	Pos() : x(0), y(0) {}
//	Pos operator + (const Pos& p) const { return { x + p.x, y + p.y }; }
//	Pos operator - (const Pos& p) const { return { x - p.x, y - p.y }; }
//	Pos operator * (const ld& n) const { return { x * n, y * n }; }
//	Pos operator / (const ld& n) const { return { x / n, y / n }; }
//	Pos operator * (const int& n) const { return { x * n, y * n }; }
//	Pos operator / (const int& n) const { return { x / n, y / n }; }
//	Pos operator ~ () const { return { -y, x }; }//rotate PI/2;
//	Pos& operator *= (const ld n) {
//		x *= n; y *= n;
//		return *this;
//	}
//	Pos& operator /= (const ld n) {
//		x /= n; y /= n;
//		return *this;
//	}
//	Pos& operator *= (const int n) {
//		x *= n; y *= n;
//		return *this;
//	}
//	Pos& operator /= (const int n) {
//		x /= n; y /= n;
//		return *this;
//	}
//	bool operator == (const Pos& p) const {
//		return z(x - p.x) && z(y - p.y);
//	}
//	bool operator != (const Pos& p) const {
//		return !z(x - p.x) || !z(y - p.y);
//	}
//	bool operator < (const Pos& p) const {
//		return z(x - p.x) ? y < p.y : x < p.x;
//	}
//	bool operator <= (const Pos& p) const {
//		return (*this < p) || (*this == p);
//	}
//	ld mag() { return hypot(x, y); }
//} S, E, nodes[LEN], zero = { 0 ,0 }; int n = 0;//pointer of nodes

//bool connect_node(int i, int j) {
//	//std::cout << "i : " << nodes[i].x << " " << nodes[i].y << "\n";
//	//std::cout << "j : " << nodes[j].x << " " << nodes[j].y << "\n";
//	Pos v = nodes[i], w = nodes[j];
//	if (z(dist(v, w))) {
//		G[i].push_back({ j, 0 });
//		G[j].push_back({ i, 0 });
//		return 1;
//	}
//	else {
//		std::vector<Seg> tmp;
//		bool F1 = 0;
//		if (w < v) std::swap(v, w);
//		tmp.push_back({ v, v });
//		Pos V = w - v;
//		//std::cout << "V : " << V.x << " " << V.y << "\n";
//		ld distance = V.mag();
//		for (int i = 0; i < N; i++) {
//			Circle cur = shelves[i];
//			//std::cout << "on_seg " << on_seg(cur, {v, w}) << "\n";
//			if (!on_seg(cur, { v, w })) continue;
//			Pos vec = ~V;
//			//std::cout << "vec : " << vec.x << " " << vec.y << "\n";
//			Pos center = { cur.X, cur.Y };
//			ld A = V.y;
//			ld B = -V.x;
//			ld C = A * v.x + B * v.y;
//			//ld d = std::abs(C / hypot(A, B));
//			//ld width = std::abs(d - std::abs((A * cur.X + B * cur.Y) / hypot(A, B)));
//			ld width = dist(v, w, center);
//			Pos w1, w2, W;
//			vec *= width;
//			//std::cout << "vec : " << vec.x << " " << vec.y << "\n";
//			//std::cout << "center : " << center.x << " " << center.y << "\n";
//			//std::cout << "width : " << width << "\n";
//			Pos t = vec / distance;
//			//std::cout << "cross : " << cross(v, w, center) << "\n";
//			//std::cout << "distance : " << distance << "\n";
//			//std::cout << "width pos : " << t.x << " " << t.y << "\n";
//			//std::cout << "mag " << t.mag() << "\n";
//			w1 = center + (vec / distance);
//			w2 = center - (vec / distance);
//			W = !ccw(v, w, w1) ? w1 : w2;
//			//std::cout << "ccw : " << ccw(v, w, w1) << " " << ccw(v, w, w2) << "\n";
//			//std::cout << "cross : " << cross(v, w, w1) << " " << cross(v, w, w2) << "\n";
//			//std::cout << "v, w : " << v.x << " " << v.y << " " << w.x << " " << w.y << "\n";
//			//std::cout << "w1, w2 : " << w1.x << " " << w1.y << " " << w2.x << " " << w2.y << "\n";
//			ld height = std::sqrt(cur.R * cur.R - width * width);
//			Pos H = V * (height / distance);
//			Pos d1 = W + H;
//			Pos d2 = W - H;
//			if (d2 < d1) std::swap(d1, d2);
//			//std::cout << "bridge : " << v.x << " " << v.y << " " << w.x << " " << w.y << "\n";
//			//std::cout << "d1, d2 : " << d1.x << " " << d1.y << " " << d2.x << " " << d2.y << " d1, d2\n";
//			if (d1 < v) d1 = v;
//			if (w < d2) d2 = w;
//			//std::cout << "d1, d2 : " << d1.x << " " << d1.y << " " << d2.x << " " << d2.y << " swapped\n";
//			tmp.push_back({ d1, d2 });
//		}
//		std::sort(tmp.begin(), tmp.end());
//		bool F2 = line_sweeping(w, tmp);
//		//std::cout << F << "\n";
//		if (F2) {
//			G[i].push_back({ j, distance });
//			G[j].push_back({ i, distance });
//			return 1;
//		}
//		else return 0;
//	}
//	return 0;
//}