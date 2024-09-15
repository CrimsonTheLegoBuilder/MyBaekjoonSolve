#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <algorithm>
#include <cmath>
#include <cstring>
#include <cassert>
#include <vector>
#include <queue>
typedef long long ll;
//typedef long double ld;
typedef double ld;
const ll INF = 1e17;
const int LEN = 20;
const ld TOL = 1e-7;
inline int sign(const ld& x) { return x < -TOL ? -1 : x > TOL; }
inline bool zero(const ld& x) { return !sign(x); }

#define __FUCK__ ;

int N, xs, ys;
int MAXX, MAXY, MINX, MINY;
short int board[LEN][LEN];
struct Pii {
	int x, y;
	inline Pii(int X = 0, int Y = 0) : x(X), y(Y) {}
	inline bool operator == (const Pii& p) const { return x == p.x && y == p.y; }
	inline bool operator < (const Pii& p) const { return x == p.x ? y < p.y : x < p.x; }
	inline Pii operator + (const Pii& p) const { return { x + p.x, y + p.y }; }
	inline Pii operator - (const Pii& p) const { return { x - p.x, y - p.y }; }
	inline Pii operator * (const int& n) const { return { x * n, y * n }; }
	inline Pii operator / (const int& n) const { return { x / n, y / n }; }
	inline ll operator * (const Pii& p) const { return (ll)x * p.x + (ll)y * p.y; }
	inline ll operator / (const Pii& p) const { return (ll)x * p.y - (ll)y * p.x; }
	inline Pii& operator += (const Pii& p) { x += p.x; y += p.y; return *this; }
	inline Pii& operator -= (const Pii& p) { x -= p.x; y -= p.y; return *this; }
	inline ll Euc() const { return (ll)x * x + (ll)y * y; }
	inline friend std::istream& operator >> (std::istream& is, Pii& p) { is >> p.x >> p.y; return is; }
	inline friend std::ostream& operator << (std::ostream& os, const Pii& p) { os << p.x << " " << p.y; return os; }
}; const Pii Oii = Pii(0, 0);
typedef std::vector<Pii> Polygon;
inline ll cross(const Pii& d1, const Pii& d2, const Pii& d3) { return (d2 - d1) / (d3 - d2); }
inline ll cross(const Pii& d1, const Pii& d2, const Pii& d3, const Pii& d4) { return (d2 - d1) / (d4 - d3); }
inline int ccw(const Pii& d1, const Pii& d2, const Pii& d3) { ll ret = cross(d1, d2, d3); return ret < 0 ? -1 : !!ret; }
inline ll area(const Polygon& H) {
	ll ret = 0;
	int sz = H.size();
	for (int i = 0; i < sz; i++) ret += H[i] / H[(i + 1) % sz];
	return ret;
}
inline void norm(Polygon& H) {
	ll A = area(H); assert(A);
	if (A < 0) std::reverse(H.begin(), H.end());
	auto s = std::min_element(H.begin(), H.end());
	std::rotate(H.begin(), s, H.end());
	return;
}
inline Polygon graham_scan(Polygon& C) {
	Polygon H;
	if (C.size() < 3) {
		std::sort(C.begin(), C.end());
		return C;
	}
	std::swap(C[0], *min_element(C.begin(), C.end()));
	std::sort(C.begin() + 1, C.end(), [&](const Pii& p, const Pii& q) -> bool {
		int ret = ccw(C[0], p, q);
		if (!ret) return (C[0] - p).Euc() < (C[0] - q).Euc();
		return ret > 0;
		}
	);
	C.erase(unique(C.begin(), C.end()), C.end());
	int sz = C.size();
	for (int i = 0; i < sz; i++) {
		while (H.size() >= 2 && ccw(H[H.size() - 2], H.back(), C[i]) <= 0)
			H.pop_back();
		H.push_back(C[i]);
	}
	return H;
}
inline bool inner_check(const Polygon& H, const Pii& p) {
	int sz = H.size();
	for (int i = 0; i < sz; i++) if (ccw(H[i], H[(i + 1) % sz], p) < 0) return 0;
	return 1;
}
inline int bfs(const int& x, const int& y, const int& val) {//flood fill
	Pii DRC[4] = { Pii(1, 0), Pii(0, 1), Pii(-1, 0), Pii(0, -1) };
	std::queue<Pii> Q;
	Q.push(Pii(x, y));
	board[y][x] = val;
	int cnt = 1;
	while (Q.size()) {
		Pii p = Q.front(); Q.pop();
		for (int i = 0; i < 4; i++) {
			Pii w = p + DRC[i];
			if ((MINX <= w.x) && (w.x <= MAXX) && (MINY <= w.y) && (w.y <= MAXY) && !board[w.y][w.x]) {
				Q.push(w);
				board[w.y][w.x] = val;
				cnt++;
			}
		}
	}
	return cnt;
}
struct Pos {
	ld x, y;
	inline Pos(ld X = 0, ld Y = 0) : x(X), y(Y) {}
	inline bool operator == (const Pos& p) const { return zero(x - p.x) && zero(y - p.y); }
	inline bool operator < (const Pos& p) const { return zero(x - p.x) ? y < p.y : x < p.x; }
	inline Pos operator + (const Pos& p) const { return { x + p.x, y + p.y }; }
	inline Pos operator - (const Pos& p) const { return { x - p.x, y - p.y }; }
	inline Pos operator * (const ld& scalar) const { return { x * scalar, y * scalar }; }
	inline Pos operator / (const ld& scalar) const { return { x / scalar, y / scalar }; }
	inline ld operator * (const Pos& p) const { return x * p.x + y * p.y; }
	inline ld operator / (const Pos& p) const { return x * p.y - y * p.x; }
	inline Pos& operator += (const Pos& p) { x += p.x; y += p.y; return *this; }
	inline Pos& operator -= (const Pos& p) { x -= p.x; y -= p.y; return *this; }
}; const Pos O = Pos(0, 0);
typedef std::vector<Pos> Polygonf;
inline ld cross(const Pos& d1, const Pos& d2, const Pos& d3) { return (d2 - d1) / (d3 - d2); }
inline ld dot(const Pos& d1, const Pos& d2, const Pos& d3) { return (d2 - d1) * (d3 - d2); }
inline int ccw(const Pos& d1, const Pos& d2, const Pos& d3) { ld ret = cross(d1, d2, d3); return sign(ret); }
inline bool on_seg_strong(const Pos& d1, const Pos& d2, const Pos& d3) { ld ret = dot(d1, d3, d2); return !ccw(d1, d2, d3) && sign(ret) >= 0; }
inline Pos intersection(const Pos& p1, const Pos& p2, const Pos& q1, const Pos& q2) { ld a1 = cross(q1, q2, p1), a2 = -cross(q1, q2, p2); return (p1 * a2 + p2 * a1) / (a1 + a2); }
inline bool inner_check(const Polygonf& H, const Pos& p) {//concave
	int sz = H.size(), cnt = 0;
	for (int i = 0; i < sz; i++) {
		Pos cur = H[i], nxt = H[(i + 1) % sz];
		if (on_seg_strong(cur, nxt, p)) return 1;
		if (cur.y == nxt.y) continue;
		if (nxt.y < cur.y) std::swap(cur, nxt);
		if (nxt.y - TOL < p.y || cur.y > p.y) continue;
		cnt += ccw(cur, nxt, p) > 0;
	}
	return cnt & 1;
}
inline Pos P(const Pii& p) { return Pos((ld)p.x, (ld)p.y); }
inline Pii P(const Pos& p) { return Pii(p.x + TOL, p.y + TOL); }
inline Pos cen(const int& i, const int& j, const int& xs, const int& ys) { return Pos(xs * (j + .5), ys * (i + .5)); }
inline Pos norm(Pos& p, const int& x, const int& y) {//fit in tile (0, 0), (xs, ys)
	while (sign(p.x) < 0) p.x += x;
	while (sign(p.x - x) >= 0) p.x -= x;
	while (sign(p.y) < 0) p.y += y;
	while (sign(p.y - y) >= 0) p.y -= y;
	return p;
}
inline void norm(int& x, const int& vx, const int& xs) {
	while (x <= (vx - xs)) x += xs;
	while (x > vx) x -= xs;
	return;
}
inline void norm(int& x, const ld& vx, const int& xs) {
	while (sign(x - (vx - xs)) <= 0) x += xs;
	while (sign(x - vx) > 0) x -= xs;
	return;
}
void sweep(const Pos& cur, const Pos& nxt, const int& xs, const int& ys) {//from 6632 arable
	int sx = 0, ex = 0, sy = 0, ey = 0;
	norm(sx, cur.x, xs);
	norm(ex, nxt.x, xs);
	norm(sy, cur.y, ys);
	norm(ey, nxt.y, ys);
	Pos d = nxt - cur;
	if (zero(d.x)) {
		if (sign(d.y) > 0) {
			if (zero(ey - nxt.y)) ey -= ys;
			if (zero(cur.x - sx) && zero(nxt.x - ex)) for (int i = sy; i <= ey; i += ys) board[i / ys][sx / xs - 1] = 1;
			else for (int i = sy; i <= ey; i += ys) board[i / ys][sx / xs] = 1;
		}
		else if (sign(d.y) < 0) {
			if (zero(sy - cur.y)) sy -= ys;
			for (int i = ey; i <= sy; i += xs) board[i / ys][sx / xs] = 1;
		}
		return;
	}
	if (zero(d.y)) {
		if (sign(d.x) > 0) {
			if (zero(ex - nxt.x)) ex -= xs;
			for (int i = sx; i <= ex; i += xs) board[sy / ys][i / xs] = 1;
		}
		else if (sign(d.x) < 0) {
			if (zero(sx - cur.x)) sx -= xs;
			if (zero(cur.y - sy) && zero(nxt.y - ey)) for (int i = ex; i <= sx; i += xs) board[sy / ys - 1][i / xs] = 1;
			else for (int i = ex; i <= sx; i += xs) board[sy / ys][i / xs] = 1;
		}
		return;
	}
	Pos s = cur, e = nxt;
	if (e < s) std::swap(s, e), std::swap(sx, ex), std::swap(sy, ey);
	int j = sx, i = sy;
	if (zero(e.x - ex)) ex -= xs;
	if (sign(std::abs(e.x - s.x) - std::abs(e.y - s.y)) >= 0) {
		if (s.y < e.y) {
			board[i / ys][j / xs] = 1;
			if (zero(e.y - ey)) ey -= ys;
			while (i <= ey) {
				while (j <= ex && ccw(s, e, Pos((ld)j, (ld)i + ys)) > 0 && ccw(s, e, Pos((ld)j + xs, (ld)i)) < 0) {
					board[i / ys][j / xs] = 1;
					j += xs;
				}
				if (ccw(s, e, Pos((ld)j - xs, (ld)i + 2. * ys)) > 0 && ccw(s, e, Pos(j, (ld)i + ys)) < 0) j -= xs;
				i += ys;
			}
		}
		else if (s.y > e.y) {
			board[i / ys - zero(s.y - sy)][j / xs] = 1;
			if (!zero(s.y - sy)) sy += ys, i += ys;
			while (i > ey) {
				while (j <= ex && ccw(s, e, Pos((ld)j + xs, (ld)i)) > 0 && ccw(s, e, Pos(j, (ld)i - ys)) < 0) {
					board[i / ys - 1][j / xs] = 1;
					j += xs;
				}
				if (ccw(s, e, Pos(j, (ld)i - ys)) > 0 && ccw(s, e, Pos((ld)j - xs, (ld)i - 2. * ys)) < 0) j -= xs;
				i -= ys;
			}
		}
	}
	else {
		if (s.y < e.y) {
			board[i / ys][j / xs] = 1;
			if (zero(e.y - ey)) ey -= ys;
			while (j <= ex) {
				while (i <= ey && ccw(s, e, Pos((ld)j, (ld)i + ys)) > 0 && ccw(s, e, Pos((ld)j + xs, (ld)i)) < 0) {
					board[i / ys][j / xs] = 1;
					i += ys;
				}
				if (ccw(s, e, Pos((ld)j + xs, (ld)i)) > 0 && ccw(s, e, Pos((ld)j + 2. * xs, (ld)i - ys)) < 0) i -= ys;
				j += xs;
			}
		}
		else if (s.y > e.y) {
			board[i / ys - zero(s.y - sy)][j / xs] = 1;
			if (!zero(s.y - sy)) sy += ys, i += ys;
			while (j <= ex) {
				while (i > ey && ccw(s, e, Pos((ld)j + xs, (ld)i)) > 0 && ccw(s, e, Pos((ld)j, (ld)i - ys)) < 0) {
					board[i / ys - 1][j / xs] = 1;
					i -= ys;
				}
				if (ccw(s, e, Pos((ld)j + 2. * xs, (ld)i + ys)) > 0 && ccw(s, e, Pos((ld)j + xs, (ld)i)) < 0) i += ys;
				j += xs;
			}
		}
	}
	return;
}
int sweep(const Polygonf& HF, const int& xs, const int& ys) {
	memset(board, 0, sizeof board);
	ld lx = 1e9, rx = -1e9, ly = 1e9, uy = -1e9;
	int sz = HF.size();
	for (int i = 0; i < sz; i++) {
		lx = std::min(lx, HF[i].x);
		rx = std::max(rx, HF[i].x);
		ly = std::min(ly, HF[i].y);
		uy = std::max(uy, HF[i].y);
		const Pos& cur = HF[i], & nxt = HF[(i + 1) % sz];
		sweep(cur, nxt, xs, ys);
	}
	MAXX = (int)rx / xs;
	MINX = (int)lx / xs;
	MAXY = (int)uy / ys;
	MINY = (int)ly / ys;
	MAXX += 2;
	MAXY += 2;
	for (int i = MINY; i <= MAXY; i++) {
		for (int j = MINX; j <= MAXX; j++) {
			if (!board[i][j]) {
				if (inner_check(HF, cen(i, j, xs, ys))) bfs(j, i, 1);
				else bfs(j, i, 2);
			}
		}
	}
	int cnt = 0;
	for (int i = MINY; i <= MAXY; i++) {
		for (int j = MINX; j <= MAXX; j++) {
			assert(board[i][j]);
			if (board[i][j] == 1) cnt++;
		}
	}
	return cnt;
}
void solve() {
	std::cin.tie(0)->sync_with_stdio(0);
	std::cout.tie(0);
	std::cout << std::fixed;
	std::cout.precision(7);
	std::cin >> N >> xs >> ys;
	Polygon H(N);
	Polygonf V;//All possible point
	for (int i = 0; i < N; i++) std::cin >> H[i];
	norm(H);
	Pii S = H[0];
	for (int i = 0; i < N; i++) {//O(50)
		for (int j = 0; j < N; j++) {//O(50 * 50)
			Pii I = H[i], J = H[j], K = H[(j + 1) % N];
			int x = I.x, y = J.y;
			Pos v = P(S - Pii(x, y));
			V.push_back(norm(v, xs, ys));
			if (J.x != K.x) {
				if (J.x > K.x) std::swap(J, K);
				int jx = I.x;
				norm(jx, J.x, xs);
				int kx = I.x;
				norm(kx, K.x, xs);
				Pii vec = K - J;
				for (int kw = jx; kw <= kx; kw += xs) {//O(50 * 50 * 20)
					if (kw < J.x || K.x < kw) continue;
					ld yh = J.y + ((ld)kw - J.x) * vec.y / vec.x;
					v = P(S) - Pos(I.x, yh);
					V.push_back(norm(v, xs, ys));
				}
			}

			I = H[i], J = H[j], K = H[(j + 1) % N];
			x = J.x, y = I.y;
			v = P(S - Pii(x, y));
			V.push_back(norm(v, xs, ys));
			if (J.y != K.y) {
				if (J.y > K.y) std::swap(J, K);
				int jy = I.y;
				norm(jy, J.y, ys);
				int ky = I.y;
				norm(ky, K.y, ys);
				Pii vec = K - J;
				for (int kh = jy; kh <= ky; kh += ys) {//O(50 * 50 * 20)
					if (kh < J.y || K.y < kh) continue;
					ld yw = J.x + ((ld)kh - J.y) * vec.x / vec.y;
					v = P(S) - Pos(yw, I.y);
					V.push_back(norm(v, xs, ys));
				}
			}
		}
	}
	std::sort(V.begin(), V.end());
	V.erase(unique(V.begin(), V.end()), V.end());
	for (int i = 0; i < N; i++) {//O(50)
		Pii I1 = H[i], I2 = H[(i + 1) % N];
		for (int j = 0; j < N; j++) {//O(50 * 50)
			Pii J1 = H[j], J2 = H[(j + 1) % N];
			if (i == j || !cross(I1, I2, J1, J2)) continue;
			Pii vec = I2 - I1;
			if (J2 < J1) std::swap(J1, J2);
			Polygon box = { J1, J1 - vec, J2, J2 - vec };
			box = graham_scan(box);
			assert(box.size() == 4);
			int lx = 1e9, rx = -1e9, ly = 1e9, uy = -1e9;
			for (int k = 0; k < 4; k++) {
				lx = std::min(lx, box[k].x);
				rx = std::max(rx, box[k].x);
				ly = std::min(ly, box[k].y);
				uy = std::max(uy, box[k].y);
			}
			int jx = I1.x;
			norm(jx, lx, xs);
			int jy = I1.y;
			norm(jy, ly, ys);
			for (int x = jx; x <= rx; x += xs) {//O(50 * 50 * 20)
				for (int y = jy; y <= uy; y += ys) {//O(50 * 50 * 20 * 20)
					if (inner_check(box, Pii(x, y))) {//O(50 * 50 * 20 * 20 * 4) == O(4000000)
						Pos o = intersection(P(J1), P(J2), Pos(x, y), Pos(x, y) + P(vec));
						Pos v = P(S) - o;
						V.push_back(norm(v, xs, ys));
					}
				}
			}
		}
	}
	std::sort(V.begin(), V.end());
	V.erase(unique(V.begin(), V.end()), V.end());
	Polygonf C(N);
	for (int i = 0; i < N; i++) C[i] = P(H[i]);
	int sz = V.size();
	int cnt = 1e6;
	for (int z = 0; z < sz; z++) {//O(50 * 50 * 20 * 20 * 4) == O(4000000)
		const Pos& s = V[z];
		Polygonf HF = C;
		Pos v = s - HF[0];
		ld miny = 1e9;
		for (int i = 0; i < N; i++) HF[i] += v, miny = std::min(miny, HF[i].y);
		int y = 0; norm(y, miny, ys);
		for (int i = 0; i < N; i++) HF[i].y -= y, HF[i] += Pos(xs, ys);
		cnt = std::min(cnt, sweep(HF, xs, ys));//O(50 * 50 * 20 * 20 * 4 * 50) == O(2,0000,0000)
	}
	std::cout << cnt << "\n";
	return;
}
int main() { solve(); return 0; }//boj8883