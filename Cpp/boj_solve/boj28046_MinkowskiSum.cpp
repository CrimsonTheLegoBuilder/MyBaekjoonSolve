#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <algorithm>
#include <vector>
#include <cmath>
#include <cstring>
typedef long long ll;
typedef double ld;
const int LEN = 1e5 + 1;
const ld TOL = 1e-7;
int N, M, Q;
bool zero(const ld& x) { return std::abs(x) < TOL; }
int dcmp(const ld& x) { return std::abs(x) < TOL ? 0 : x > 0 ? 1 : -1; }
ll nC2(const ll& x) { return ((x - 1) * x) >> 1; }

struct Pos {
	int x, y;
	Pos(int X = 0, int Y = 0) : x(X), y(Y) {}
	bool operator == (const Pos& p) const { return x == p.x && y == p.y; }
	bool operator != (const Pos& p) const { return x != p.x || y != p.y; }
	bool operator < (const Pos& p) const { return x == p.x ? y < p.y : x < p.x; }
	Pos operator + (const Pos& p) const { return { x + p.x, y + p.y }; }
	Pos operator - (const Pos& p) const { return { x - p.x, y - p.y }; }
	Pos operator * (const int& n) const { return { x * n, y * n }; }
	Pos operator / (const int& n) const { return { x / n, y / n }; }
	ll operator * (const Pos& p) const { return { (ll)x * p.x + (ll)y * p.y }; }
	ll operator / (const Pos& p) const { return { (ll)x * p.y - (ll)y * p.x }; }
	Pos& operator += (const Pos& p) { x += p.x; y += p.y; return *this; }
	Pos& operator -= (const Pos& p) { x -= p.x; y -= p.y; return *this; }
	Pos& operator *= (const int& scale) { x *= scale; y *= scale; return *this; }
	Pos& operator /= (const int& scale) { x /= scale; y /= scale; return *this; }
	Pos operator ~ () const { return { -y, x }; }
	ll operator ! () const { return (ll)x * y; }
	ll Euc() const { return x * x + y * y; }
	ll Man() const { return std::abs(x) + std::abs(y); }
	ld mag() const { return hypot(x, y); }
	friend std::istream& operator >> (std::istream& is, Pos& p) {
		is >> p.x >> p.y;
		return is;
	}
	friend std::ostream& operator << (std::ostream& os, const Pos& p) {
		os << p.x << " " << p.y << "\n";
		return os;
	}
}; const Pos O = { 0, 0 };
std::vector<Pos> H1, H2, MM, ML, MR;
ll cross(const Pos& d1, const Pos& d2, const Pos& d3) { return (d2 - d1) / (d3 - d2); }
ll dot(const Pos& d1, const Pos& d2, const Pos& d3) { return (d2 - d1) * (d3 - d2); }
int ccw(const Pos& d1, const Pos& d2, const Pos& d3) {
	ll ret = cross(d1, d2, d3);
	return !ret ? 0 : ret > 0 ? 1 : -1;
}
bool on_seg_strong(const Pos& d1, const Pos& d2, const Pos& d3) {
	return !ccw(d1, d2, d3) && dot(d1, d3, d2) >= 0;
}
std::vector<Pos> graham_scan(std::vector<Pos>& C) {
	std::vector<Pos> H;
	if (C.size() < 3) {
		std::sort(C.begin(), C.end());
		return C;
	}
	std::swap(C[0], *min_element(C.begin(), C.end()));
	std::sort(C.begin() + 1, C.end(), [&](const Pos& p, const Pos& q) -> bool {
		int ret = ccw(C[0], p, q);
		if (!ret) return (C[0] - p).Euc() < (C[0] - q).Euc();
		return ret > 0;
		}
	);
	//C.erase(unique(C.begin(), C.end()), C.end());
	int sz = C.size();
	for (int i = 0; i < sz; i++) {
		while (H.size() >= 2 && ccw(H[H.size() - 2], H.back(), C[i]) <= 0)
			H.pop_back();
		H.push_back(C[i]);
	}
	return H;
}
int inner_check_bi_search(const std::vector<Pos>& H, const Pos& p) {//convex
	int sz = H.size();
	//if (!sz) return -1;
	//if (sz == 1) return p == H[0] ? 0 : -1;
	//if (sz == 2) return on_seg_strong(H[0], H[1], p) ? 0 : -1;
	if (cross(H[0], H[1], p) < 0 || cross(H[0], H[sz - 1], p) > 0) return -1;
	if (on_seg_strong(H[0], H[1], p) || on_seg_strong(H[0], H[sz - 1], p)) return 0;
	int s = 0, e = sz - 1, m;
	while (s + 1 < e) {
		m = s + e >> 1;
		if (cross(H[0], H[m], p) > 0) s = m;
		else e = m;
	}
	if (cross(H[s], H[e], p) > 0) return 1;
	else if (on_seg_strong(H[s], H[e], p)) return 0;
	else return -1;
}
ll area(std::vector<Pos>& H) {
	Pos P = { 0, 0 };
	ll ret = 0;
	int h = H.size();
	for (int i = 0; i < h; i++) {
		Pos cur = H[i], nxt = H[(i + 1) % h];
		ret += cross(P, cur, nxt);
	}
	return ret;
}
void norm(std::vector<Pos>& H) { if (area(H) < 0) std::reverse(H.begin(), H.end()); }
std::vector<Pos> Minkowski_sum(std::vector<Pos>& A, std::vector<Pos>& B, const bool& f = 0) {
	int sza = A.size();
	int szb = B.size();
	std::vector<Pos> minkowsum;

	if (f) { for (Pos& p : A) p *= 2; for (Pos& p : B) p *= -1; }
	int a = 0, b = 0;
	for (int i = 0; i < sza; i++) if (A[i] < A[a]) a = i;
	for (int j = 0; j < szb; j++) if (B[j] < B[b]) b = j; 
	minkowsum.push_back(A[a] + B[b]);
	//std::cout << sza + szb << "\n";
	for (int k = 1; k < sza + szb; k++) {
		//std::cout << "DEBUG\n";
		Pos a1 = A[(a + 1) % sza] + B[b];
		Pos b1 = B[(b + 1) % szb] + A[a];
		if (ccw(minkowsum.back(), a1, b1) > 0) minkowsum.push_back(a1), a = (a + 1) % sza;// , std::cout << a1;
		else minkowsum.push_back(b1), b = (b + 1) % szb;// , std::cout << b1;
	}

	if (f) { for (Pos& p : A) p /= 2; for (Pos& p : B) p *= -1; }
	return graham_scan(minkowsum);
}
void query() {
	Pos p;
	std::cin >> p;
	if (inner_check_bi_search(MM, p * 2) >= 0 ||
		inner_check_bi_search(ML, p) >= 0 ||
		inner_check_bi_search(MR, p) >= 0) std::cout << "Y";
	else std::cout << "N";
	return;
}
void solve() {
	std::cin.tie(0)->sync_with_stdio(0);
	std::cout.tie(0);
	std::cin >> N;
	H1.resize(N);
	for (int i = 0; i < N; i++) std::cin >> H1[i];
	//norm(H1);
	std::cin >> M;
	H2.resize(M);
	//norm(H2);
	for (int j = 0; j < M; j++) std::cin >> H2[j];
	MM = Minkowski_sum(H1, H2);
	ML = Minkowski_sum(H1, H2, 1);
	MR = Minkowski_sum(H2, H1, 1);
	std::cin >> Q;
	while (Q--) query();
	return;
}
int main() { solve(); return 0; }//boj28046 Gravitational Wave Detector

