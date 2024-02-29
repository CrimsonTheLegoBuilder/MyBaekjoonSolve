#include <iostream>
#include <algorithm>
#include <vector>
#include <queue>
#include <cmath>
#include <cstring>
typedef long long ll;
typedef long double ld;
const ld INF = 1e17;
const ld TOL = 1e-7;
const ld PI = acos(-1);
const ld ERAD = 6370;
const int LEN = 800;
int N, M, T, q;
ld R, TH;
bool zero(const ld& x) { return std::abs(x) < TOL; }
//int dcmp(const ld& x) { return std::abs(x) < TOL ? 0 : x > 0 ? 1 : -1; }
ld norm(ld& th) {
	while (th < -TOL) th += PI * 2;
	while (th > PI * 2) th -= PI * 2;
	return th;
}

struct Cost {
	int i;
	ld c;
	Cost(int I = 0, ld C = 0) : i(I), c(C) {}
	bool operator < (const Cost& x) const { return zero(c - x.c) ? i < x.i : c < x.c; }
};
struct Info {
	int i;
	ld c;
	Info(int I = 0, ld C = 0) : i(I), c(C) {}
	bool operator < (const Info& x) const { return zero(c - x.c) ? i < x.i : c > x.c; }
};
ld adj[LEN][LEN], G[30][30], copied[30][30], COST[30];
std::priority_queue<Info> Q;
ld dijkstra(int v, int g) {
	for (int i = 0; i < N; i++) COST[i] = INF;
	Q.push({ v, 0 });
	COST[v] = 0;
	while (Q.size()) {
		Info p = Q.top(); Q.pop();
		if (p.c > COST[p.i]) continue;
		for (int j = 0; j < N; j++) {
			ld w = G[p.i][j];
			if (w > 1e16) continue;
			ld cost = p.c + w;
			if (COST[j] > cost) {
				COST[j] = cost;
				Q.push({ j, cost });
			}
		}
	}
	return COST[g];
}
void floyd_warshall() {
	for (int k = 0; k < N; k++) {
		for (int i = 0; i < N; i++) {
			for (int j = 0; j < N; j++) {
				G[i][j] = std::min(G[i][j], G[i][k] + G[k][j]);
			}
		}
	}
	return;
}

