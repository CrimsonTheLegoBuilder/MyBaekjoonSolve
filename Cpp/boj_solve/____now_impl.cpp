//not my code

#ifndef LOCAL
#pragma GCC optimize ("Ofast")
#pragma GCC optimize ("unroll-loops")
#endif

#include <bits/stdc++.h>
using namespace std;

using ll = long long;
#define int ll

#define rng(i,a,b) for(int i=int(a);i<int(b);i++)
#define rep(i,b) rng(i,0,b)
#define gnr(i,a,b) for(int i=int(b)-1;i>=int(a);i--)
#define per(i,b) gnr(i,0,b)
#define pb push_back
#define eb emplace_back
#define a first
#define b second
#define bg begin()
#define ed end()
#define all(x) x.bg,x.ed
#define si(x) int(x.size())
#ifdef LOCAL
#define dmp(x) cerr<<__LINE__<<" "<<#x<<" "<<x<<endl
#else
#define dmp(x) void(0)
#endif

template<class t, class u> bool chmax(t& a, u b) { if (a < b) { a = b; return true; } else return false; }
template<class t, class u> bool chmin(t& a, u b) { if (b < a) { a = b; return true; } else return false; }

template<class t> using vc = vector<t>;
template<class t> using vvc = vc<vc<t>>;

using pi = pair<int, int>;
using vi = vc<int>;

template<class t, class u>
ostream& operator<<(ostream& os, const pair<t, u>& p) {
	return os << "{" << p.a << "," << p.b << "}";
}

template<class t> ostream& operator<<(ostream& os, const vc<t>& v) {
	os << "{";
	for (auto e : v)os << e << ",";
	return os << "}";
}

#define mp make_pair
#define mt make_tuple
#define one(x) memset(x,-1,sizeof(x))
#define zero(x) memset(x,0,sizeof(x))
#ifdef LOCAL
void dmpr(ostream& os) { os << endl; }
template<class T, class... Args>
void dmpr(ostream& os, const T& t, const Args&... args) {
	os << t << " ";
	dmpr(os, args...);
}
#define dmp2(...) dmpr(cerr,__LINE__,##__VA_ARGS__)
#else
#define dmp2(...) void(0)
#endif

using uint = unsigned;
using ull = unsigned long long;

template<class t, size_t n>
ostream& operator<<(ostream& os, const array<t, n>& a) {
	return os << vc<t>(all(a));
}

template<int i, class T>
void print_tuple(ostream&, const T&) {
}

template<int i, class T, class H, class ...Args>
void print_tuple(ostream& os, const T& t) {
	if (i)os << ",";
	os << get<i>(t);
	print_tuple<i + 1, T, Args...>(os, t);
}

template<class ...Args>
ostream& operator<<(ostream& os, const tuple<Args...>& t) {
	os << "{";
	print_tuple<0, tuple<Args...>, Args...>(os, t);
	return os << "}";
}

ll read() {
	ll i;
	cin >> i;
	return i;
}

vi readvi(int n, int off = 0) {
	vi v(n);
	rep(i, n)v[i] = read() + off;
	return v;
}

pi readpi(int off = 0) {
	int a, b; cin >> a >> b;
	return pi(a + off, b + off);
}

template<class t>
void print_single(t x, int suc = 1) {
	cout << x;
	if (suc == 1)
		cout << "\n";
	if (suc == 2)
		cout << " ";
}

template<class t, class u>
void print_single(const pair<t, u>& p, int suc = 1) {
	print_single(p.a, 2);
	print_single(p.b, suc);
}

template<class T>
void print_single(const vector<T>& v, int suc = 1) {
	rep(i, v.size())
		print_single(v[i], i == int(v.size()) - 1 ? suc : 2);
}

template<class T>
void print_offset(const vector<T>& v, ll off, int suc = 1) {
	rep(i, v.size())
		print_single(v[i] + off, i == int(v.size()) - 1 ? suc : 2);
}

template<class T, size_t N>
void print_single(const array<T, N>& v, int suc = 1) {
	rep(i, N)
		print_single(v[i], i == int(N) - 1 ? suc : 2);
}

template<class T>
void print(const T& t) {
	print_single(t);
}

