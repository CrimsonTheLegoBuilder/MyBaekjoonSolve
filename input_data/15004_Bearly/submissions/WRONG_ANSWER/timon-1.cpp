#include <bits/stdc++.h>
using namespace std;

using ld = long double;
using ll = long long;

constexpr ld EPS = 1e-9;

struct P {
	ld x, y;
	P operator+(const P &o) const { return P{x+o.x, y+o.y}; }
	P operator-(const P &o) const { return P{x-o.x, y-o.y}; }
	P operator*(const ld &c) const { return P{x*c, y*c}; }
	P operator/(const ld &c) const { return P{x/c, y/c}; }
	ld operator*(const P &o) const { return x*o.x + y*o.y; }
	ld operator/(const P &o) const { return x*o.y - y*o.x; }
	ld lsq() const { return (*this) * (*this); }
	P rot90() const { return P{y, -x}; }
	ld onto(const P &o) const { return ((*this) * o) / o.lsq(); }
};

void add(P ca, ll ra, P cb, ll rb, vector<P> &pts) {
	P ba = cb - ca;
	ld D = -(ba.lsq() - (ra+rb) * (ra+rb)) * (ba.lsq() - (rb-ra) * (rb-ra));
	if (D < -EPS) return;
	P m = ba * (ba.lsq() + ra * ra - rb * rb);
	if (D == 0) {
		pts.push_back(ca + (m / (2.0 * ba.lsq())));
	} else {
		ld sd = sqrt(D);
		pts.push_back(ca + (P{m.x - ba.y * sd, m.y + ba.x * sd} / (2.0 * ba.lsq())));
		pts.push_back(ca + (P{m.x + ba.y * sd, m.y - ba.x * sd} / (2.0 * ba.lsq())));
	}
}

struct iv {
	ld l, r;
	bool operator<(const iv &o) const { return l < o.l; }
};
bool free(P a, P b, const vector<P> &C, const vector<ll> &R) {
	P ba = (b - a);
	if (ba.lsq() < EPS) return true;
	P ban = ba / sqrt(ba.lsq());

	vector<iv> cover;
	for (size_t i = 0; i < C.size(); ++i) {
		P cp = a + (ba * ((C[i] - a).onto(ba)));
//		cerr << "Checking if " << i << " is in the cover ..." << endl;
//		cerr << cp.x << ' ' << cp.y << endl;
//		cerr << a.x << ' '<< a.y << ' ' << ba.x << ' ' << ba.y << endl;
//		cerr << C[i].x << ' ' << C[i].y << endl;
//		cerr << (C[i] - a).onto(ba) << endl;
		if ((cp - C[i]).lsq() > R[i] * R[i] + EPS) continue;
		ld d = sqrt(max(ld(0.0), R[i] * R[i] - (cp - C[i]).lsq()));
		P p1 = cp - (ban * d);
		P p2 = cp + (ban * d);
		cover.push_back(iv{(p1 - a).onto(ba), (p2 - a).onto(ba)});
	}

//	cerr << "Cover of size " << cover.size() << endl;
	for (auto &i : cover)
		i.l = max(i.l - EPS, ld(0.0)),
		i.r = min(i.r + EPS, ld(1.0));
	sort(cover.begin(), cover.end());

//	for (auto &i : cover) {
//		cerr << i.l << ' ' << i.r << endl;
//	}

	ld last = 0.0;
	for (const auto &i : cover) {
		if (i.l - last > EPS) return false;
		last = max(last, i.r);
	}
	return last + EPS > 1.0;
}

struct E {
	int u;
	ld w;
	bool operator<(const E &o) const { return w > o.w; }
};

int main() {
	P b, m;
	cin >> b.x >> b.y >> m.x >> m.y;

	int n;
	cin >> n;
	vector<P> C(n);
	vector<ll> R(n);
	for (int i = 0; i < n; ++i)
		cin >> C[i].x >> C[i].y >> R[i];

	vector<P> pts{b, m};
	for (int i = 0; i < n; ++i) {
		for (int j = i + 1; j < n; ++j) {
			add(C[i], R[i], C[j], R[j], pts);
		}
	}

	vector<vector<E>> edges(pts.size());
	for (size_t i = 0; i < pts.size(); ++i) {
//		cerr << "Point " << i << ": (" << pts[i].x << ',' << pts[i].y << ")\n";
		for (size_t j = 0; j < pts.size(); ++j) {
			if (i == j) continue;
//			cerr << "Connecting with point " << j << endl;
			if (free(pts[i], pts[j], C, R))
				edges[i].push_back(E{(int)j, sqrt((pts[i] - pts[j]).lsq())}),
				edges[j].push_back(E{(int)i, sqrt((pts[i] - pts[j]).lsq())});
//				cerr << "Success!" << endl
		}
	}

	vector<ld> dist(pts.size(), -1.0);
	priority_queue<E> pq;
	pq.push(E{0, dist[0] = 0.0});
	while (!pq.empty()) {
		int u = pq.top().u;
		ld w = pq.top().w;
		pq.pop();
		if (dist[u] + EPS < w) continue;
		for (const auto &e : edges[u]) {
			if (dist[e.u] < -0.5 || dist[e.u] > w + e.w + EPS) {
				dist[e.u] = w + e.w;
				pq.push({e.u, dist[e.u]});
			}
		}
	}

	if (dist[1] < -0.5)
		printf("impossible\n");
	else
		printf("%.12lf\n", double(dist[1]));
}
