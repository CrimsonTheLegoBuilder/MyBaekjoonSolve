#include<stdio.h>
#include<math.h>
#include<algorithm>
#include<cmath>

using namespace std;

typedef long long ll;
typedef pair<int, int> pii;
typedef pair<double, double> pdd;
typedef pair<double, int> pdi;

const double EPS = 1e-8;
const double PI = acos(-1);

double sq(double x) { return x * x; }
ll sq(ll x) { return x * x; }

pii operator+(const pii& l, const pii& r) {
	return pii(l.first + r.first, l.second + r.second);
}
pii operator-(const pii& l, const pii& r) {
	return pii(l.first - r.first, l.second - r.second);
}
ll operator^(const pii& l, const pii& r) {
	return (ll)l.first * r.second - (ll)l.second * r.first;
}
ll operator*(const pii& l, const pii& r) {
	return (ll)l.first * r.first + (ll)l.second * r.second;
}

pdd operator+(const pdd& l, const pdd& r) {
	return pdd(l.first + r.first, l.second + r.second);
}
pdd operator-(const pdd& l, const pdd& r) {
	return pdd(l.first - r.first, l.second - r.second);
}
double operator^(const pdd& l, const pdd& r) {
	return l.first * r.second - l.second * r.first;
}
double operator*(const pdd& l, const pdd& r) {
	return l.first * r.first + l.second * r.second;
}
pdd operator*(const pdd& l, const double& r) {
	return pdd(l.first * r, l.second * r);
}
pdd operator-(const pdd& l) {
	return pdd(-l.first, -l.second);
}
double size(pdd x) { return hypot(x.first, x.second); }
double size2(pdd x) { return sq(x.first) + sq(x.second); }
double polar(pdd x) { return atan2(x.second, x.first); }
//double abs(const double &x){ return x < 0 ? -x : x; } 
pdd unit(double a) { return pdd(cos(a), sin(a)); }
pdd norm(pdd a) { return a * (1.0 / size(a)); }
pdd rotate(pdd v, double a) { return unit(a) * v.first + unit(a + PI / 2) * v.second; }

void normalize(double& a) {
	while (a < 0) a += 2 * PI;
	while (a >= 2 * PI) a -= 2 * PI;
}

struct circle {
	pdd O;
	double r;
};

int tangent(circle& A, circle& B, pdd des[4]) {
	// outer
	int top = 0;
	double d = size(A.O - B.O), a = polar(B.O - A.O), b = PI + a;

	double t = sq(d) - sq(A.r - B.r);
	if (t >= 0) {
		t = sqrt(t);
		double p = atan2(B.r - A.r, t);
		des[top++] = pdd(a + p + PI / 2, b + p - PI / 2);
		des[top++] = pdd(a - p - PI / 2, b - p + PI / 2);
	}
	// inner
	t = sq(d) - sq(A.r + B.r);
	if (t >= 0) {
		t = sqrt(t);
		double p = atan2(B.r + A.r, t);
		des[top++] = pdd(a + p - PI / 2, b + p - PI / 2);
		des[top++] = pdd(a - p + PI / 2, b - p + PI / 2);
	}
	return top;
}

int intersect(circle& A, circle& B, pdd des[2]) {
	double d = size(A.O - B.O), t = (sq(A.r) + sq(d) - sq(B.r)) / 2 / A.r / d, u = (sq(B.r) + sq(d) - sq(A.r)) / 2 / B.r / d;
	if (abs(d) < EPS) return 0;
	if (1 - t * t < 0 || 1 - u * u < 0) return 0;
	double a = atan2(sqrt(1 - t * t), t), b = atan2(sqrt(1 - u * u), u), p = polar(B.O - A.O), q = PI + p;
	des[0] = pdd(p + a, q - b);
	des[1] = pdd(p - a, q + b);
	return 2;
}

int intersect(circle& A, pdd& s, pdd& d, pdd des[2]) {
	double c = size2(A.O - s) - sq(A.r), b = d * (s - A.O), a = size2(d);

	if (b * b - a * c < 0) return 0;
	des[0].second = (-b + sqrt(b * b - a * c)) / a;
	des[1].second = (-b - sqrt(b * b - a * c)) / a;
	des[0].first = polar(s + d * des[0].second - A.O);
	des[1].first = polar(s + d * des[1].second - A.O);
	return 2;
}

int intersect(pdd& a, pdd& b, pdd& u, pdd& v, pdd des[2]) {
	if (abs(b ^ v) < EPS) return 0;
	des[0] = pdd(((a - u) ^ v) / (v ^ b), ((a - u) ^ b) / (v ^ b));
	return 1;
}

double dist(pdd& A, pdd& p, pdd& d) {
	double a = atan2((A - p) ^ d, (A - p) * d);
	double r = abs(size(A - p) * sin(a)), e = size(A - p) * cos(a);
	if (0 < e && e < size(d));
	else r = min(size(A - p), size(A - p - d));
	return r;
}