template<class T, class ...Args>
void print(const T& t, const Args&...args) {
	print_single(t, 2);
	print(args...);
}

string readString() {
	string s;
	cin >> s;
	return s;
}

template<class T>
T sq(const T& t) {
	return t * t;
}

void YES(bool ex = true) {
	cout << "YES\n";
	if (ex)exit(0);
#ifdef LOCAL
	cout.flush();
#endif
}
void NO(bool ex = true) {
	cout << "NO\n";
	if (ex)exit(0);
#ifdef LOCAL
	cout.flush();
#endif
}
void Yes(bool ex = true) {
	cout << "Yes\n";
	if (ex)exit(0);
#ifdef LOCAL
	cout.flush();
#endif
}
void No(bool ex = true) {
	cout << "No\n";
	if (ex)exit(0);
#ifdef LOCAL
	cout.flush();
#endif
}
//#define CAPITAL
/*
void yes(bool ex=true){
	#ifdef CAPITAL
	cout<<"YES"<<"\n";
	#else
	cout<<"Yes"<<"\n";
	#endif
	if(ex)exit(0);
	#ifdef LOCAL
	cout.flush();
	#endif
}
void no(bool ex=true){
	#ifdef CAPITAL
	cout<<"NO"<<"\n";
	#else
	cout<<"No"<<"\n";
	#endif
	if(ex)exit(0);
	#ifdef LOCAL
	cout.flush();
	#endif
}*/
void possible(bool ex = true) {
#ifdef CAPITAL
	cout << "POSSIBLE" << "\n";
#else
	cout << "Possible" << "\n";
#endif
	if (ex)exit(0);
#ifdef LOCAL
	cout.flush();
#endif
}
void impossible(bool ex = true) {
#ifdef CAPITAL
	cout << "IMPOSSIBLE" << "\n";
#else
	cout << "Impossible" << "\n";
#endif
	if (ex)exit(0);
#ifdef LOCAL
	cout.flush();
#endif
}

constexpr ll ten(int n) {
	return n == 0 ? 1 : ten(n - 1) * 10;
}

const ll infLL = LLONG_MAX / 3;

#ifdef int
const int inf = infLL;
#else
const int inf = INT_MAX / 2 - 100;
#endif

int topbit(signed t) {
	return t == 0 ? -1 : 31 - __builtin_clz(t);
}
int topbit(ll t) {
	return t == 0 ? -1 : 63 - __builtin_clzll(t);
}
int topbit(ull t) {
	return t == 0 ? -1 : 63 - __builtin_clzll(t);
}
int botbit(signed a) {
	return a == 0 ? 32 : __builtin_ctz(a);
}
int botbit(ll a) {
	return a == 0 ? 64 : __builtin_ctzll(a);
}
int botbit(ull a) {
	return a == 0 ? 64 : __builtin_ctzll(a);
}
int popcount(signed t) {
	return __builtin_popcount(t);
}
int popcount(ll t) {
	return __builtin_popcountll(t);
}
int popcount(ull t) {
	return __builtin_popcountll(t);
}
int bitparity(ll t) {
	return __builtin_parityll(t);
}
bool ispow2(int i) {
	return i && (i & -i) == i;
}
ll mask(int i) {
	return (ll(1) << i) - 1;
}
ull umask(int i) {
	return (ull(1) << i) - 1;
}
ll minp2(ll n) {
	if (n <= 1)return 1;
	else return ll(1) << (topbit(n - 1) + 1);
}

bool inc(int a, int b, int c) {
	return a <= b && b <= c;
}

template<class t> void mkuni(vc<t>& v) {
	sort(all(v));
	v.erase(unique(all(v)), v.ed);
}

ll rand_int(ll l, ll r) { //[l, r]
	//#ifdef LOCAL
	static mt19937_64 gen;
	/*#else
	static mt19937_64 gen(chrono::steady_clock::now().time_since_epoch().count());
	#endif*/
	return uniform_int_distribution<ll>(l, r)(gen);
}

ll rand_int(ll k) { //[0,k)
	return rand_int(0, k - 1);
}

template<class t>
void myshuffle(vc<t>& a) {
	rep(i, si(a))swap(a[i], a[rand_int(0, i)]);
}

