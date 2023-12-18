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
const ld PI = acos(-1);
const int SCALE = 1;
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
	Pos operator / (const ld& n) const { return { x / n, y / n }; }
	Pos operator * (const int& n) const { return { x * n, y * n }; }
	Pos operator / (const int& n) const { return { x / n, y / n }; }
	Pos operator ~ () const { return { -y, x }; }//rotate PI/2;
	Pos& operator *= (const ld n) {
		x *= n;
		y *= n;
		return *this;
	}
	Pos& operator /= (const ld n) {
		x /= n;
		y /= n;
		return *this;
	}
	Pos& operator *= (const int n) {
		x *= n;
		y *= n;
		return *this;
	}
	Pos& operator /= (const int n) {
		x /= n;
		y /= n;
		return *this;
	}
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
	ld mag() { return hypot(x, y); }
} S, E, nodes[LEN], zero = { 0 ,0 }; int n = 0;//pointer of nodes
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
		return L == s.L ? R < s.R : L < s.L;
	}
};
void norm(Seg& s) {
	if (s.R < s.L) std::swap(s.L, s.R);
}


struct Circle {
	ld X, Y, R;
	bool operator == (const Circle& c) const {
		return z(X - c.X) && z(Y - c.Y) && z(R - c.R);
	}
	Circle& operator *= (const ld x) {
		X *= x;
		Y *= x;
		R *= x;
		return *this;
	}
	Circle& operator *= (const int x) {
		X *= x;
		Y *= x;
		R *= x;
		return *this;
	}
	void get_f(ld& A, ld& B, ld& C) {
		ld a = -X, b = -Y;
		A = 2 * a;
		B = 2 * b;
		C = R * R - a * a - b * b;
	}
} shelves[30];
int dist_check(const Circle& a, const Circle& b) {
	Pos d1 = { a.X, a.Y }, d2 = { b.X, b.Y };
	ld distance = dist(d1, d2);
	ld radius_sum = a.R + b.R;
	ld diff = std::abs(a.R - b.R);
	if (z(distance - radius_sum) || z(distance - diff)) return 1;
	else if (distance > radius_sum || diff + a.R < b.R || diff + b.R < a.R) return 0;
	else return 2;
}
bool on_seg(const Circle& c, const Seg& s) {
	Pos C = { c.X, c.Y };
	if (between(s.L, s.R, C)) {
		ld distance = dist(s.L, s.R, C);
		return c.R > distance || z(distance - c.R);
	}
	else return (dist(s.L, C) < c.R + TOL || dist(s.R, C) < c.R + TOL);
}
//Pos get_grediant(const Circle& c, const Seg& s) {
//	Pos d1 = s.L, d2 = s.R, C = { c.X, c.Y };
//	Pos vec = ~(d1 - d2);
//	ld theta = atan2(vec.y, vec.x);
//	ld distance = cross(d1, d2, C) / dist(d1, d2);
//	//if (distance < 0) theta += PI;
//	return { distance * cos(theta), distance * sin(theta) };
//}
bool line_sweeping(const Pos& w, const std::vector<Seg>& tmp, int i, int j) {
	int sz = tmp.size();
	//std::cout << sz << "\n";
	//for (const Seg& s : tmp) {
	//	std::cout << s.L.x << " " << s.L.y << " " << s.R.x << " " << s.R.y << "\n";
	//}
	if (!i && j == 1) std::cout << sz << "\n";
	//for (int k = 0; k < sz - 1; k++) {
	//	Seg cur = tmp[k], nxt = tmp[k + 1];
	//	//if (!i && j == 1) std::cout << "a cur : " << cur.R.x << " " << cur.R.y << " nxt : " << nxt.L.x << " " << nxt.L.y << " ";
	//	if (!i && j == 1) std::cout << (nxt.L - cur.R).x << " " << (nxt.L - cur.R).y << "\n";
	//	//if (cur.R < nxt.L) return 0;
	//}
	bool f = 1;
	for (int i = 0; i < sz - 1; i++) {
		Seg cur = tmp[i], nxt = tmp[i + 1];
		if (!i && j == 1) std::cout << "cur : " << cur.R.x << " " << cur.R.y << " nxt : " << nxt.L.x << " " << nxt.L.y << " ";
		if (!i && j == 1) std::cout << (nxt.L - cur.R).x << " " << (nxt.L - cur.R).y << "\n";
		if (w <= nxt.R) f = 1;
		if (!(nxt.L <= cur.R)) f = 0;
		//if (cur.R < nxt.L) return 0;
	}
	return f;
}
bool connect_node(int i, int j) {
	//std::cout << "i : " << nodes[i].x << " " << nodes[i].y << "\n";
	//std::cout << "j : " << nodes[j].x << " " << nodes[j].y << "\n";
	Pos v = nodes[i], w = nodes[j];
	if (z(dist(v, w))) {
		G[i].push_back({ j, 0 });
		G[j].push_back({ i, 0 });
		return 1;
	}
	else {
		std::vector<Seg> tmp;
		bool F1 = 0;
		if (w < v) std::swap(v, w);
		tmp.push_back({ v, v });
		Pos V = w - v;
		//std::cout << "V : " << V.x << " " << V.y << "\n";
		ld distance = V.mag();
		for (int k = 0; k < N; k++) {
			if (!i && j == 1) {
				Circle cur = shelves[k];
				//std::cout << "on_seg " << k << " " << N << " " << on_seg(cur, { v, w }) << "\n";
				if (!on_seg(cur, { v, w })) continue;
				Pos vec = ~V;
				//std::cout << "vec : " << vec.x << " " << vec.y << "\n";
				Pos center = { cur.X, cur.Y };
				ld A = V.y;
				ld B = -V.x;
				ld C = A * v.x + B * v.y;
				//ld d = std::abs(C / hypot(A, B));
				//ld width = std::abs(d - std::abs((A * cur.X + B * cur.Y) / hypot(A, B)));
				ld width = dist(v, w, center);
				Pos w1, w2, W;
				vec *= width;
				//std::cout << "vec : " << vec.x << " " << vec.y << "\n";
				//std::cout << "center : " << center.x << " " << center.y << "\n";
				//std::cout << "width : " << width << "\n";
				Pos t = vec / distance;
				//std::cout << "cross : " << cross(v, w, center) << "\n";
				//std::cout << "distance : " << distance << "\n";
				//std::cout << "width pos : " << t.x << " " << t.y << "\n";
				//std::cout << "t mag " << t.mag() << "\n";
				//std::cout << "vec mag " << vec.mag() << "\n";
				w1 = center + (vec / distance);
				w2 = center - (vec / distance);
				//std::cout << "dot : " << dot(v, w1, center) << " " << dot(v, w2, center) << "\n";
				ld D1 = std::abs(dot(v, w1, center)), D2 = std::abs(dot(v, w2, center));
				//W = !ccw(v, w, w1) ? w1 : w2;
				W = D1 < D2 ? w1 : w2;
				//std::cout << "ccw : " << ccw(v, w, w1) << " " << ccw(v, w, w2) << "\n";
				//std::cout << "cross : " << cross(v, w, w1) << " " << cross(v, w, w2) << "\n";
				//std::cout << "v, w : " << v.x << " " << v.y << " " << w.x << " " << w.y << "\n";
				//std::cout << "w1, w2 : " << w1.x << " " << w1.y << " " << w2.x << " " << w2.y << "\n";
				ld height = std::sqrt(cur.R * cur.R - width * width);
				Pos H = V * (height / distance);
				Pos d1 = W + H;
				Pos d2 = W - H;
				if (d2 < d1) std::swap(d1, d2);
				//std::cout << "bridge : " << v.x << " " << v.y << " " << w.x << " " << w.y << "\n";
				//std::cout << "d1, d2 : " << d1.x << " " << d1.y << " " << d2.x << " " << d2.y << " d1, d2\n";
				if (d1 < v) d1 = v;
				if (w < d2) d2 = w;
				//std::cout << "d1, d2 : " << d1.x << " " << d1.y << " " << d2.x << " " << d2.y << " swapped\n";
				tmp.push_back({ d1, d2 });
			}
			else {

			Circle cur = shelves[k];
			//std::cout << "on_seg " << k << " " << N << " " << on_seg(cur, {v, w}) << "\n";
			if (!on_seg(cur, { v, w })) continue;
			Pos vec = ~V;
			//std::cout << "vec : " << vec.x << " " << vec.y << "\n";
			Pos center = { cur.X, cur.Y };
			ld A = V.y;
			ld B = -V.x;
			ld C = A * v.x + B * v.y;
			//ld d = std::abs(C / hypot(A, B));
			//ld width = std::abs(d - std::abs((A * cur.X + B * cur.Y) / hypot(A, B)));
			ld width = dist(v, w, center);
			Pos w1, w2, W;
			vec *= width;
			//std::cout << "vec : " << vec.x << " " << vec.y << "\n";
			//std::cout << "center : " << center.x << " " << center.y << "\n";
			//std::cout << "width : " << width << "\n";
			Pos t = vec / distance;
			//std::cout << "cross : " << cross(v, w, center) << "\n";
			//std::cout << "distance : " << distance << "\n";
			//std::cout << "width pos : " << t.x << " " << t.y << "\n";
			//std::cout << "t mag " << t.mag() << "\n";
			w1 = center + (vec / distance);
			w2 = center - (vec / distance);
			W = !ccw(v, w, w1) ? w1 : w2;
			//std::cout << "ccw : " << ccw(v, w, w1) << " " << ccw(v, w, w2) << "\n";
			//std::cout << "cross : " << cross(v, w, w1) << " " << cross(v, w, w2) << "\n";
			//std::cout << "v, w : " << v.x << " " << v.y << " " << w.x << " " << w.y << "\n";
			//std::cout << "w1, w2 : " << w1.x << " " << w1.y << " " << w2.x << " " << w2.y << "\n";
			ld height = std::sqrt(cur.R * cur.R - width * width);
			Pos H = V * (height / distance);
			Pos d1 = W + H;
			Pos d2 = W - H;
			if (d2 < d1) std::swap(d1, d2);
			//std::cout << "bridge : " << v.x << " " << v.y << " " << w.x << " " << w.y << "\n";
			//std::cout << "d1, d2 : " << d1.x << " " << d1.y << " " << d2.x << " " << d2.y << " d1, d2\n";
			if (d1 < v) d1 = v;
			if (w < d2) d2 = w;
			//std::cout << "d1, d2 : " << d1.x << " " << d1.y << " " << d2.x << " " << d2.y << " swapped\n";
			tmp.push_back({ d1, d2 });
			}
		}
		std::sort(tmp.begin(), tmp.end());
		bool F2 = line_sweeping(w, tmp, i, j);
		//std::cout << F << "\n";
		if (F2) {
			G[i].push_back({ j, distance });
			G[j].push_back({ i, distance });
			return 1;
		}
		else return 0;
	}
	return 0;
}
void get_node(int i, int j) {
	Circle a = shelves[i], b = shelves[j];
	Pos ci = { a.X, a.Y }, cj = { b.X, b.Y };
	int F = dist_check(a, b);
	if (!F || a == b) return;
	if (F == 1) {
		Pos V = cj - ci;
		ld distance = V.mag();
		ld ratio = a.R / distance;
		Pos d1 = ci + V * ratio;
		nodes[n++] = d1;
		return;
	}
	else if (F == 2) {
		Pos V = cj - ci;
		Pos vec = ~V;
		ld distance = V.mag();
		ld A, B, C, Ai, Bi, Ci, Aj, Bj, Cj;
		a.get_f(Ai, Bi, Ci);
		b.get_f(Aj, Bj, Cj);
		A = Ai - Aj; B = Bi - Bj; C = Ci - Cj;
		//std::cout << A << " " << B << " " << C << "\n";
		ld d = C / hypot(A, B);
		//std::cout << d << " " << (A * a.X + B * a.Y) / hypot(A, B) << "\n";
		ld width = std::abs(d - ((A * a.X + B * a.Y) / hypot(A, B)));
		ld height = std::sqrt(a.R * a.R - width * width);
		//std::cout << width << " " << height << " " << a.R << " " << distance << "\n";
		Pos w = V * (width / distance);
		Pos h = vec * (height / distance);
		Pos d1 = ci + w + h;
		Pos d2 = ci + w - h;
		nodes[n++] = d1;
		nodes[n++] = d2;
		return;
	}
}


//Solve
void init() {
	std::cin.tie(0)->sync_with_stdio(0);
	std::cout.tie(0);
	std::cout << std::fixed;
	std::cout.precision(9);
	std::cin >> S.x >> S.y >> E.x >> E.y >> N;
	for (int i = 0; i < N; i++) {
		std::cin >> shelves[i].X >> shelves[i].Y >> shelves[i].R;
	}
	S *= SCALE;
	E *= SCALE;
	for (int i = 0; i < N; i++) shelves[i] *= SCALE;
	return;
}
void pos_init() {
	nodes[n++] = S, nodes[n++] = E;
	for (int i = 0; i < N; i++)
		for (int j = i + 1; j < N; j++)
			get_node(i, j);
	return;
}
void graph_init() {
	//for (int i = 0; i < n; i++) std::cout << "nodes[" << i << "] : " << nodes[i].x << " " << nodes[i].y << "\n";
	for (int i = 0; i < n; i++)
		for (int j = i + 1; j < n; j++) {
			//if (!i) std::cout << connect_node(i, j) << "\n\n";
			connect_node(i, j);
		}
	return;
}
void solve() {
	init();
	pos_init();
	graph_init();
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