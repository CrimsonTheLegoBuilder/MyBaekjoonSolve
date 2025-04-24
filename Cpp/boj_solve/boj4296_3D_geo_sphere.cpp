#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <algorithm>
#include <cmath>
#include <cstring>
typedef long long ll;
typedef double ld;
const ld INF = 1e17;
const ld TOL = 1e-7;
const ld PI = acos(-1);
char info[4][10];
bool zero(const ld& x) { return std::abs(x) < TOL; }
ld norm(ld th) {
	while (th < -TOL) th += PI * 2;
	while (th > PI * 2) th -= PI * 2;
	return th;
}
ld flip(ld lat) {
	if (zero(lat - PI * .5) || zero(lat + PI * .5)) return 0;
	if (zero(lat)) return PI * .5;
	if (lat > 0) return PI * .5 - lat;
	if (lat < 0) return -(PI * .5) - lat;
	return INF;
}

int T;
struct Pos3D {
	ld x, y, z;
	Pos3D(ld X = 0, ld Y = 0, ld Z = 0) : x(X), y(Y), z(Z) {}
	bool operator == (const Pos3D& p) const { return zero(x - p.x) && zero(y - p.y) && zero(z - p.z); }
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
	Pos3D& operator += (const Pos3D& p) { x += p.x; y += p.y; z += p.z; return *this; }
	Pos3D& operator *= (const ld& scalar) { x *= scalar; y *= scalar; z *= scalar; return *this; }
	ld Euc() const { return x * x + y * y + z * z; }
	ld mag() const { return sqrtl(Euc()); }
	ld lon() const { return atan2(y, x); }
	ld lat() const { return atan2(z, sqrtl(x * x + y * y)); }
	Pos3D unit() const { return *this / mag(); }
};
const Pos3D O = { 0, 0, 0 };
//const Pos3D X_axis = { 1, 0, 0 };
//const Pos3D Y_axis = { 0, 1, 0 };
const Pos3D Z_axis = { 0, 0, 1 };
Pos3D S2C(const ld& lon, const ld& lat, bool f = 0) {//Spherical to Cartesian
	ld phi = f ? norm(lon * PI / 180) : norm(lon);
	ld the = f ? lat * PI / 180 : lat;
	return Pos3D(cos(phi) * cos(the), sin(phi) * cos(the), sin(the));
}
Pos3D cross(const Pos3D& d1, const Pos3D& d2, const Pos3D& d3) { return (d2 - d1) / (d3 - d2); }
int ccw(const Pos3D& d1, const Pos3D& d2, const Pos3D& d3, const Pos3D& norm) {
	Pos3D CCW = cross(d1, d2, d3);
	ld ret = CCW * norm;
	return zero(ret) ? 0 : ret > 0 ? 1 : -1;
}
bool inner_check(const Pos3D& d1, const Pos3D& d2, const Pos3D& t) {
	Pos3D nrm = cross(O, d1, d2);
	Pos3D p1 = d1, p2 = d2;
	if (ccw(O, p1, p2, nrm) < 0) std::swap(p1, p2);
	return ccw(O, p1, t, nrm) >= 0 && ccw(O, p2, t, nrm) <= 0;
}
ld parser(const int& i) {
	int itgr = 0, deci = 0, t = 0;
	while (info[i][t] != ',')
		itgr *= 10, itgr += info[i][t++] - '0';
	t++;
	while ('0' <= info[i][t] && info[i][t] <= '9')
		deci *= 10, deci += info[i][t++] - '0';

	ld ret = itgr + (ld)deci / 60;
	if (info[i][t] == 'S' || info[i][t] == 'W') ret *= -1;
	return ret;
}
void query() {
	ld lon, lat;
	Pos3D d1, d2;
	for (int i = 0; i < 4; i++) std::cin >> info[i];

	lat = parser(0); lon = parser(1);
	d1 = S2C(lon, lat, 1);

	lat = parser(2); lon = parser(3);
	d2 = S2C(lon, lat, 1);

	if (d1 == Z_axis || d2 == Z_axis) { std::cout << "90,0N\n"; return; }
	else if (d1 == d2) { std::cout << info[0] << "\n"; return; }
	else if ((d1 + d2) * .5 == O) { std::cout << "undefined\n"; return; }
	else if (zero(d1.z) && zero(d2.z)) { std::cout << "0,0N\n"; return; }

	Pos3D top;
	if (d1.z < 0 + TOL && d2.z < 0 + TOL) { top = d1.z < d2.z ? d2 : d1; }
	else {
		Pos3D perp = (d1 / d2).unit();
		if (perp.z < 0) perp *= -1;
		lon = norm(perp.lon() + PI);
		lat = flip(perp.lat());
		top = S2C(lon, lat);
		if (!inner_check(d1, d2, top)) top = d1.z < d2.z ? d2 : d1;
	}
	ld ret = top.lat() * 180 / PI;
	int itgr = ret;
	int sexa = std::abs(itgr - ret) * 60 + .5;//sexagesimal
	itgr = std::abs(itgr);
	if (sexa == 60) itgr++, sexa = 0;
	std::cout << itgr << "," << sexa << (ret > 0 ? "N\n" : "S\n");
	return;
}
void solve() {
	std::cin.tie(0)->sync_with_stdio(0);
	std::cout.tie(0);
	//freopen("boj4296_in.txt", "r", stdin);
	//freopen("boj4296_out.txt", "w", stdout);
	std::cin >> T;
	while (T--) query();
	return;
}
int main() { solve(); return 0; }//boj4296 Great Circle

/*

1
53,17S 110,48E 85,4N 10,38W

*/