template<class t, class u>
int lwb(const vc<t>& v, const u& a) {
	return lower_bound(all(v), a) - v.bg;
}
template<class t, class u>
bool bis(const vc<t>& v, const u& a) {
	return binary_search(all(v), a);
}

vvc<int> readGraph(int n, int m) {
	vvc<int> g(n);
	rep(i, m) {
		int a, b;
		cin >> a >> b;
		//sc.read(a,b);
		a--; b--;
		g[a].pb(b);
		g[b].pb(a);
	}
	return g;
}

vvc<int> readTree(int n) {
	return readGraph(n, n - 1);
}

template<class t>
vc<t> presum(const vc<t>& a) {
	vc<t> s(si(a) + 1);
	rep(i, si(a))s[i + 1] = s[i] + a[i];
	return s;
}
vc<ll> presum(const vi& a) {
	vc<ll> s(si(a) + 1);
	rep(i, si(a))s[i + 1] = s[i] + a[i];
	return s;
}
//BIT で数列を管理するときに使う (CF850C)
template<class t>
vc<t> predif(vc<t> a) {
	gnr(i, 1, si(a))a[i] -= a[i - 1];
	return a;
}
template<class t>
vvc<ll> imos(const vvc<t>& a) {
	int n = si(a), m = si(a[0]);
	vvc<ll> b(n + 1, vc<ll>(m + 1));
	rep(i, n)rep(j, m)
		b[i + 1][j + 1] = b[i + 1][j] + b[i][j + 1] - b[i][j] + a[i][j];
	return b;
}

//verify してないや
void transvvc(int& n, int& m) {
	swap(n, m);
}
template<class t, class... Args>
void transvvc(int& n, int& m, vvc<t>& a, Args&...args) {
	assert(si(a) == n);
	vvc<t> b(m, vi(n));
	rep(i, n) {
		assert(si(a[i]) == m);
		rep(j, m)b[j][i] = a[i][j];
	}
	a.swap(b);
	transvvc(n, m, args...);
}
//CF854E
void rotvvc(int& n, int& m) {
	swap(n, m);
}
template<class t, class... Args>
void rotvvc(int& n, int& m, vvc<t>& a, Args&...args) {
	assert(si(a) == n);
	vvc<t> b(m, vi(n));
	rep(i, n) {
		assert(si(a[i]) == m);
		rep(j, m)b[m - 1 - j][i] = a[i][j];
	}
	a.swap(b);
	rotvvc(n, m, args...);
}

//ソートして i 番目が idx[i]
//CF850C
template<class t>
vi sortidx(const vc<t>& a) {
	int n = si(a);
	vi idx(n); iota(all(idx), 0);
	sort(all(idx), [&](int i, int j) {return a[i] < a[j]; });
	return idx;
}
//vs[i]=a[idx[i]]
//例えば sortidx で得た idx を使えば単にソート列になって返ってくる
//CF850C
template<class t>
vc<t> a_idx(const vc<t>& a, const vi& idx) {
	int n = si(a);
	assert(si(idx) == n);
	vc<t> vs(n);
	rep(i, n)vs[i] = a[idx[i]];
	return vs;
}
//CF850C
vi invperm(const vi& p) {
	int n = si(p);
	vi q(n);
	rep(i, n)q[p[i]] = i;
	return q;
}

template<class t, class s = t>
s SUM(const vc<t>& a) {
	return accumulate(all(a), s(0));
}
template<class t, size_t K, class s = t>
s SUM(const array<t, K>& a) {
	return accumulate(all(a), s(0));
}

template<class t>
t MAX(const vc<t>& a) {
	return *max_element(all(a));
}

template<class t>
pair<t, int> MAXi(const vc<t>& a) {
	auto itr = max_element(all(a));
	return mp(*itr, itr - a.bg);
}

template<class A>
auto MIN(const A& a) {
	return *min_element(all(a));
}

template<class t>
pair<t, int> MINi(const vc<t>& a) {
	auto itr = min_element(all(a));
	return mp(*itr, itr - a.bg);
}

vi vid(int n) {
	vi res(n); iota(all(res), 0);
	return res;
}

template<class S>
void soin(S& s) {
	sort(all(s));
}