//#define _CRT_SECURE_NO_WARNINGS
//#include <iostream>
//#include <algorithm>
//#include <vector>
//#include <cmath>
//#include <cstring>
//typedef long long ll;
//typedef double ld;
//const int LEN = 1e5 + 1;
//const ld TOL = 1e-7;
//int N, M, Q;
//bool zero(const ld& x) { return std::abs(x) < TOL; }
//int dcmp(const ld& x) { return std::abs(x) < TOL ? 0 : x > 0 ? 1 : -1; }
//ll nC2(const ll& x) { return ((x - 1) * x) >> 1; }
//
//struct Pos {
//	ll x, y;
//	Pos(ll X = 0, ll Y = 0) : x(X), y(Y) {}
//	bool operator == (const Pos& p) const { return x == p.x && y == p.y; }
//	bool operator != (const Pos& p) const { return x != p.x || y != p.y; }
//	bool operator < (const Pos& p) const { return x == p.x ? y < p.y : x < p.x; }
//	Pos operator + (const Pos& p) const { return { x + p.x, y + p.y }; }
//	Pos operator - (const Pos& p) const { return { x - p.x, y - p.y }; }
//	Pos operator * (const ll& n) const { return { x * n, y * n }; }
//	Pos operator / (const ll& n) const { return { x / n, y / n }; }
//	ll operator * (const Pos& p) const { return { x * p.x + y * p.y }; }
//	ll operator / (const Pos& p) const { return { x * p.y - y * p.x }; }
//	Pos& operator += (const Pos& p) { x += p.x; y += p.y; return *this; }
//	Pos& operator -= (const Pos& p) { x -= p.x; y -= p.y; return *this; }
//	Pos& operator *= (const ll& scale) { x *= scale; y *= scale; return *this; }
//	Pos& operator /= (const ll& scale) { x /= scale; y /= scale; return *this; }
//	Pos operator ~ () const { return { -y, x }; }
//	ll operator ! () const { return x * y; }
//	ll Euc() const { return x * x + y * y; }
//	ll Man() const { return std::abs(x) + std::abs(y); }
//	ld mag() const { return hypot(x, y); }
//	friend std::istream& operator >> (std::istream& is, Pos& p) {
//		is >> p.x >> p.y;
//		return is;
//	}
//	friend std::ostream& operator << (std::ostream& os, const Pos& p) {
//		os << p.x << " " << p.y << "\n";
//		return os;
//	}
//}; const Pos O = { 0, 0 };
//std::vector<Pos> H1, H2, MM, ML, MR;
//ll cross(const Pos& d1, const Pos& d2, const Pos& d3) { return (d2 - d1) / (d3 - d2); }
//ll dot(const Pos& d1, const Pos& d2, const Pos& d3) { return (d2 - d1) * (d3 - d2); }
//int ccw(const Pos& d1, const Pos& d2, const Pos& d3) {
//	ll ret = cross(d1, d2, d3);
//	return !ret ? 0 : ret > 0 ? 1 : -1;
//}
//bool on_seg_strong(const Pos& d1, const Pos& d2, const Pos& d3) {
//	return !ccw(d1, d2, d3) && dot(d1, d3, d2) >= 0;
//}
//std::vector<Pos> graham_scan(std::vector<Pos>& C) {
//	std::vector<Pos> H;
//	if (C.size() < 3) {
//		std::sort(C.begin(), C.end());
//		return C;
//	}
//	std::swap(C[0], *min_element(C.begin(), C.end()));
//	std::sort(C.begin() + 1, C.end(), [&](const Pos& p, const Pos& q) -> bool {
//		int ret = ccw(C[0], p, q);
//		if (!ret) return (C[0] - p).Euc() < (C[0] - q).Euc();
//		return ret > 0;
//		}
//	);
//	//C.erase(unique(C.begin(), C.end()), C.end());
//	int sz = C.size();
//	for (int i = 0; i < sz; i++) {
//		while (H.size() >= 2 && ccw(H[H.size() - 2], H.back(), C[i]) <= 0)
//			H.pop_back();
//		H.push_back(C[i]);
//	}
//	return H;
//}
//int inner_check_bi_search(const std::vector<Pos>& H, const Pos& p) {//convex
//	int sz = H.size();
//	//if (!sz) return -1;
//	//if (sz == 1) return p == H[0] ? 0 : -1;
//	//if (sz == 2) return on_seg_strong(H[0], H[1], p) ? 0 : -1;
//	if (cross(H[0], H[1], p) < 0 || cross(H[0], H[sz - 1], p) > 0) return -1;
//	if (on_seg_strong(H[0], H[1], p) || on_seg_strong(H[0], H[sz - 1], p)) return 0;
//	int s = 0, e = sz - 1, m;
//	while (s + 1 < e) {
//		m = s + e >> 1;
//		if (cross(H[0], H[m], p) > 0) s = m;
//		else e = m;
//	}
//	if (cross(H[s], H[e], p) > 0) return 1;
//	else if (on_seg_strong(H[s], H[e], p)) return 0;
//	else return -1;
//}
//ll area(std::vector<Pos>& H) {
//	Pos P = { 0, 0 };
//	ll ret = 0;
//	int h = H.size();
//	for (int i = 0; i < h; i++) {
//		Pos cur = H[i], nxt = H[(i + 1) % h];
//		ret += cross(P, cur, nxt);
//	}
//	return ret;
//}
//void norm(std::vector<Pos>& H) { if (area(H) < 0) std::reverse(H.begin(), H.end()); }
//std::vector<Pos> Minkowski_sum(std::vector<Pos>& A, std::vector<Pos>& B, const bool& f = 0) {
//	int sza = A.size();
//	int szb = B.size();
//	std::vector<Pos> minkowsum;
//
//	if (f) { for (Pos& p : A) p *= 2; for (Pos& p : B) p *= -1; }
//	int a = 0, b = 0;
//	for (int i = 0; i < sza; i++) if (A[i] < A[a]) a = i;
//	for (int j = 0; j < szb; j++) if (B[j] < B[b]) b = j; 
//	minkowsum.push_back(A[a] + B[b]);
//	//std::cout << sza + szb << "\n";
//	for (int k = 1; k < sza + szb; k++) {
//		//std::cout << "DEBUG\n";
//		Pos a1 = A[(a + 1) % sza] + B[b];
//		Pos b1 = B[(b + 1) % szb] + A[a];
//		if (ccw(minkowsum.back(), a1, b1) > 0) minkowsum.push_back(a1), a = (a + 1) % sza;// , std::cout << a1;
//		else minkowsum.push_back(b1), b = (b + 1) % szb;// , std::cout << b1;
//	}
//
//	if (f) { for (Pos& p : A) p /= 2; for (Pos& p : B) p *= -1; }
//	return graham_scan(minkowsum);
//}
//void query() {
//	Pos p;
//	std::cin >> p;
//	if (inner_check_bi_search(MM, p * 2) >= 0 ||
//		inner_check_bi_search(ML, p) >= 0 ||
//		inner_check_bi_search(MR, p) >= 0) std::cout << "Y";
//	else std::cout << "N";
//	return;
//}
//void solve() {
//	std::cin.tie(0)->sync_with_stdio(0);
//	std::cout.tie(0);
//	std::cin >> N;
//	H1.resize(N);
//	for (int i = 0; i < N; i++) std::cin >> H1[i];
//	//norm(H1);
//	std::cin >> M;
//	H2.resize(M);
//	//norm(H2);
//	for (int j = 0; j < M; j++) std::cin >> H2[j];
//	MM = Minkowski_sum(H1, H2);
//	ML = Minkowski_sum(H1, H2, 1);
//	MR = Minkowski_sum(H2, H1, 1);
//	std::cin >> Q;
//	while (Q--) query();
//	return;
//}
//int main() { solve(); return 0; }//boj28046 Gravitational Wave Detector