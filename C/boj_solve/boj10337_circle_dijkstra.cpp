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
const ld TOL = 1e-3;
const ld PI = acos(-1);
const ld R = 100.0;
const ld D = 200.0;
const ld para = D * PI;
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
int n = 0;
struct Info {
	int i;
	ld c;
	std::vector<int> path;
	bool operator < (const Info& x) const {
		return c > x.c;
	}
};

std::priority_queue<Info> Q;
std::vector<Info> G[LEN];
ld dijkstra(int v, int e) {
	for (int i = 0; i < LEN; i++) C[i] = MAX;
	Q.push({ v, 0, {v} });
	C[v] = 0;
	while (!Q.empty()) {
		Info p = Q.top(); Q.pop();
		if (p.c > C[p.i]) continue;
		if (p.i == e) {
			for (const int& a : p.path) {
				std::cout << a << " ";
			}
			std::cout << "\n";
		}
		for (const Info& w : G[p.i]) {
			ld cost = p.c + w.c;
			if (C[w.i] > cost) {
				C[w.i] = cost;
				Info tmp = { w.i, cost, };
				for (const int& a : p.path) {
					tmp.path.push_back(a);
				}
				tmp.path.push_back(w.i);
				Q.push(tmp);
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
ld dist(const Pos& d1) { return hypot(d1.x, d1.y); }
ld dist(const Pos& d1, const Pos& d2) {
	return hypot((d1.x - d2.x), (d1.y - d2.y));
}
ld dist(const Pos& d1, const Pos& d2, const Pos& d3) {
	return std::abs(cross(d1, d2, d3) / dist(d1, d2));
}
ld norm(ld theta) {
	while (theta >= 2 * PI) theta -= 2 * PI;
	while (0 > theta) theta += 2 * PI;
	return theta;
}
ld dist(int i, int j) {
	Pos d1 = nodes[i], d2 = nodes[j];
	if (!d1.i || d1.i != d2.i) return dist(d1, d2);
	else if (d1.i == d2.i) {
		Pos p1 = d1, p2 = d2, p = poles[d1.i];
		ld theta1 = atan2(d1.y - p.y, d1.x - p.x);
		ld theta2 = atan2(d2.y - p.y, d2.x - p.x);
		ld theta = std::abs(theta1 - theta2);
		if (theta > PI) theta = 2 * PI - theta;
		return R * theta;
	}
}
bool close(const Pos& d1, const Pos& d2) {
	ld distance = dist(d1, d2);
	//std::cout << d1.i << " " << d2.i << " " << dist(d1, d2) << " " << R + .01 << "\n";
	return distance < R - TOL;
}
bool close(const Pos& d1, const Pos& d2, const Pos& d3) {
	if (between(d1, d3, d2)) {
		//std::cout << d1.i << " " << d2.i << " " << d3.i << " " << dist(d1, d2, d3) << " " << R + .01 << "\n";
		return dist(d1, d2, d3) < R - TOL;
	}
	else {
		//std::cout << close(d1, d3) << " " << dist(d1, d3) << " " << close(d2, d3) << " " << dist(d2, d3) << " " << R + TOL << " close, two circle\n";
		return close(d1, d3) || close(d2, d3);
	}
}
Pos mid(const Pos& d1, const Pos& d2) { return (d1 + d2) * .5; }
ld get_theta(const Pos& d1, const Pos& d2) {
	ld w = dist(d1, d2);
	//std::cout << "w : " << w << "\n";
	return asin(R / w);
}
Pos rotate(const Pos& pivot, const Pos& p, ld theta, int i) {
	Pos v = p - pivot;
	ld ratio = cos(theta);
	ld x = v.x * cos(theta) - v.y * sin(theta);
	ld y = v.x * sin(theta) + v.y * cos(theta);
	//std::cout << v.x << " " << v.y << " " << ratio << " " << ratio * x + p.x << " " << ratio * y + p.y << " " << pivot.i << " " << p.i << " rotate\n";
	return Pos(x, y, i) * ratio + pivot;
}

void connect(int n1, int n2) {
	Pos d1 = nodes[n1], d2 = nodes[n2];
	//std::cout << d1.i << " " << d2.i << " connection\n";
	if (!d1.i || d1.i != d2.i) {
		bool f = 1;
		for (int i = 1; i < N + 1; i++) {
			if (close(d1, d2, poles[i])) return;
		}
		//std::cout << n1 << " " << n2 << " " << (!f ? "dis" : "") << "connected\n";
		G[n1].push_back({ n2, dist(d1, d2) });
		G[n2].push_back({ n1, dist(d1, d2) });
		return;
	}
	else if (d1.i == d2.i) {  // if robot revolves around a poles
		bool f0 = 0, fa = 1, fb = 1;
		Pos p1 = d1, p2 = d2, p = poles[d1.i];
		ld theta1 = norm(atan2(d1.y - p.y, d1.x - p.x));
		ld theta2 = norm(atan2(d2.y - p.y, d2.x - p.x));
		//std::cout << p1.x - p.x << " " << p1.y - p.y << "\n";
		//std::cout << theta1 << " " << theta2 << "\n";
		ld theta = theta1 - theta2;
		//std::cout << theta << "\n";
		if (theta < 0) {
			//std::cout << "swap\n";
			std::swap(p1, p2); }
		bool f1 = 0, f2 = 0, f3 = 0;
		for (int i = 1; i < N + 1; i++) {
			if (p.i == i) continue;
			Pos p1_ = p + (p1 - p) * 2;
			Pos p2_ = p + (p2 - p) * 2;
			//std::cout << p1_.x << " " << p1_.y << " " << p1.x << " " << p1.y << " " << p.x << " " << p.y << " " << poles[i]. x << " " << poles[i].y << "\n";
			//std::cout << p2_.x << " " << p2_.y << " " << p2.x << " " << p2.y << " " << p.x << " " << p.y << " " << poles[i]. x << " " << poles[i].y << "\n";
			f1 = (ccw(p, p1_, poles[i]) != 1) && (ccw(p, p2_, poles[i]) != -1);
			//std::cout << ccw(p, p1_, poles[i]) << " " << ccw(p, p2_, poles[i]) << " ccw\n";
			f2 = dist(p, poles[i]) < D;
			f3 = (dist(p1, poles[i]) < R) || (dist(p2, poles[i]) < R);
			//bool f4 = 1;
			//std::cout << f1 << f2 << f3 << "\n";
			if ((f1 && f2) || f3) { fa = 0; break; }
		}
		if (theta <= PI && fa) {
			ld arc = dist(n1, n2);
			G[n1].push_back({ n2, dist(n1, n2) });
			G[n2].push_back({ n1, dist(n1, n2) });
			return;
		}
		//std::cout << n1 << " " << n2 << " " << " " << ((f1 && f2) || f3 ? "dis" : "") << "connected\n";
		std::swap(p1, p2);
		for (int i = 1; i < N + 1; i++) {
			if (p.i == i) continue;
			Pos p1_ = p + (p1 - p) * 2;
			Pos p2_ = p + (p2 - p) * 2;
			//std::cout << p1_.x << " " << p1_.y << " " << p1.x << " " << p1.y << " " << p.x << " " << p.y << " " << poles[i].x << " " << poles[i].y << "\n";
			//std::cout << p2_.x << " " << p2_.y << " " << p2.x << " " << p2.y << " " << p.x << " " << p.y << " " << poles[i].x << " " << poles[i].y << "\n";
			f1 = (ccw(p, p1_, poles[i]) != 1) && (ccw(p, p2_, poles[i]) != -1);
			//std::cout << ccw(p, p1_, poles[i]) << " " << ccw(p, p2_, poles[i]) << " ccw\n";
			f2 = dist(p, poles[i]) < D;
			f3 = (dist(p1, poles[i]) < R) || (dist(p2, poles[i]) < R);
			//bool f4 = 1;
			//std::cout << f1 << f2 << f3 << "\n";
			if ((f1 && f2) || f3) { fb = 0; break; }
		}
		//std::cout << n1 << " " << n2 << " " << " " << ((f1 && f2) || f3 ? "dis" : "") << "connected\n";
		if (theta <= PI && fb) {
			ld arc = dist(n1, n2);
			G[n1].push_back({ n2, dist(n1, n2) });
			G[n2].push_back({ n1, dist(n1, n2) });
			return;
		}
		if (fa || fb) {
			ld arc = dist(n1, n2);
			G[n1].push_back({ n2, para - dist(n1, n2) });
			G[n2].push_back({ n1, para - dist(n1, n2) });
		}
		return;
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
	//std::cout << "nodes[0] : " << nodes[0].x << " " << nodes[0].y << " " << nodes[0].i << "\n";
	nodes[n++] = E; //goal
	//std::cout << "nodes[1] : " << nodes[1].x << " " << nodes[1].y << " " << nodes[1].i << "\n";
	for (int i = 1; i <= N; i++) {
		std::cin >> poles[i].x >> poles[i].y;
		poles[i].i = i;
	}
	return;
}
void pos_init() {
	for (int i = 1; i < N + 1; i++) {
		Pos p1, p2, p3, p4;
		ld theta1 = get_theta(S, poles[i]);
		ld theta2 = get_theta(E, poles[i]);
		//std::cout << "theta1 : " << theta1 << ", theta2 : " << theta2 << "\n";
		p1 = rotate(S, poles[i], theta1, i);
		p2 = rotate(S, poles[i], -theta1, i);
		p3 = rotate(E, poles[i], theta2, i);
		p4 = rotate(E, poles[i], -theta2, i);
		//std::cout << "p1 : " << p1.x << " " << p1.y << " " << p1.i << "\n";
		//std::cout << "p2 : " << p2.x << " " << p2.y << " " << p2.i << "\n";
		//std::cout << "p3 : " << p3.x << " " << p3.y << " " << p3.i << "\n";
		//std::cout << "p4 : " << p4.x << " " << p4.y << " " << p4.i << " pos_init 1\n";
		nodes[n++] = p1;
		nodes[n++] = p2;
		nodes[n++] = p3;
		nodes[n++] = p4;
	}
	for (int i = 1; i < N + 1; i++) {
		for (int j = i + 1; j < N + 1; j++) {
			if (i == j) continue;
			ld distance = dist(poles[j], poles[i]);
			if (distance > D) {
				Pos p1, p2, p3, p4;
				Pos m = mid(poles[i], poles[j]);
				ld theta = get_theta(m, poles[i]);
				p1 = rotate(m, poles[i], theta, i);
				p2 = rotate(m, poles[j], theta + PI, j);
				p3 = rotate(m, poles[i], -theta, i);
				p4 = rotate(m, poles[j], -theta - PI, j);
				//std::cout << "p1 : " << p1.x << " " << p1.y << " " << p1.i << "\n";
				//std::cout << "p2 : " << p2.x << " " << p2.y << " " << p2.i << "\n";
				//std::cout << "p3 : " << p3.x << " " << p3.y << " " << p3.i << "\n";
				//std::cout << "p4 : " << p4.x << " " << p4.y << " " << p4.i << " pos_init 2\n";
				nodes[n++] = p1;
				nodes[n++] = p2;
				nodes[n++] = p3;
				nodes[n++] = p4;
			}
			Pos p5, p6, p7, p8;
			Pos v = ~(poles[j] - poles[i]);
			ld scale = R / distance;
			p5 = poles[i] + v * scale;
			p6 = poles[j] + v * scale;
			p7 = poles[i] - v * scale;
			p8 = poles[j] - v * scale;
			//std::cout << "p5 : " << p5.x << " " << p5.y << " " << p5.i << " " << dist(poles[i], p5) << "\n";
			//std::cout << "p6 : " << p6.x << " " << p6.y << " " << p6.i << " " << dist(poles[j], p6) << "\n";
			//std::cout << "p7 : " << p7.x << " " << p7.y << " " << p7.i << " " << dist(poles[i], p7) << "\n";
			//std::cout << "p8 : " << p8.x << " " << p8.y << " " << p8.i << " " << dist(poles[j], p8) << " pos_init 3\n";
			nodes[n++] = p5;
			nodes[n++] = p6;
			nodes[n++] = p7;
			nodes[n++] = p8;
		}
	}
	return;
}
void graph_init() {
	std::cout << "size of nodes : " << n << "\n";
	for (int i = 0; i < n; i++) std::cout << "nodes[" << i << "] : " << nodes[i].x << " " << nodes[i].y << " " << nodes[i].i << "\n";
	for (int i = 0; i < n; i++) {
		for (int j = i + 1; j < n; j++) {
			connect(i, j);
		}
	}
	return;
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

/*
1 0 -300
0 -150

1 0 -202
0 -101
*/