template<class S, class F>
void soin(S& s, F&& f) {
	sort(all(s), forward<F>(f));
}

template<class S>
S soout(S s) {
	soin(s);
	return s;
}

template<class S>
void rein(S& s) {
	reverse(all(s));
}

template<class S>
S reout(S s) {
	rein(s);
	return s;
}

template<class t, class u>
pair<t, u>& operator+=(pair<t, u>& a, pair<t, u> b) {
	a.a += b.a; a.b += b.b; return a;
}
template<class t, class u>
pair<t, u>& operator-=(pair<t, u>& a, pair<t, u> b) {
	a.a -= b.a; a.b -= b.b; return a;
}
template<class t, class u>
pair<t, u> operator+(pair<t, u> a, pair<t, u> b) { return mp(a.a + b.a, a.b + b.b); }
template<class t, class u>
pair<t, u> operator-(pair<t, u> a, pair<t, u> b) { return mp(a.a - b.a, a.b - b.b); }

template<class t>
t gpp(vc<t>& vs) {
	assert(si(vs));
	t res = move(vs.back());
	vs.pop_back();
	return res;
}

template<class t, class u>
void pb(vc<t>& a, const vc<u>& b) {
	a.insert(a.ed, all(b));
}

template<class t, class...Args>
vc<t> cat(vc<t> a, Args&&...b) {
	(pb(a, forward<Args>(b)), ...);
	return a;
}

template<class t, class u>
vc<t>& operator+=(vc<t>& a, u x) {
	for (auto& v : a)v += x;
	return a;
}

template<class t, class u>
vc<t> operator+(vc<t> a, u x) {
	return a += x;
}

template<class t>
vc<t> operator+(const vc<t>& a, const vc<t>& b) {
	vc<t> c(max(si(a), si(b)));
	rep(i, si(a))c[i] += a[i];
	rep(i, si(b))c[i] += b[i];
	return c;
}

template<class t, class u>
vc<t>& operator-=(vc<t>& a, u x) {
	for (auto& v : a)v -= x;
	return a;
}

template<class t, class u>
vc<t>& operator-(vc<t> a, u x) {
	return a -= x;
}

template<class t, class u>
void remval(vc<t>& a, const u& v) {
	a.erase(remove(all(a), v), a.ed);
}

template<class VS, class u>
void fila(VS& vs, const u& a) {
	fill(all(vs), a);
}

template<class t, class u>
int findid(const vc<t>& vs, const u& a) {
	auto itr = find(all(vs), a);
	if (itr == vs.ed)return -1;
	else return itr - vs.bg;
}

template<class t>
void rtt(vc<t>& vs, int i) {
	rotate(vs.bg, vs.bg + i, vs.ed);
}


//デバッグ実行でオーバーフローするとコアダンプしがち

using int128 = __int128_t;
using uint128 = unsigned __int128_t;

istream& operator>>(istream& is, int128& res) {
	res = 0;
	string s; is >> s;
	int head = 0;
	int128 w = 1;
	if (s[0] == '-') {
		w = -1;
		head++;
	}
	while (head<int(s.size())) {
		res = res * 10 + s[head++] - '0';
	}
	res *= w;
	return is;
}
ostream& operator<<(ostream& os, int128 i) {
	if (i == 0)
		return os << 0;
	static char buf[100];
	if (i < 0) {
		os << "-";
		i = -i;
	}
	int p = 0;
	while (i) {
		buf[p++] = '0' + i % 10;
		i /= 10;
	}
	reverse(buf, buf + p);
	buf[p] = 0;
	return os << buf;
}
ostream& operator<<(ostream& os, uint128 i) {
	if (i == 0)
		return os << 0;
	static char buf[100];
	int p = 0;
	while (i) {
		buf[p++] = '0' + i % 10;
		i /= 10;
	}
	reverse(buf, buf + p);
	buf[p] = 0;
	return os << buf;
}
int128 abs(int128 a) {
	return a < 0 ? -a : a;
}
int botbit(int128 a) {
	const int128 m = (int128(1) << 64) - 1;
	if (a & m)return __builtin_ctzll(ll(a));
	else return __builtin_ctzll(ll(a >> 64)) + 64;
}
int128 gcd(int128 a, int128 b) {
	if (a < 0)a = -a;
	if (b < 0)b = -b;
	if (a == 0)return b;
	if (b == 0)return a;
	int128 s = botbit(a | b);
	a >>= botbit(a);
	do {
		b >>= botbit(b);
		if (a > b)swap(a, b);
		b -= a;
	} while (b);
	return a << s;
}
const int128 inf128 = int128(1) << 122;

