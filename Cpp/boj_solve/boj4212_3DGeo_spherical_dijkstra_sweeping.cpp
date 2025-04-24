#include <iostream>
#include <algorithm>
#include <vector>
#include <queue>
#include <cmath>
#include <cstring>
typedef long long ll;
//typedef long double ld;
typedef double ld;
const ld INF = 1e17;
const ld TOL = 1e-7;
const ld PI = acos(-1);
const ld ERAD = 6370;
const int LEN = 600 + 25;//nC2(25) + 25
bool zero(const ld& x) { return std::abs(x) < TOL; }
ld norm(ld& th) {
	while (th < -TOL) th += PI * 2;
	while (th > PI * 2) th -= PI * 2;
	return th;
}

int N, T, q;
ld R, TH;
ld adj[LEN][LEN], COST[LEN], G[25][25];
struct Info {
	int i;
	ld c;
	Info(int I = 0, ld C = 0) : i(I), c(C) {}
	bool operator < (const Info& x) const { return zero(c - x.c) ? i < x.i : c > x.c; }
};
std::priority_queue<Info> Q;
void dijkstra_adj(const int& v, const int& sz, const int& n = N) {
	for (int i = 0; i < sz; i++) COST[i] = INF;
	Q.push({ v, 0 });
	COST[v] = 0;
	while (Q.size()) {
		Info p = Q.top(); Q.pop();
		if (p.c > COST[p.i]) continue;
		for (int i = 0; i < sz; i++) {
			ld w = adj[p.i][i];
			if (w > INF - 1) continue;
			ld cost = p.c + w;
			if (COST[i] > cost) {
				COST[i] = cost;
				Q.push({ i, cost });
			}
		}
	}
	for (int g = 0; g < n; g++) G[v][g] = COST[g];
	return;
}
ld dijkstra(const int& v, const int& g, const int& sz, const ld& limit) {
	for (int i = 0; i < sz; i++) COST[i] = INF;
	Q.push({ v, 0 });
	COST[v] = 0;
	while (Q.size()) {
		Info p = Q.top(); Q.pop();
		if (p.c > COST[p.i]) continue;
		for (int i = 0; i < sz; i++) {
			ld w = G[p.i][i];
			if (w > limit) continue;
			ld cost = p.c + w;
			if (COST[i] > cost) {
				COST[i] = cost;
				Q.push({ i, cost });
			}
		}
	}
	return COST[g];
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
Pos3D S2C(const ld& lon, const ld& lat) {//Spherical to Cartesian
	ld phi = lon * PI / 180;
	ld the = lat * PI / 180;
	return Pos3D(cos(phi) * cos(the), sin(phi) * cos(the), sin(the));
}
bool circle_intersection(const Pos3D& a, const Pos3D& b, const ld& th, std::vector<Pos3D>& inxs) {
	inxs.clear();
	Pos3D mid = (a + b) * .5;
	if (zero(mid.mag())) return 0;
	ld x = cos(th) / mid.mag();
	if (x < -1 || 1 < x) return 0;
	Pos3D w = mid.unit() * x;
	ld ratio = sqrtl(1 - x * x);
	Pos3D h = (mid / (b - a)).unit() * ratio;
	inxs.push_back(w + h);
	if (!zero(ratio)) inxs.push_back(w - h);
	return 1;
}
bool plane_circle_intersection(const Pos3D& a, const Pos3D& perp, const ld& th, std::vector<Pos3D>& inxs) {//refer to koosaga
	inxs.clear();
	Pos3D vec = a - (perp * (perp * a));
	if (zero(vec.mag())) return 0;
	ld x = cos(th) / vec.mag();
	if (x < -1 || 1 < x) return 0;
	Pos3D w = vec.unit() * x;
	ld ratio = sqrtl(1 - x * x);
	Pos3D h = (vec.unit() / perp) * ratio;
	inxs.push_back(w + h);
	if (!zero(ratio)) inxs.push_back(w - h);
	return 1;
}
Pos3D point(const Pos3D Xaxis, const Pos3D Yaxis, const ld& th) {
	return Xaxis * cos(th) + Yaxis * sin(th);
}
ld angle(const Pos3D Xaxis, const Pos3D Yaxis, const Pos3D& p) {
	ld X = Xaxis * p;
	ld Y = Yaxis * p;
	ld th = atan2(Y, X);
	return norm(th);
}
bool connectable(const std::vector<Pos3D>& P, const Pos3D& a, const Pos3D& b, const ld& th) {//refer to koosaga
	if (zero((a - b).mag())) return 1;
	Pos3D perp = (a / b).unit();
	Pos3D X = a.unit();//X-axis
	Pos3D Y = (perp / a).unit();//Y-axis
	ld hi = angle(X, Y, b);
	std::vector<ld> tmp = { 0, hi };
	std::vector<Pos3D> inxs;
	for (int i = 0; i < N; i++) {
		if (acos(a * P[i]) < th + TOL && acos(b * P[i]) < th + TOL) return 1;
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
void query() {
	int s, t; ld c;
	std::cin >> s >> t >> c;
	ld ans = dijkstra(s - 1, t - 1, N, c);
	if (ans > 1e16) std::cout << "impossible\n";
	else std::cout << ans << "\n";
	return;
}
void solve(const int& tc) {
	std::cout << "Case " << tc << ":\n";
	TH = R / ERAD;
	pos.resize(N);
	ld lon, lat;
	for (int i = 0; i < N; i++)
		std::cin >> lon >> lat, pos[i] = S2C(lon, lat);//unit

	std::vector<Pos3D> inxs;
	for (int i = 0; i < N; i++)
		for (int j = i + 1; j < N; j++)
			if (circle_intersection(pos[i], pos[j], TH, inxs))
				for (const Pos3D& inx : inxs) pos.push_back(inx);

	int sz = pos.size();
	for (int i = 0; i < sz; i++) {
		adj[i][i] = 0;
		for (int j = 0; j < i; j++) {
			if (connectable(pos, pos[i], pos[j], TH))
				adj[i][j] = adj[j][i] = ERAD * acos(pos[i] * pos[j]);
			else
				adj[i][j] = adj[j][i] = INF;
		}
	}

	for (int i = 0; i < N; i++) dijkstra_adj(i, sz);

	std::cin >> q;
	while (q--) query();
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
int main() { solve(); return 0; }//boj4212 Shortest Flight Path - refer to koosaga



//#include <iostream>
//#include <algorithm>
//#include <vector>
//#include <queue>
//#include <cmath>
//#include <cstring>
//typedef long long ll;
////typedef long double ld;
//typedef double ld;
//const ld INF = 1e17;
//const ld TOL = 1e-7;
//const ld PI = acos(-1);
//const ld ERAD = 6370;
//const int LEN = 600 + 25;//nC2(25) + 25
//int N, M, T, q;
//ld R, TH;
//bool zero(const ld& x) { return std::abs(x) < TOL; }
//int dcmp(const ld& x) { return std::abs(x) < TOL ? 0 : x > 0 ? 1 : -1; }
//int dcmp(const ll& x) { return !x ? 0 : x > 0 ? 1 : -1; }
//ll nC2(const ll& x) { return ((x - 1) * x) >> 1; }
//ld norm(ld& th) {
//	while (th < -TOL) th += PI * 2;
//	while (th > PI * 2) th -= PI * 2;
//	return th;
//}
//
//struct Info {
//	int i;
//	ld c;
//	Info(int I = 0, ld C = 0) : i(I), c(C) {}
//	bool operator < (const Info& x) const { return zero(c - x.c) ? i < x.i : c > x.c; }
//};
//ld adj[LEN][LEN], COST[LEN], G[25][25];
//std::priority_queue<Info> Q;
//void dijkstra_adj(const int& v, const int& sz, const int& n = N) {
//	for (int i = 0; i < sz; i++) COST[i] = INF;
//	Q.push({ v, 0 });
//	COST[v] = 0;
//	while (Q.size()) {
//		Info p = Q.top(); Q.pop();
//		if (p.c > COST[p.i]) continue;
//		for (int i = 0; i < sz; i++) {
//			ld w = adj[p.i][i];
//			if (w > 1e16) continue;
//			ld cost = p.c + w;
//			if (COST[i] > cost) {
//				COST[i] = cost;
//				Q.push({ i, cost });
//			}
//		}
//	}
//	for (int g = 0; g < n; g++) G[v][g] = COST[g];
//	return;
//}
//ld dijkstra(const int& v, const int& g, const int& sz, const ld& limit) {
//	for (int i = 0; i < sz; i++) COST[i] = INF;
//	Q.push({ v, 0 });
//	COST[v] = 0;
//	while (Q.size()) {
//		Info p = Q.top(); Q.pop();
//		if (p.c > COST[p.i]) continue;
//		for (int i = 0; i < sz; i++) {
//			ld w = G[p.i][i];
//			if (w > limit) continue;
//			//if (w > limit + TOL) continue;
//			ld cost = p.c + w;
//			if (COST[i] > cost) {
//				COST[i] = cost;
//				Q.push({ i, cost });
//			}
//		}
//	}
//	return COST[g];
//}
//struct Pos3D {
//	ld x, y, z;
//	Pos3D(ld X = 0, ld Y = 0, ld Z = 0) : x(X), y(Y), z(Z) {}
//	bool operator == (const Pos3D& p) const { return zero(x - p.x) && zero(y - p.y) && zero(z - p.z); }
//	bool operator != (const Pos3D& p) const { return !zero(x - p.x) || !zero(y - p.y) || !zero(z - p.z); }
//	bool operator < (const Pos3D& p) const { return zero(x - p.x) ? zero(y - p.y) ? z < p.z : y < p.y : x < p.x; }
//	ld operator * (const Pos3D& p) const { return x * p.x + y * p.y + z * p.z; }
//	Pos3D operator / (const Pos3D& p) const {
//		Pos3D ret;
//		ret.x = y * p.z - z * p.y;
//		ret.y = z * p.x - x * p.z;
//		ret.z = x * p.y - y * p.x;
//		return ret;
//	}
//	Pos3D operator + (const Pos3D& p) const { return { x + p.x, y + p.y, z + p.z }; }
//	Pos3D operator - (const Pos3D& p) const { return { x - p.x, y - p.y, z - p.z }; }
//	Pos3D operator * (const ld& scalar) const { return { x * scalar, y * scalar, z * scalar }; }
//	Pos3D operator / (const ld& scalar) const { return { x / scalar, y / scalar, z / scalar }; }
//	Pos3D& operator += (const Pos3D& p) { x + p.x; y + p.y; z + p.z; return *this; }
//	Pos3D& operator *= (const ld& scalar) { x* scalar; y* scalar; z* scalar; return *this; }
//	ld Euc() const { return x * x + y * y + z * z; }
//	ld mag() const { return sqrtl(Euc()); }
//	ld lon() const { return atan2(y, x); }
//	ld lat() const { return atan2(z, sqrtl(x * x + y * y)); }
//	Pos3D unit() const { return *this / mag(); }
//	Pos3D norm(const Pos3D& p) const { return (*this / p).unit(); }
//	Pos3D rotate(const ld& th, const Pos3D& axis) const {
//		ld SIN = sin(th), COS = cos(th);
//		Pos3D norm = axis.unit();
//		return norm * (*this * norm) * (1 - COS) + (*this * COS) - *this / norm * SIN;
//	}
//	friend std::istream& operator >> (std::istream& is, Pos3D& p) {
//		is >> p.x >> p.y >> p.z;
//		return is;
//	}
//	friend std::ostream& operator << (std::ostream& os, const Pos3D& p) {
//		os << p.x << " " << p.y << " " << p.z << "\n";
//		return os;
//	}
//}; const Pos3D MAXP3D = { INF, INF, INF };
//std::vector<Pos3D> pos;
//Pos3D S2C(const ld& lon, const ld& lat) {//Spherical to Cartesian
//	ld phi = lon * PI / 180;
//	ld the = lat * PI / 180;
//	return Pos3D(cos(phi) * cos(the), sin(phi) * cos(the), sin(the));
//}
//bool circle_intersection(const Pos3D& a, const Pos3D& b, const ld& th, std::vector<Pos3D>& inxs) {
//	inxs.clear();
//	Pos3D mid = (a + b) * .5;
//	if (zero(mid.mag())) return 0;
//	ld x = cos(th) / mid.mag();
//	if (x < -1 || 1 < x) return 0;
//	Pos3D w = mid.unit() * x;
//	ld ratio = sqrtl(1 - x * x);
//	//Pos3D h = (mid.unit() / (b - a).unit()) * ratio;
//	Pos3D h = (mid / (b - a)).unit() * ratio;
//	inxs.push_back(w + h);
//	//inxs.push_back(w - h);
//	if (!zero(ratio)) inxs.push_back(w - h);
//	return 1;
//}
//bool plane_circle_intersection(const Pos3D& a, const Pos3D& perp, const ld& th, std::vector<Pos3D>& inxs) {//refer to koosaga
//	inxs.clear();
//	Pos3D vec = a - (perp * (perp * a));
//	if (zero(vec.mag())) return 0;
//	ld x = cos(th) / vec.mag();
//	if (x < -1 || 1 < x) return 0;
//	Pos3D w = vec.unit() * x;
//	ld ratio = sqrtl(1 - x * x);
//	Pos3D h = (vec.unit() / perp) * ratio;
//	inxs.push_back(w + h);
//	//inxs.push_back(w - h);
//	if (!zero(ratio)) inxs.push_back(w - h);
//	return 1;
//}
//Pos3D point(const Pos3D Xaxis, const Pos3D Yaxis, const ld& th) {
//	return Xaxis * cos(th) + Yaxis * sin(th);
//}
//ld angle(const Pos3D Xaxis, const Pos3D Yaxis, const Pos3D& p) {
//	ld X = Xaxis * p;
//	ld Y = Yaxis * p;
//	ld th = atan2(Y, X);
//	return norm(th);
//}
//bool connectable(const std::vector<Pos3D>& P, const Pos3D& a, const Pos3D& b, const ld& th) {//refer to koosaga
//	if (zero((a - b).mag())) return 1;
//	Pos3D perp = (a / b).unit();
//	Pos3D X = a.unit();//X-axis
//	Pos3D Y = (perp / a).unit();//Y-axis
//	ld hi = angle(X, Y, b);
//	std::vector<ld> tmp = { 0, hi };
//	std::vector<Pos3D> inxs;
//	for (int i = 0; i < N; i++) {
//		if (plane_circle_intersection(P[i], perp, th, inxs)) {
//			for (const Pos3D& inx : inxs) {
//				ld ang = angle(X, Y, inx);
//				if (ang < hi) tmp.push_back(ang);
//			}
//		}
//	}
//	std::sort(tmp.begin(), tmp.end());
//	int sz = tmp.size();
//	bool f = 0;
//	for (int i = 0; i < sz - 1; i++) {//sweeping
//		Pos3D mid = point(X, Y, (tmp[i] + tmp[(i + 1) % sz]) * .5);
//		f = 0;
//		for (int j = 0; j < N; j++) {
//			if (acos(mid * P[j]) < th + TOL) {
//				f = 1;
//				break;
//			}
//		}
//		if (!f) return 0;
//	}
//	return 1;
//}
//void query() {
//	int s, t; ld c;
//	std::cin >> s >> t >> c;
//	ld ans = dijkstra(s - 1, t - 1, N, c);
//	if (ans > 1e16) std::cout << "impossible\n";
//	else std::cout << ans << "\n";
//	return;
//}
//void solve(const int& tc) {
//	std::cout << "Case " << tc << ":\n";
//	TH = R / ERAD;
//	pos.resize(N);
//	ld lon, lat;
//	for (int i = 0; i < N; i++)
//		std::cin >> lon >> lat, pos[i] = S2C(lon, lat);//unit
//	
//	std::vector<Pos3D> inxs;
//	for (int i = 0; i < N; i++)
//		for (int j = i + 1; j < N; j++)
//			if (circle_intersection(pos[i], pos[j], TH, inxs)) 
//				for (const Pos3D& inx : inxs) pos.push_back(inx);
//
//	int sz = pos.size();
//	for (int i = 0; i < sz; i++) {
//		adj[i][i] = 0;
//		for (int j = 0; j < i; j++) {
//			if (connectable(pos, pos[i], pos[j], TH))
//				adj[i][j] = adj[j][i] = ERAD * acos(pos[i] * pos[j]);
//			else
//				adj[i][j] = adj[j][i] = INF;
//		}
//	}
//
//	for (int i = 0; i < N; i++) dijkstra_adj(i, sz);
//	
//	std::cin >> q;
//	while (q--) query();
//	return;
//}
//void solve() {
//	std::cin.tie(0)->sync_with_stdio(0);
//	std::cout.tie(0);
//	std::cout << std::fixed;
//	std::cout.precision(3);
//	T = 0;
//	while (std::cin >> N >> R) solve(++T);
//	return;
//}
//int main() { solve(); return 0; }//boj4212 Shortest Flight Path - refer to koosaga

/*

#include <iostream>
#include <algorithm>
#include <vector>
#include <queue>
#include <cmath>
//#include <cstring>
typedef long long ll;
//typedef long double ld;
typedef double ld;
const ld INF = 1e17;
const ld TOL = 1e-7;
const ld PI = acos(-1);
const ld ERAD = 6370;
const int LEN = 600 + 25;//nC2(25) + 25
bool zero(const ld& x) { return std::abs(x) < TOL; }
ld norm(ld& th) {
	while (th < 0) th += PI * 2;
	while (th > PI * 2 - TOL) th -= PI * 2;
	return th;
}

int N, T, q;
ld R, TH;
ld adj[LEN][LEN], COST[LEN], G[25][25];
struct Info {
	int i;
	ld c;
	Info(int I = 0, ld C = 0) : i(I), c(C) {}
	bool operator < (const Info& x) const { return zero(c - x.c) ? i < x.i : c > x.c; }
};
std::priority_queue<Info> Q;
void dijkstra_adj(const int& v, const int& sz, const int& n = N) {
	for (int i = 0; i < sz; i++) COST[i] = INF;
	Q.push({ v, 0 });
	COST[v] = 0;
	while (Q.size()) {
		Info p = Q.top(); Q.pop();
		if (p.c > COST[p.i]) continue;
		for (int i = 0; i < sz; i++) {
			ld w = adj[p.i][i];
			if (w > INF - 1) continue;
			ld cost = p.c + w;
			if (COST[i] > cost) {
				COST[i] = cost;
				Q.push({ i, cost });
			}
		}
	}
	for (int g = 0; g < n; g++) G[v][g] = COST[g];
	return;
}
ld dijkstra(const int& v, const int& g, const int& sz, const ld& limit) {
	for (int i = 0; i < sz; i++) COST[i] = INF;
	Q.push({ v, 0 });
	COST[v] = 0;
	while (Q.size()) {
		Info p = Q.top(); Q.pop();
		if (p.c > COST[p.i]) continue;
		for (int i = 0; i < sz; i++) {
			ld w = G[p.i][i];
			if (w > limit) continue;
			ld cost = p.c + w;
			if (COST[i] > cost) {
				COST[i] = cost;
				Q.push({ i, cost });
			}
		}
	}
	return COST[g];
}
struct Pos3D {
	ld x, y, z;
	Pos3D(ld X = 0, ld Y = 0, ld Z = 0) : x(X), y(Y), z(Z) {}
	//bool operator == (const Pos3D& p) const { return zero(x - p.x) && zero(y - p.y) && zero(z - p.z); }
	//bool operator != (const Pos3D& p) const { return !zero(x - p.x) || !zero(y - p.y) || !zero(z - p.z); }
	//bool operator < (const Pos3D& p) const { return zero(x - p.x) ? zero(y - p.y) ? z < p.z : y < p.y : x < p.x; }
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
	//Pos3D& operator += (const Pos3D& p) { x += p.x; y += p.y; z += p.z; return *this; }
	//Pos3D& operator *= (const ld& scalar) { x *= scalar; y *= scalar; z *= scalar; return *this; }
	ld Euc() const { return x * x + y * y + z * z; }
	ld mag() const { return sqrtl(Euc()); }
	Pos3D unit() const { return *this / mag(); }
};
const Pos3D O = { 0, 0, 0 };
const Pos3D MAXP3D = { INF, INF, INF };
std::vector<Pos3D> pos;
Pos3D S2C(const ld& lon, const ld& lat) {//Spherical to Cartesian
	ld phi = lon * PI / 180;
	ld the = lat * PI / 180;
	return Pos3D(cos(phi) * cos(the), sin(phi) * cos(the), sin(the));
}
bool circle_intersection(const Pos3D& a, const Pos3D& b, const ld& th, std::vector<Pos3D>& inxs) {
	inxs.clear();
	Pos3D mid = (a + b) * .5;
	if (zero(mid.mag())) return 0;
	ld x = cos(th) / mid.mag();
	if (x < -1 || 1 < x) return 0;
	Pos3D w = mid.unit() * x;
	ld ratio = sqrtl(1 - x * x);
	Pos3D h = (mid / (b - a)).unit() * ratio;
	inxs.push_back(w + h);
	if (!zero(ratio)) inxs.push_back(w - h);
	return 1;
}
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
	if (!zero(ratio)) inxs.push_back(w - h);
	return 1;
}
Pos3D point(const Pos3D Xaxis, const Pos3D Yaxis, const ld& th) {
	return Xaxis * cos(th) + Yaxis * sin(th);
}
ld angle(const Pos3D Xaxis, const Pos3D Yaxis, const Pos3D& p) {
	ld X = Xaxis * p;
	ld Y = Yaxis * p;
	ld th = atan2(Y, X);
	return th;
}
Pos3D cross(const Pos3D& d1, const Pos3D& d2, const Pos3D& d3) { return (d2 - d1) / (d3 - d2); }
int ccw(const Pos3D& d1, const Pos3D& d2, const Pos3D& d3, const Pos3D& norm) {
	Pos3D CCW = cross(d1, d2, d3);
	ld ret = CCW * norm;
	return zero(ret) ? 0 : ret > 0 ? 1 : -1;
}
bool inner_check(const Pos3D& d1, const Pos3D& d2, const Pos3D& t, const Pos3D& nrm) {
	return ccw(O, d1, t, nrm) >= 0 && ccw(O, d2, t, nrm) <= 0;
}
bool connectable(const std::vector<Pos3D>& P, const Pos3D& a, const Pos3D& b, const ld& th) {
	if (zero((a - b).mag())) return 1;
	if (zero((a + b).mag()) && R > ERAD * PI * .5 - TOL) return 1;
	Pos3D perp = (a / b).unit();
	Pos3D X = a.unit();//X-axis
	Pos3D Y = (perp / a).unit();//Y-axis
	ld ang = angle(X, Y, b);
	std::vector<Info> tmp = { { 0, 0 }, { 0, ang } };
	std::vector<Pos3D> inxs;
	for (int i = 0; i < N; i++) {//sweeping
		if (acos(a * P[i]) < th + TOL && acos(b * P[i]) < th + TOL) return 1;
		if (plane_circle_intersection(P[i], perp, th, inxs)) {
			if (inxs.size() == 1) continue;
			Pos3D axis = (P[i] / perp).unit();
			Pos3D mid = (perp / axis).unit();
			Pos3D hi = inxs[0], lo = inxs[1];
			if (ccw(O, mid, lo, perp) > 0) std::swap(hi, lo);
			ld h = angle(X, Y, hi), l = angle(X, Y, lo);
			if (inner_check(a, b, lo, perp) &&
				inner_check(a, b, hi, perp)) {
				if (h < l) {
					tmp.push_back({ 1, -INF });
					tmp.push_back({ -1, l });
					tmp.push_back({ 1, h });
					tmp.push_back({ -1, INF });
				}
				else {
					tmp.push_back({ 1, l });
					tmp.push_back({ -1, h });
				}
			}
			else if (inner_check(a, b, lo, perp)) {
				if (h < 0) h += 2 * PI;
				tmp.push_back({ 1, l });
				tmp.push_back({ -1, h });
			}
			else if (inner_check(a, b, hi, perp)) {
				if (l > 0) l -= 2 * PI;
				tmp.push_back({ 1, l });
				tmp.push_back({ -1, h });
			}
		}
	}
	std::sort(tmp.begin(), tmp.end());
	int toggle = 0;
	bool f = 0;
	for (const Info& s : tmp) {
		toggle -= s.i;
		if (!s.i) f = !f;
		if (f && toggle <= 0) return 0;
	}
	return 1;
}
void query() {
	int s, t; ld c;
	std::cin >> s >> t >> c;
	ld ans = dijkstra(s - 1, t - 1, N, c);
	if (ans > 1e16) std::cout << "impossible\n";
	else std::cout << ans << "\n";
	return;
}
void solve(const int& tc) {
	std::cout << "Case " << tc << ":\n";
	TH = R / ERAD;
	pos.resize(N);
	ld lon, lat;
	for (int i = 0; i < N; i++)
		std::cin >> lon >> lat, pos[i] = S2C(lon, lat);//unit

	std::vector<Pos3D> inxs;
	for (int i = 0; i < N; i++)
		for (int j = i + 1; j < N; j++)
			if (circle_intersection(pos[i], pos[j], TH, inxs))
				for (const Pos3D& inx : inxs) pos.push_back(inx);

	int sz = pos.size();
	for (int i = 0; i < sz; i++) {
		adj[i][i] = 0;
		for (int j = 0; j < i; j++) {
			if (connectable(pos, pos[i], pos[j], TH))
				adj[i][j] = adj[j][i] = ERAD * acos(pos[i] * pos[j]);
			else
				adj[i][j] = adj[j][i] = INF;
		}
	}

	for (int i = 0; i < N; i++) dijkstra_adj(i, sz);

	std::cin >> q;
	while (q--) query();
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
int main() { solve(); return 0; }//boj4212 Shortest Flight Path


//struct Seq {
//	ld d; int i;
//	Seq(ld D = 0, int I = 0) : d(D), i(I) {}
//	bool operator < (const Seq& s) const { return zero(d - s.d) ? i < s.i : d < s.d; }
//};
//bool connectable(const std::vector<Pos3D>& P, const Pos3D& a, const Pos3D& b, const ld& th) {
//	if (zero((a - b).mag())) return 1;
//	if (zero((a + b).mag()) && R > ERAD * PI * .5 - TOL) return 1;
//	Pos3D perp = (a / b).unit();
//	Pos3D X = a.unit();//X-axis
//	Pos3D Y = (perp / a).unit();//Y-axis
//	ld ang = angle(X, Y, b);
//	std::vector<Seq> tmp = { { 0, 0 }, { ang, 0 } };
//	std::vector<Pos3D> inxs;
//	for (int i = 0; i < N; i++) {
//		if (acos(a * P[i]) < th + TOL && acos(b * P[i]) < th + TOL) return 1;
//		if (plane_circle_intersection(P[i], perp, th, inxs)) {
//			if (inxs.size() == 1) continue;
//			Pos3D axis = (P[i] / perp).unit();
//			Pos3D mid = (perp / axis).unit();
//			Pos3D hi = inxs[0], lo = inxs[1];
//			if (ccw(O, mid, lo - mid, perp) > 0) std::swap(hi, lo);
//			ld h = angle(X, Y, hi), l = angle(X, Y, lo);
//			if (inner_check(a, b, lo, perp) &&
//				inner_check(a, b, hi, perp)) {
//				if (h < l) {
//					tmp.push_back({ -INF, -1 });
//					tmp.push_back({ l, 1 });
//					tmp.push_back({ h, -1 });
//					tmp.push_back({ INF, 1 });
//				}
//				else {
//					tmp.push_back({ l, -1 });
//					tmp.push_back({ h, 1 });
//				}
//			}
//			else if (inner_check(a, b, lo, perp)) {
//				if (h < 0) h += 2 * PI;
//				tmp.push_back({ l, -1 });
//				tmp.push_back({ h, 1 });
//			}
//			else if (inner_check(a, b, hi, perp)) {
//				if (l > 0) l -= 2 * PI;
//				tmp.push_back({ l, -1 });
//				tmp.push_back({ h, 1 });
//			}
//		}
//	}
//	std::sort(tmp.begin(), tmp.end());
//	int toggle = 0;
//	bool f = 0;
//	for (const Seq& s : tmp) {
//		toggle -= s.i;
//		if (!s.i) f = !f;
//		if (f && toggle <= 0) return 0;
//	}
//	return 1;
//}

*/