struct V3 {
	double x, y, z;
	V3() {}
	V3(double x, double y, double z) :x(x), y(y), z(z) {}

	V3 operator-()const { return V3(-x, -y, -z); }
	V3 operator-(const V3& l)const {
		return V3(x - l.x, y - l.y, z - l.z);
	}
	V3 operator+(const V3& l)const {
		return V3(x + l.x, y + l.y, z + l.z);
	}
	V3 operator*(const double c)const {
		return V3(x * c, y * c, z * c);
	}
	double operator*(const V3& l)const {
		return x * l.x + y * l.y + z * l.z;
	}
	V3 operator^(const V3& l)const {
		return V3(y * l.z - z * l.y, z * l.x - x * l.z, x * l.y - y * l.x);
	}
	double size() { return sqrt(sq(x) + sq(y) + sq(z)); }
	double size2() { return sq(x) + sq(y) + sq(z); }
	V3 norm() {
		double p = size();
		return V3(x / p, y / p, z / p);
	}
	void print() { printf("%lf %lf %lf\n", x, y, z); }

	bool operator < (const V3& l) const {
		if (abs(x - l.x) >= EPS) return x < l.x;
		if (abs(y - l.y) >= EPS) return y < l.y;
		if (abs(z - l.z) >= EPS) return z < l.z;
		return false;
	}
	bool operator == (const V3& l) const {
		return abs(x - l.x) < EPS && abs(y - l.y) < EPS && abs(z - l.z) < EPS;
	}
};

struct Quad {
	double a;
	V3 v;
	Quad(double a, V3 v) :a(a), v(v) {}
	Quad operator * (const double& c)const {
		return Quad(a * c, v * c);
	}
	Quad operator~() const {
		return Quad(-a, -v);
	}
	Quad operator-() const {
		return Quad(a, -v) * (1 / (sq(a) + sq(v.x) + sq(v.y) + sq(v.z)));
	}
	Quad operator * (const Quad& l)const {
		return Quad(a * l.a - v * l.v, l.v * a + v * l.a + (v ^ l.v));
	}
	V3 apply(V3 p) {
		return ((*this) * Quad(0, p) * -(*this)).v;
	}
};

Quad set_rotate(V3 axis, double a) {
	return Quad(cos(a / 2), axis.norm() * sin(a / 2));
}

struct sphere {
	V3 O;
	double r;
};

int intersect(sphere A, V3 s, V3 d, double des[2]) {
	double c = (A.O - s).size2() - sq(A.r), b = d * (s - A.O), a = d.size2();

	if (b * b - a * c < 0) return 0;
	des[0] = (-b + sqrt(b * b - a * c)) / a;
	des[1] = (-b - sqrt(b * b - a * c)) / a;
	return 2;
}

// == // == // == // == // == // == // == // == // == // == // == // == // == // == // == // == // == // == // == // == // == // == //

#include<vector>

const int MX = 305;
circle D[MX];
vector<double> L[MX];

int main()
{
	//	freopen("input.txt", "r", stdin);
	int N;
	scanf("%d", &N);
	for (int i = 1; i <= N; i++) {
		scanf("%lf%lf%lf", &D[i].O.first, &D[i].O.second, &D[i].r);
	}
	bool chk[MX] = {};
	for (int i = 1; i <= N; i++) {
		for (int j = i + 1; j <= N; j++) {
			if (D[i].O == D[j].O && D[i].r == D[j].r || D[j].r == 0) chk[j] = 1;
		}
	}
	for (int i = 1; i <= N; i++) {
		for (int j = i + 1; j <= N; j++) {
			pdd A[2];
			int top = intersect(D[i], D[j], A);
			for (int k = 0; k < top; k++) {
				L[i].push_back(A[k].first);
				L[j].push_back(A[k].second);
			}
		}
	}
	for (int i = 1; i <= N; i++) {
		for (double& c : L[i]) normalize(c);
		sort(L[i].begin(), L[i].end());
		if (L[i].size() == 0) L[i].push_back(0);
		L[i].push_back(L[i][0] + PI * 2);
	}
	int ans = 0;
	for (int i = 1; i <= N; i++) {
		int in = 1;
		for (int j = 0; j + 1 < L[i].size(); j++) {
			if (L[i][j + 1] - L[i][j] < EPS) continue;
			double u = (L[i][j] + L[i][j + 1]) / 2;
			pdd c = D[i].O + unit(u) * D[i].r;
			int pos = 1;
			for (int k = 1; k <= N; k++) {
				if (size2(D[k].O - c) < (D[k].r - EPS) * (D[k].r - EPS)) pos = 0;
			}
			if (pos) in = 0;
		}
		if (in) chk[i] = 1;
		ans += chk[i];
	}
	printf("%d\n", ans);
}