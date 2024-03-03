#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <algorithm>
#include <cmath>
#include <cstring>
typedef long long ll;
//typedef long double ld;
typedef double ld;
const ld INF = 1e17;
const ld TOL = 1e-7;
const ld PI = acos(-1);
const int LEN = 1e2;
int N, K;
bool zero(const ld& x) { return std::abs(x) < TOL; }

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
	Pos3D& operator += (const Pos3D& p) { x += p.x; y += p.y; z += p.z; return *this; }
	Pos3D& operator -= (const Pos3D& p) { x -= p.x; y -= p.y; z -= p.z; return *this; }
	Pos3D& operator *= (const ld& scalar) { x *= scalar; y *= scalar; z *= scalar; return *this; }
	Pos3D& operator /= (const ld& scalar) { x /= scalar; y /= scalar; z /= scalar; return *this; }
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
} pos[LEN];
const Pos3D O3D = { 0, 0, 0 };
Pos3D cross(const Pos3D& d1, const Pos3D& d2, const Pos3D& d3) { return (d2 - d1) / (d3 - d2); }
void brute() {//O(N^4)
	std::cin.tie(0)->sync_with_stdio(0);
	std::cout.tie(0);
	std::cout << std::fixed;
	std::cout.precision(6);
	std::cin >> N >> K;
	for (int i = 0; i < N; i++) std::cin >> pos[i];

	int A1, B1, A2, B2;
	ld total, Adots = 0, Bdots, Atotal, Btotal;
	Pos3D Aside = { 0, 0, 0 }, Bside, Avec, Bvec;

	if (K == 1) {
		for (int i = 0; i < N; i++) Aside += pos[i];
		Aside /= N;
		for (int i = 0; i < N; i++) Adots += (pos[i] - Aside).Euc();
		std::cout << Adots << "\n";
		return;
	}

	//if (K == 2)
	if (N == 1 || N == 2) { std::cout << "0.000000\n"; return; }

	total = INF;
	for (int i = 0; i < N; i++) {//O(N^4)
		for (int j = i + 1; j < N; j++) {
			for (int k = j + 1; k < N; k++) {
				Pos3D& d1 = pos[i], d2 = pos[j], d4 = pos[k];
				Pos3D nrm = cross(d1, d2, d4);

				A1 = 0, B1 = 0;//ptr
				Adots = 0, Bdots = 0;//dot_produtc_sum
				Aside = O3D, Bside = O3D;//vec_sum

				for (int l = 0; l < N; l++) {
					if (l == i || l == j || l == k) continue;
					Pos3D vec = pos[l] - d1;
					if (nrm * vec > 0) {
						Aside += pos[l];
						Adots += pos[l] * pos[l];
						A1++;
					}
					else {//nrm * vec < 0
						Bside += pos[l];
						Bdots += pos[l] * pos[l];
						B1++;
					}
				}

				for (int t = 0; t < 8; t++) {//bitmasking
					A2 = A1, B2 = B1;//ptr
					Atotal = Adots, Btotal = Bdots;//dot_product_sum
					Avec = Aside, Bvec = Bside;//vec_sum

					if (t & 1) {//include d1 in A-side
						A2++;
						Atotal += d1 * d1;
						Avec += d1;
					}
					else {//include d1 in B-side
						B2++;
						Btotal += d1 * d1;
						Bvec += d1;
					}

					if (t & 2) {//include d2 in A-side
						A2++;
						Atotal += d2 * d2;
						Avec += d2;
					}
					else {//include d2 in B-side
						B2++;
						Btotal += d2 * d2;
						Bvec += d2;
					}

					if (t & 4) {//include d4 in A-side
						A2++;
						Atotal += d4 * d4;
						Avec += d4;
					}
					else {//include d4 in B-side
						B2++;
						Btotal += d4 * d4;
						Bvec += d4;
					}

					ld T = 0;
					if (A2)	T += Atotal - Avec * Avec / A2;
					if (B2)	T += Btotal - Bvec * Bvec / B2;
					total = std::min(total, T);
					//ld T = 0;
					//if (A2) {
					//	Avec /= A2;
					//	T += Atotal - (Avec * Avec * 2 * A2) + ((Avec * Avec) * A2);					
					//}
					//if (B2) {
					//	Bvec /= B2;
					//	T += Btotal - (Bvec * Bvec * 2 * B2) + ((Bvec * Bvec) * B2);
					//}
					//total = std::min(total, T);
				}
			}
		}
	}
	std::cout << total << "\n";
	return;
}
int main() { brute(); return 0; }//boj17978 Washer