struct Pos3D {
	ld x, y, z;
	Pos3D(ld X = 0, ld Y = 0, ld Z = 0) : x(X), y(Y), z(Z) {}
	bool operator == (const Pos3D& p) const { return zero(x - p.x) && zero(y - p.y) && zero(z - p.z); }
	bool operator != (const Pos3D& p) const { return !zero(x - p.x) || !zero(y - p.y) || !zero(z - p.z); }
	bool operator < (const Pos3D& p) const { return zero(x - p.x) ? zero(y - p.y) ? z < p.z : y < p.y : x < p.x; }
	ld operator * (const Pos3D& p) const { return x * p.x + y * p.y + z * p.z; }
	Pos3D operator / (const Pos3D& p) const {
		Pos3D ret;
		ret.x = y * p.z - z * p.y;
		ret.y = z * p.x - x * p.z;
		ret.z = x * p.y - y * p.x;
		return ret;
	}
	Pos3D operator + (const Pos3D& p) const { return { x + p.x, y + p.y, z + p.z }; }
	Pos3D operator - (const Pos3D& p) const { return { x - p.x, y - p.y, z - p.z }; }
	Pos3D operator * (const ld& scalar) const { return { x * scalar, y * scalar, z * scalar }; }
	Pos3D operator / (const ld& scalar) const { return { x / scalar, y / scalar, z / scalar }; }
	Pos3D& operator += (const Pos3D& p) { x + p.x; y + p.y; z + p.z; return *this; }
	Pos3D& operator *= (const ld& scalar) { x* scalar; y* scalar; z* scalar; return *this; }
	ld Euc() const { return x * x + y * y + z * z; }
	ld mag() const { return sqrtl(Euc()); }
	ld lon() const { return atan2(y, x); }
	ld lat() const { return atan2(z, sqrtl(x * x + y * y)); }
	Pos3D unit() const { return *this / mag(); }
	Pos3D norm(const Pos3D& p) const { return (*this / p).unit(); }
	Pos3D rotate(const ld& th, const Pos3D& axis) const {
		ld SIN = sin(th), COS = cos(th);
		Pos3D norm = axis.unit();
		return norm * (*this * norm) * (1 - COS) + (*this * COS) - *this / norm * SIN;
	}
	friend std::istream& operator >> (std::istream& is, Pos3D& p) {
		is >> p.x >> p.y >> p.z;
		return is;
	}
	friend std::ostream& operator << (std::ostream& os, const Pos3D& p) {
		os << p.x << " " << p.y << " " << p.z << "\n";
		return os;
	}
}; const Pos3D MAXP3D = { INF, INF, INF };
std::vector<Pos3D> pos;
//struct Line3D {
//	Pos3D dir, p0;
//	Line3D(Pos3D DIR = Pos3D(0, 0, 0), Pos3D P0 = Pos3D(0, 0, 0)) : dir(DIR), p0(P0) {}
//};
//struct Plane {
//	ld a, b, c, d;
//	Plane(ld A = 0, ld B = 0, ld C = 0, ld D = 0) : a(A), b(B), c(C), d(D) {}
//	Pos3D norm() const { return Pos3D(a, b, c); };
//	friend std::istream& operator >> (std::istream& is, Plane& f) {
//		is >> f.a >> f.b >> f.c >> f.d;
//		return is;
//	}
//	friend std::ostream& operator << (std::ostream& os, const Plane& f) {
//		os << f.a << " " << f.b << " " << f.c << " " << f.d << "\n";
//		return os;
//	}
//};
Pos3D S2C(const ld& lon, const ld& lat) {//Spherical to Cartesian
	ld phi = lon * PI / 180;
	ld the = lat * PI / 180;
	return Pos3D(cos(phi) * cos(the), sin(phi) * cos(the), sin(the));
}
//int above(const Plane& S, const Pos3D& p) {
//	ld ret = p * S.norm() + S.d;
//	return dcmp(ret);
//}
//Pos3D cross(const Pos3D& d1, const Pos3D& d2, const Pos3D& d3) { return (d2 - d1) / (d3 - d2); }
//ld dot(const Pos3D& d1, const Pos3D& d2, const Pos3D& d3) { return (d2 - d1) * (d3 - d2); }
//int ccw(const Pos3D& d1, const Pos3D& d2, const Pos3D& d3, const Pos3D& norm) {
//	Pos3D CCW = cross(d1, d2, d3);
//	ld ret = CCW * norm;
//	return zero(ret) ? 0 : ret > 0 ? 1 : -1;
//}
//std::vector<Pos3D> circle_intersection(const Pos3D& a, const Pos3D& b, const ld& th) {
//	Pos3D mid = (a + b) * .5;
//	if (zero(mid.mag())) return {};
//	ld x = cos(th) / mid.mag();
//	if (x < -1 || 1 < x) return {};
//	Pos3D w = mid.unit() * x;
//	ld ratio = sqrtl(1 - x * x);
//	Pos3D h = (mid.unit() / (b - a).unit()) * ratio;
//	return { w + h, w - h };
//}
bool circle_intersection(const Pos3D& a, const Pos3D& b, const ld& th, std::vector<Pos3D>& inxs) {
	inxs.clear();
	Pos3D mid = (a + b) * .5;
	if (zero(mid.mag())) return 0;
	ld x = cos(th) / mid.mag();
	if (x < -1 || 1 < x) return 0;
	Pos3D w = mid.unit() * x;
	ld ratio = sqrtl(1 - x * x);
	Pos3D h = (mid.unit() / (b - a).unit()) * ratio;
	inxs.push_back(w + h);
	inxs.push_back(w - h);
	return 1;
}
//std::vector<Pos3D> plane_circle_intersection(const Pos3D& a, const Pos3D& perp, const ld& th) {
//	Pos3D vec = a - (perp * (a * perp));
//	if (zero(vec.mag())) return {};
//	ld x = cos(th) / vec.mag();
//	if (x < -1 || 1 < x) return {};
//	Pos3D w = vec.unit() * x;
//	ld ratio = sqrtl(1 - x * x);
//	Pos3D h = (vec.unit() / perp) * ratio;
//	return { w + h, w - h };
//}
bool plane_circle_intersection(const Pos3D& a, const Pos3D& perp, const ld& th, std::vector<Pos3D>& inxs) {
	inxs.clear();
	Pos3D vec = a - (perp * (perp * a));
	if (zero(vec.mag())) return 0;
	ld x = cos(th) / vec.mag();
	if (x < -1 || 1 < x) return 0;
	Pos3D w = vec.unit() * x;
	ld ratio = sqrtl(1 - x * x);
	Pos3D h = (vec.unit() / perp) * ratio;
	inxs.push_back(w + h);
	inxs.push_back(w - h);
	return 1;
}
Pos3D point(const Pos3D Xaxis, const Pos3D Yaxis, const ld& th) { return Xaxis * cos(th) + Yaxis * sin(th); }
ld angle(const Pos3D Xaxis, const Pos3D Yaxis, const Pos3D& p) {
	ld X = Xaxis * p;
	ld Y = Yaxis * p;
	ld th = atan2(Y, X);
	return norm(th);
}
bool connetable(const std::vector<Pos3D>& P, const Pos3D& a, const Pos3D& b, const ld& th) {
	if (zero((a - b).mag())) return 1;
	Pos3D perp = (a / b).unit();
	Pos3D X = a.unit();//X-axis
	Pos3D Y = (perp / a).unit();//Y-axis
	ld hi = angle(X, Y, b);
	std::vector<ld> tmp = { 0, hi };
	std::vector<Pos3D> inxs;
	for (int i = 0; i < N; i++) {
		if (plane_circle_intersection(P[i], perp, th, inxs)) {
			for (const Pos3D& inx : inxs) {
				ld ang = angle(X, Y, inx);
				if (ang < hi) tmp.push_back(ang);
			}
		}
	}
	std::sort(tmp.begin(), tmp.end());
	int sz = tmp.size();
	bool f = 0;
	for (int i = 0; i < sz - 1; i++) {//sweeping
		Pos3D mid = point(X, Y, (tmp[i] + tmp[(i + 1) % sz]) * .5);
		f = 0;
		for (int j = 0; j < N; j++) {
			if (acos(mid * P[j]) < th + TOL) {
				f = 1;
				break;
			}
		}
		if (!f) return 0;
	}
	return 1;
}
void COPY(bool f = 0) {
	if (!f)
		for (int i = 0; i < N; i++)
			for (int j = 0; j < N; j++)
				copied[i][j] = G[i][j];
	if (f)
		for (int i = 0; i < N; i++)
			for (int j = 0; j < N; j++)
				G[i][j] = copied[i][j];
	return;
}
void query() {
	int s, t;
	ld c;
	std::cin >> s >> t >> c;
	s--; t--;
	COPY();
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++) {
			if (G[i][j] > c + TOL) G[i][j] = INF;
		}
	}
	floyd_warshall();
	ld ans = G[s][t];
	if (ans > 1e16) std::cout << "impossible\n";
	else std::cout << ans << "\n";
	COPY(1);
	return;
}
void solve(const int& tc) {
	std::cout << "Case " << tc << ":\n";
	TH = R / ERAD;
	pos.resize(N);
	ld lon, lat;
	std::vector<Pos3D> inxs;
	for (int i = 0; i < N; i++) std::cin >> lon >> lat, pos[i] = S2C(lon, lat);//to unit
	for (int i = 0; i < N; i++) {
		for (int j = i + 1; j < N; j++) {
			if (circle_intersection(pos[i], pos[j], TH, inxs)) {
				for (const Pos3D& inx : inxs) pos.push_back(inx);
			}
		}
	}
	int sz = pos.size();
	for (int i = 0; i < sz; i++) {
		adj[i][i] = 0;
		for (int j = 0; j < i; j++) {
			if (connetable(pos, pos[i], pos[j], TH)) {
				adj[i][j] = adj[j][i] = ERAD * acos(pos[i] * pos[j]);
			}
			else adj[i][j] = adj[j][i] = INF;
		}
	}
	for (int i = 0; i < N; i++) {
		std::vector<bool> connect(sz, 0);
		std::vector<ld> dist(sz, INF);
		dist[i] = 0;
		while (1) {
			Cost info = { -1, INF };
			for (int j = 0; j < sz; j++)
				if (!connect[j])
					info = std::min(info, Cost(j, dist[j]));

			if (!~info.i) break;

			int nxt = info.i;
			connect[nxt] = 1;
			for (int j = 0; j < sz; j++)
				dist[j] = std::min(dist[j], dist[nxt] + adj[nxt][j]);
		}
		for (int g = 0; g < N; g++) G[i][g] = dist[g];
	}
	std::cin >> q;
	while (q--) query();
	pos.clear();
	return;
}
void solve() {
	std::cin.tie(0)->sync_with_stdio(0);
	std::cout.tie(0);
	std::cout << std::fixed;
	std::cout.precision(3);
	T = 0;
	while (std::cin >> N >> R) solve(++T);
	return;
}
int main() { solve();  return 0; }//boj4212 Shortest Flight Path