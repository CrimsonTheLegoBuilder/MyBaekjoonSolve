#include <algorithm>
#include <array>
#include <cmath>
#include <iomanip>
#include <iostream>
#include <queue>
#include <vector>
using namespace std;

using T  = long double;
using ll = long long;
T INF    = 1e12;

struct LP {
	ll x, y;
};
struct P {
	T x, y;
};
struct C {
	LP p;
	T r;
};
P operator-(const P &l, const P &r) { return {l.x - r.x, l.y - r.y}; }
T dot(P l, P r) { return l.x * r.x + l.y * r.y; }
struct L {
	T a, b, c; // ax + by + cz = 0
	L(T a = 0, T b = 0, T c = 0) : a(a), b(b), c(c) {}
	L(P p1, P p2) : a(p2.y - p1.y), b(p1.x - p2.x), c(p2.x * p1.y - p2.y * p1.x) {}
};
vector<P> intersect(const C &cc, const L &l) {
	T x = cc.p.x, y = cc.p.y, r = cc.r, a = l.a, b = l.b, c = l.c;
	T n = a * a + b * b, t1 = c + a * x + b * y, D = n * r * r - t1 * t1;
	if(D < 0) return {};
	T xmid = b * b * x - a * (c + b * y), ymid = a * a * y - b * (c + a * x);
	if(D == 0) return {P{xmid / n, ymid / n}};
	T sd = sqrt(D);
	return {P{(xmid - b * sd) / n, (ymid + a * sd) / n},
	        P{(xmid + b * sd) / n, (ymid - a * sd) / n}};
}
vector<P> intersect(const C &c1, const C &c2) {
	ll x = c1.p.x - c2.p.x, y = c1.p.y - c2.p.y, r1 = c1.r, r2 = c2.r;
	ll n  = x * x + y * y;
	ll d1 = -(n - (r1 + r2) * (r1 + r2)), d2 = (n - (r1 - r2) * (r1 - r2));
	ll xmid = x * (-r1 * r1 + r2 * r2 + n), ymid = y * (-r1 * r1 + r2 * r2 + n);
	if(d1 == 0 || d2 == 0) return {P{c2.p.x + xmid / (2.L * n), c2.p.y + ymid / (2.L * n)}};
	if(d1 < 0 ^ d2 < 0) return {};
	T sd = sqrt(T(d1) * T(d2));
	return {P{c2.p.x + (xmid - y * sd) / (2.L * n), c2.p.y + (ymid + x * sd) / (2.L * n)},
	        P{c2.p.x + (xmid + y * sd) / (2.L * n), c2.p.y + (ymid - x * sd) / (2.L * n)}};
}

struct Edge {
	int v;
	T weight;
}; // input edges
struct PQ {
	T d;
	int v;
}; // distance and target
bool operator>(const PQ &l, const PQ &r) { return l.d > r.d; }
T dijkstra(vector<vector<Edge>> &edges, int s, int t) {
	vector<T> dist(edges.size(), INF);
	priority_queue<PQ, vector<PQ>, greater<PQ>> pq;
	dist[s] = 0;
	pq.push({0, s});
	while(!pq.empty()) {
		auto d = pq.top().d;
		auto u = pq.top().v;
		pq.pop();
		if(u == t) break; // target reached
		if(d == dist[u])
			for(auto &e : edges[u])
				if(dist[e.v] > d + e.weight) pq.push({dist[e.v] = d + e.weight, e.v});
	}
	return dist[t];
}

int main() {
	vector<P> ps(2);
	cin >> ps[0].x >> ps[0].y >> ps[1].x >> ps[1].y;
	int n;
	cin >> n;
	vector<C> cs(n);
	for(auto &c : cs) cin >> c.p.x >> c.p.y >> c.r;

	// calculate all intersection points
	for(int i = 0; i < n; ++i)
		for(int j = 0; j < i; ++j) {
			auto is = intersect(cs[i], cs[j]);
			ps.insert(ps.end(), is.begin(), is.end());
		}
	// for(int i = 0; i < ps.size(); ++i) cerr << i << ": " << ps[i].x << '-' << ps[i].y << '\n';
	n = ps.size();

	// cerr << "NUMBER OF INTERSECTIONS: " << n << endl;

	vector<vector<Edge>> g(n);
	for(int i = 0; i < n; ++i)
		for(int j = 0; j < i; ++j) {
			L l(ps[i], ps[j]);
			T target = dot(ps[j] - ps[i], ps[j] - ps[i]), length = sqrt(target);
			// cerr << "line " << ps[i].x << '-' << ps[i].y << " to " << ps[j].x << '-' << ps[j].y
			//<< endl;
			// cerr << "target: " << target << endl;
			// is this line covered by circles?
			vector<array<T, 2>> fs;
			for(auto &c : cs) {
				auto is = intersect(c, l);
				if(is.size() == 2) {
					array<T, 2> a;
					for(int k : {0, 1}) a[k] = dot(ps[j] - ps[i], is[k] - ps[i]);
					if(a[0] > a[1]) swap(a[0], a[1]);
					a[0] -= 1e-6;
					a[1] += 1e-6;
					fs.emplace_back(a);
					// cerr << "cover at ";
					// for(auto p : is) cerr << p.x << "-" << p.y << "; ";
					// cerr << "from " << a[0] << " to " << a[1] << endl;
				}
			}
			sort(fs.begin(), fs.end(), [](auto &l, auto &r) { return l[0] < r[0]; });
			// do the intervals cover [0,target]
			T last = 0;
			for(auto &i : fs) {
				if(i[0] > last) goto bad;
				last = max(last, i[1]);
				if(last >= target) break;
			}
			if(last < target) goto bad;
			// add this edge to the graph
			g[i].push_back({j, length});
			g[j].push_back({i, length});
		// cerr << "ADD LINE " << j << ' ' << i << " length " << length << endl;
		bad:;
		}

	// run dijkstra from s to t
	auto dist = dijkstra(g, 0, 1);
	if(dist < INF)
		cout << fixed << setprecision(11) << dist << endl;
	else
		cout << "impossible\n";
	return 0;
}
