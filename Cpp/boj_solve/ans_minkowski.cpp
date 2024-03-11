#include <bits/stdc++.h>
#define fst first
#define snd second
#define fore(i,a,b) for(int i=a,ThxDem=b;i<ThxDem;++i)
#define pb push_back
#define ALL(s) s.begin(),s.end()
#define FIN ios_base::sync_with_stdio(0);cin.tie(0);cout.tie(0)
#define SZ(s) int(s.size())
using namespace std;
typedef long long ll;
typedef pair<int, int> ii;

struct pt {
	ll x, y;
	pt() {}
	pt(ll x, ll y) :x(x), y(y) {}
	pt operator +(pt p) { return pt(x + p.x, y + p.y); }
	pt operator -(pt p) { return pt(x - p.x, y - p.y); }
	pt operator *(ll k) { return pt(x * k, y * k); }
	pt operator /(ll k) { return pt(x / k, y / k); }
	ll operator %(pt p) { return x * p.y - y * p.x; }
	bool left(pt p, pt q) { return (q - p) % (*this - p) >= 0; }
	bool left2(pt p, pt q) { return (q - p) % (*this - p) > 0; }
	bool operator <(pt p) { return x < p.x || (x == p.x && y < p.y); }
};

void normalize(vector<pt>& p) {
	int n = SZ(p);
	if (p[2].left2(p[0], p[1]))reverse(ALL(p));
	int pi = min_element(p.begin(), p.end()) - p.begin();
	vector<pt> s(n);
	fore(i, 0, n)s[i] = p[(pi + i) % n];
	p.swap(s);
}

vector<pt> chull(vector<pt> p) {
	if (SZ(p) < 3)return p;
	vector<pt> r;
	sort(ALL(p));
	fore(i, 0, SZ(p)) {
		while (SZ(r) >= 2 && r.back().left(r[SZ(r) - 2], p[i]))r.pop_back();
		r.pb(p[i]);
	}
	r.pop_back();
	int k = SZ(r);
	for (int i = SZ(p) - 1; i >= 0; i--) {
		while (SZ(r) >= k + 2 && r.back().left(r[SZ(r) - 2], p[i])) r.pop_back();
		r.pb(p[i]);
	}
	r.pop_back();
	normalize(r);
	return r;
}

bool haslog(vector<pt>& p, pt q) {
	if (q.left2(p[0], p[1]) || q.left2(p.back(), p[0]))return 0;
	int a = 1, b = p.size() - 1;
	while (b - a > 1) {
		int c = (a + b) / 2;
		if (!q.left2(p[0], p[c]))a = c;
		else b = c;
	}
	return !q.left2(p[a], p[a + 1]);
}

vector<pt> minkowski_sum(vector<pt> p, vector<pt> q) {
	int n = SZ(p), m = SZ(q), x = 0, y = 0;
	fore(i, 0, n) if (p[i] < p[x]) x = i;
	fore(i, 0, m) if (q[i] < q[y]) y = i;
	vector<pt> ans = { p[x] + q[y] };
	fore(it, 1, n + m) {
		pt a = p[(x + 1) % n] + q[y];
		pt b = p[x] + q[(y + 1) % m];
		if (b.left(ans.back(), a)) ans.pb(b), y = (y + 1) % m;
		else ans.pb(a), x = (x + 1) % n;
	}
	return ans;
}

vector<pt> operator *(vector<pt> v, ll x) {
	for (auto& a : v) a = a * x;
	return v;
}

int main() {
	FIN;
	int n; cin >> n;
	vector<pt> p(n);
	fore(i, 0, n) cin >> p[i].x >> p[i].y;
	reverse(ALL(p));

	int m; cin >> m;
	vector<pt> q(m);
	fore(i, 0, m) cin >> q[i].x >> q[i].y;
	reverse(ALL(q));

	vector<pt> me = minkowski_sum(p, q);
	vector<pt> he = minkowski_sum(p * 2, q * -1);
	vector<pt> he2 = minkowski_sum(q * 2, p * -1);

	me = chull(me);
	he = chull(he);
	he2 = chull(he2);

	int k; cin >> k;
	fore(i, 0, k) {
		pt p; cin >> p.x >> p.y;
		if (haslog(me, p * 2) || haslog(he, p) || haslog(he2, p))cout << "Y";
		else cout << "N";
	}
	cout << "\n";
}