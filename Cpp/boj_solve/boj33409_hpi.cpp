#include <iostream>
#include <algorithm>
#include <vector>
#include <cmath>
#include <set>
typedef long long ll;
typedef long double ld;
const int LEN = 205;
const ll MOD = 1e9 + 7;
ll p(ll x, ll k) {
	ll ret = 1;
	while (k) {
		if (k & 1) ret = ret * x % MOD;
		x = x * x % MOD;
		k >>= 1;
	}
	return ret;
}

int N, M, K;
struct Fraction {
	ll num, den;
	static ll gcd(ll a, ll b) { return !b ? a : gcd(b, a % b); }
	Fraction(ll n = 0, ll d = 1) : num(n), den(d) { if (den < 0) num *= -1, den *= -1; if (!num) den = 1; }
	Fraction(const Fraction& f) : num(f.num), den(f.den) {}
	Fraction& operator=(const Fraction& f) { if (this != &f) num = f.num, den = f.den; return *this; }
	Fraction normalize() const { ll g = gcd(std::abs(num), den); return Fraction(num / g, den / g); }
	Fraction inv() const { return Fraction(den, num); }
	//ll mod() const { return (num + MOD * MOD) % MOD * p(den, MOD - 2) % MOD; }
	Fraction operator+(const Fraction& r) const {
		ll d = den * r.den / gcd(den, r.den);
		ll n = num * d / den + r.num * d / r.den;
		return Fraction(n, d).normalize();
	}
	Fraction operator-() const { return Fraction(-num, den); }
	Fraction operator-(const Fraction& r) const { return *this + (-r); }
	Fraction operator*(const Fraction& r) const { return Fraction(num * r.num, den * r.den).normalize(); }
	Fraction operator/(const Fraction& r) const { return *this * r.inv(); }
	Fraction& operator*=(const Fraction& r) { return *this = *this * r; }
	Fraction& operator/=(const Fraction& r) { return *this = *this / r; }
	bool operator<(const Fraction& o) const { return num * o.den < o.num * den; }
	bool operator<=(const Fraction& o) const { return num * o.den <= o.num * den; }
	bool operator>(const Fraction& o) const { return o < *this; }
	bool operator==(const Fraction& r) const { return num == r.num && den == r.den; }
	friend std::istream& operator>>(std::istream& i, Fraction& f) { return (i >> f.num >> f.den); }
	friend std::ostream& operator<<(std::ostream& o, const Fraction& f) { return (o << f.num << '/' << f.den); }
} Z = Fraction(0, 1);
template <typename T>
struct Pos {
	T x, y;
	Pos(T x_ = 0, T y_ = 0) : x(x_), y(y_) {}
	bool operator == (const Pos& p) const { return x == p.x && y == p.y; }
	bool operator != (const Pos& p) const { return x != p.x || y != p.y; }
	bool operator < (const Pos& p) const { return x == p.x ? y < p.y : x < p.x; }
	bool operator <= (const Pos& p) const { return x == p.x ? y <= p.y : x <= p.x; }
	Pos operator + (const Pos& p) const { return { x + p.x, y + p.y }; }
	Pos operator - (const Pos& p) const { return { x - p.x, y - p.y }; }
	Pos operator * (const T& n) const { return { x * n, y * n }; }
	Pos operator / (const T& n) const { return { x / n, y / n }; }
	T operator * (const Pos& p) const { return x * p.x + y * p.y; }
	T operator / (const Pos& p) const { return x * p.y - y * p.x; }
	Pos operator ^ (const Pos& p) const { return { x * p.x, y * p.y }; }
	Pos& operator += (const Pos& p) { x += p.x; y += p.y; return *this; }
	Pos& operator -= (const Pos& p) { x -= p.x; y -= p.y; return *this; }
	Pos& operator *= (const T& n) { x *= n; y *= n; return *this; }
	Pos& operator /= (const T& n) { x /= n; y /= n; return *this; }
	Pos operator - () const { return { -x, -y }; }
	Pos operator ~ () const { return { -y, x }; }
	Pos operator ! () const { return { y, x }; }
	T xy() const { return (ll)x * y; }
	T Euc() const { return (ll)x * x + (ll)y * y; }
	//T Man() const { return std::abs(x) + std::abs(y); }
	//ld mag() const { return hypot(x, y); }
	//ld rad() const { return atan2(y, x); }
	//friend ld rad(const Pos& p1, const Pos& p2) { return atan2l(p1 / p2, p1 * p2); }
	friend std::istream& operator >> (std::istream& is, Pos& p) { is >> p.x >> p.y; return is; }
	friend std::ostream& operator << (std::ostream& os, const Pos& p) { os << p.x << " " << p.y; return os; }
};
typedef Pos<Fraction> Pff;
const Pff O = Pff(Z, Z);
typedef std::vector<Pff> Polygon;
typedef std::set<Pff> Spos;
Pff norm(const Pff& p) { return Pff(p.x.normalize(), p.y.normalize()); }
Fraction cross(const Pff& d1, const Pff& d2, const Pff& d3) { return (d2 - d1) / (d3 - d2); }
Pff intersection(const Pff& p1, const Pff& p2, const Pff& q1, const Pff& q2) {
	Fraction a1 = cross(q1, q2, p1);
	Fraction a2 = cross(q1, q2, p2);
	Fraction x = p1.x * a2 + p2.x * a1;
	Fraction y = p1.y * a2 + p2.y * a1;
	Fraction den = a1 + a2;
	x /= den;
	y /= den;
	return Pff(x, y);
}
struct Line {
	Pff s, e;
	Line(Pff s_ = O, Pff e_ = O) : s(s_), e(e_) {}
	Pff dir() const { return e - s; }
	bool operator<(const Line& r) const {
		bool f0 = O < dir();
		bool f1 = O < r.dir();
		if (f0 != f1) return f0;
		Fraction det = dir() / r.dir();
		return det == Z ? cross(r.s, r.e, s) > Z : det > Z;
	}
};
typedef std::vector<Line> Vhp;
Pff intersection(const Line& l0, const Line& l1) { return intersection(l0.s, l0.e, l1.s, l1.e); }
void solve() {
	std::cin.tie(0)->sync_with_stdio(0);
	std::cout.tie(0);
	std::cin >> N;
	Polygon P(N);
	for (Pff& p : P) std::cin >> p;
	K = int(sqrt(N) + 1);
	Spos S;
	Vhp HP;
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++) {
			if (i == j) continue;
			Pff p1 = P[i], p2 = P[(j)];
			Pff v = norm(p2 - p1);
			Pff v_ = v *= Fraction(-1, 1);
			if (S.find(v) != S.end() || S.find(v_) != S.end()) continue;
			int z = 0, r = 0, l = 0;
			for (int k = 0; k < N; k++) {
				if (k == i || j == k) continue;
				Fraction det = cross(p1, p2, P[k]);
				if (det == Z) z++;
				else if (det > Z) l++;
				else r++;
			}
			if (z == K - 3) {
				if (l < r) std::swap(p1, p2);
				HP.push_back(Line(p1, p2));
				v = norm(p2 - p1);
				S.insert(v);
			}
		}
	}
	std::sort(HP.begin(), HP.end());
	int sz = HP.size();
	Polygon ret;
	for (int i = 0; i < sz; i++) {
		const Line& l0 = HP[i], & l1 = HP[(i + 1) % sz];
		ret.push_back(intersection(l0, l1));
	}
	for (const Pff& p : ret) {
		std::cout << p.x.num << " " << p.x.den << " ";
		std::cout << p.y.num << " " << p.y.den << "\n";
	}
	return;
}
int main() { solve(); return 0; }