int128 fdiv(int128 a, int128 b) { // floored division
	return a / b - ((a ^ b) < 0 && a % b);
}
//a,c O(10^36)
//b,d O(10^18)
//a/b, c/d の大小比較
int cmpfrac(int128 a, int128 b, int128 c, int128 d) {
	assert(b);
	assert(d);
	if (b < 0) {
		a = -a;
		b = -b;
	}
	if (d < 0) {
		c = -c;
		d = -d;
	}
	int128 x = fdiv(a, b), y = fdiv(c, d);
	if (x < y)return -1;
	if (x > y)return 1;
	int128 u = (a - x * b) * d, v = (c - y * d) * b;
	if (u < v)return -1;
	if (u > v)return 1;
	return 0;
}
bool inc(int128 a, int128 b, int128 c) {
	return a <= b && b <= c;
}

bool dbg = false;

using ld = long double;
using P = pair<int128, int128>;
using L = pair<P, P>;
struct Mirrors {
	vc<int128> vs;
	void add(int128 v) {
		vs.pb(v);
	}
	int128 g;
	void init() {
		g = 0;
		for (auto v : vs) {
			g = gcd(g, v - vs[0]);
		}
	}
	void fund(int128& v) {
		if (si(vs)) {
			v -= vs[0];
			if (g == 0) {
				if (v < 0)v = -v;
			}
			else {
				v %= (g * 2);
				if (v < 0)v += g * 2;
				if (v > g)v = g * 2 - v;
			}
			v += vs[0];
		}
	}
};

