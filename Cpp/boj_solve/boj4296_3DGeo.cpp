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
char info[4][10];
bool zero(const ld& x) { return std::abs(x) < TOL; }
ld norm(ld th) {
	while (th < -TOL) th += PI * 2;
	while (th > PI * 2) th -= PI * 2;
	return th;
}
ld flip(ld th) {
	if (zero(th - PI * .5)) return -th;
	if (zero(th + PI * .5)) return -th;
	if (zero(th)) return 0;
	if (th > 0) return PI - th;
	if (th < 0) return -PI - th;
}

int T;
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
};
const Pos3D O = { 0, 0, 0 };
const Pos3D X_axis = { 1, 0, 0 };
const Pos3D Y_axis = { 0, 1, 0 };
const Pos3D Z_axis = { 0, 0, 1 };
const Pos3D MAXP3D = { INF, INF, INF };
std::vector<Pos3D> pos;
Pos3D S2C(const ld& lon, const ld& lat, bool f = 0) {//Spherical to Cartesian
	ld phi = lon;
	ld the = lat;
	if (f) phi = lon * PI / 180;
	if (f) the = lat * PI / 180;
	return Pos3D(cos(phi) * cos(the), sin(phi) * cos(the), sin(the));
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
ld parser(const int& i) {
	ld ret;
	int itgr = 0, deci = 0, t = 0;
	//std::cout << info[i] << "\n";
	while (info[i][t] != ',') itgr *= 10, itgr += info[i][t++] - '0';
	itgr *= 100;
	t++;
	while ('0' <= info[i][t] && info[i][t] <= '9') deci *= 10, deci += info[i][t++] - '0';
	deci *= 100;
	t++;
	ret = itgr + (ld)deci / 60;
	//std::cout << ret << "\n";
	if (info[i][t] == 'S' || info[i][t] == 'W') ret *= -1;
	return ret * .01;
}
void query() {
	ld lon, lat;
	Pos3D d1, d2;
	for (int i = 0; i < 4; i++) std::cin >> info[i];
	lat = parser(0);
	lon = parser(1);
	//std::cout << lat << " " << lon << "\n";
	d1 = S2C(norm(lon), lat, 1);
	lat = parser(2);
	lon = parser(3);
	//std::cout << lat << " " << lon << "\n";
	d2 = S2C(norm(lon), lat, 1);
	if ((d1 + d2) * .5 == O) { std::cout << "undefined\n"; }
	else if (d1 == Z_axis || d2 == Z_axis) { std::cout << "90,0N"; }
	else {
		Pos3D perp = d1 / d2;
		lon = norm(perp.lon() + PI);
		lat = flip(perp.lat());
		Pos3D top = S2C(lon, lat);
	}
	return;
}
void solve() {
	std::cin.tie(0)->sync_with_stdio(0);
	std::cout.tie(0);
	std::cin >> T;
	while (T--) query();
	return;
}
int main() { solve(); return 0; }//boj4296 Great Circle