void slv() {
	int n, q; cin >> n >> q;
	vc<L> ls(n);
	vc<L> qs(q);
	rep(i, n) {
		cin >> ls[i].a.a >> ls[i].a.b >> ls[i].b.a >> ls[i].b.b;
	}
	rep(i, q) {
		cin >> qs[i].a.a >> qs[i].a.b >> qs[i].b.a >> qs[i].b.b;
	}

	P org = ls[0].a;
	P Xaxis = ls[0].b - ls[0].a;
	auto modify = [&](P& v) {
		v -= org;
		int128 x = v.a * Xaxis.a + v.b * Xaxis.b;
		int128 y = v.a * (-Xaxis.b) + v.b * Xaxis.a;
		v.a = x;
		v.b = y;
		};
	rep(i, n) {
		modify(ls[i].a);
		modify(ls[i].b);
	}
	rep(i, q) {
		modify(qs[i].a);
		modify(qs[i].b);
	}
	//all 18

	bool irr = false;
	rep(i, n) {
		P dir = ls[i].b - ls[i].a;
		if (dir.a && dir.b && abs(dir.a) != abs(dir.b)) {
			irr = true;
		}
	}

	vc<ld> ans(q, -1);
	if (irr) {
		dmp("Irrational");
		vc<P> clist;//(36,18)
		bool dif = false;
		rep(i, n) {
			auto [a, b] = ls[i];
			if (a.b != b.b) {
				int128 num = a.a * b.b - b.a * a.b;
				int128 den = b.b - a.b;
				clist.eb(num, den);
			}
			else if (a.b != 0) {
				dif = true;
			}
		}
		assert(si(clist) > 0);
		rep(i, si(clist) - 1) {
			auto [an, ad] = clist[i];
			auto [bn, bd] = clist[i + 1];
			if (cmpfrac(an, ad, bn, bd)) {
				dif = true;
			}
		}
		if (dif) {
			dmp("Dif");
			rep(i, q) {
				ans[i] = 0;
			}
		}
		else {
			dmp("Center");
			ld c = ld(clist[0].a) / clist[0].b;
			auto dist = [&](P v) {
				return sqrtl(sq(v.a - c) + sq(v.b));
				};
			rep(i, q) {
				auto [a, b] = qs[i];
				ld ad = dist(a);
				ld bd = dist(b);
				ans[i] = abs(ad - bd);
			}
		}
	}
	else {
		dmp("Rational");
		bool has45 = false;
		rep(i, n) {
			P dir = ls[i].b - ls[i].a;
			if (dir.a && dir.b) {
				has45 = true;
			}
		}
		if (has45) {
			dmp("Has 45");

			{
				int128 c;
				rep(i, n) {
					P dir = ls[i].b - ls[i].a;
					if (dir.a && dir.b) {
						dir.a /= dir.b;
						dir.b = 1;
						int128 v = ls[i].a.a - ls[i].a.b * dir.a;
						c = v;
					}
				}
				rep(i, n) {
					auto& [a, b] = ls[i];
					a.a -= c;
					b.a -= c;
				}
				rep(i, q) {
					auto& [a, b] = qs[i];
					a.a -= c;
					b.a -= c;
				}
			}
			//0 is the center

			dmp(ls);

			rep(phase, 2) {
				Mirrors m;
				m.add(0);
				rep(i, n) {
					P dir = ls[i].b - ls[i].a;
					if (dir.a && dir.b) {
						dir.a /= dir.b;
						dir.b = 1;
						int128 v = ls[i].a.a - ls[i].a.b * dir.a;
						m.add(v);
					}
					else if (dir.a == 0) {
						m.add(ls[i].a.a);
					}
					else if (dir.b == 0) {
						m.add(ls[i].a.b);
					}
					else assert(false);
				}
				dmp(m.vs);
				m.init();
				assert(m.vs[0] == 0);

				if (m.g == 0) {
					dmp("45 Centered");
				}
				else {
					dmp("45 Checkered");
				}

				auto work = [&](P& v) {
					if (m.g == 0) {
						if (v.a < 0)v.a *= -1;
						if (v.b < 0)v.b *= -1;
						if (v.a < v.b)swap(v.a, v.b);
					}
					else {
						auto& [a, b] = v;
						m.fund(a);
						m.fund(b);

						dmp2(m.g, a, b);

						//if(!inc(0,a,m.g))exit(0);
						//if(!inc(0,b,m.g))exit(0);
						assert(inc(0, a, m.g));
						assert(inc(0, b, m.g));
						if (a + b >= m.g) {
							tie(a, b) = mp(m.g - b, m.g - a);
						}
						if (a < b) {
							swap(a, b);
						}
					}
					};

				const ld wei = phase == 0 ? 1 : 1 / sqrtl(2);
				rep(i, q) {
					auto [a, b] = qs[i];
					work(a);
					work(b);

					P dif = a - b;
					chmax(ans[i], sqrtl(sq(dif.a) + sq(dif.b)) * wei);
				}

				auto rot = [&](P& v) {
					int128 x = v.a + v.b;
					int128 y = -v.a + v.b;
					v.a = x;
					v.b = y;
					};
				for (auto& [a, b] : ls) {
					rot(a);
					rot(b);
				}
				for (auto& [a, b] : qs) {
					rot(a);
					rot(b);
				}
			}
		}
		else {
			dmp("No 45");

			Mirrors vert, hori;

			rep(i, n) {
				P dir = ls[i].b - ls[i].a;
				if (dir.a == 0) {
					vert.add(ls[i].a.a);
				}
				else if (dir.b == 0) {
					hori.add(ls[i].a.b);
				}
				else assert(false);
			}
			vert.init();
			hori.init();

			auto work = [&](P& v) {
				vert.fund(v.a);
				hori.fund(v.b);
				};

			rep(i, q) {
				auto [a, b] = qs[i];
				work(a);
				work(b);
				P dif = a - b;
				ans[i] = sqrtl(sq(dif.a) + sq(dif.b));
			}
		}
	}
	ld D = sqrtl(sq(Xaxis.a) + sq(Xaxis.b));
	rep(i, q) {
		print(ans[i] / D);
	}
}

signed main() {
	cin.tie(0);
	ios::sync_with_stdio(0);
	cout << fixed << setprecision(20);

	if (dbg) {
		while (1)slv();
	}
	else {
		int t; cin >> t; rep(_, t)
			slv();
	}
}