#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <algorithm>
#include <cmath>
#include <cstring>
#include <cassert>
#include <vector>
typedef long long ll;
typedef double ld;
const ll INF = 1e17;
const int LEN = 1e5 + 1;
const ll MOD = 1'000'000'007;
int N, M, Q;

struct Pos {
	int x, y;
	Pos(int X = 0, int Y = 0) : x(X), y(Y) {}
	bool operator == (const Pos& p) const { return x == p.x && y == p.y; }
	bool operator != (const Pos& p) const { return x != p.x || y != p.y; }
	bool operator < (const Pos& p) const { return x == p.x ? y < p.y : x < p.x; }
	bool operator <= (const Pos& p) const { return x == p.x ? y <= p.y : x <= p.x; }
	Pos operator + (const Pos& p) const { return { x + p.x, y + p.y }; }
	Pos operator - (const Pos& p) const { return { x - p.x, y - p.y }; }
	Pos operator * (const int& n) const { return { x * n, y * n }; }
	Pos operator / (const int& n) const { return { x / n, y / n }; }
	ll operator * (const Pos& p) const { return { (ll)x * p.x + (ll)y * p.y }; }
	ll operator / (const Pos& p) const { return { (ll)x * p.y - (ll)y * p.x }; }
	ll Euc() const { return (ll)x * x + (ll)y * y; }
	friend std::istream& operator >> (std::istream& is, Pos& p) { is >> p.x >> p.y; return is; }
	friend std::ostream& operator << (std::ostream& os, const Pos& p) { os << p.x << " " << p.y; return os; }
}; const Pos O = Pos(0, 0);
typedef std::vector<Pos> Polygon;
std::vector<Pos> C, H;
ll cross(const Pos& d1, const Pos& d2, const Pos& d3) { return (d2 - d1) / (d3 - d2); }
ll dot(const Pos& d1, const Pos& d2, const Pos& d3) { return (d2 - d1) * (d3 - d2); }
int ccw(const Pos& d1, const Pos& d2, const Pos& d3) {
	ll ret = cross(d1, d2, d3);
	return !ret ? 0 : ret > 0 ? 1 : -1;
}
bool on_seg_strong(const Pos& d1, const Pos& d2, const Pos& d3) {
	return !ccw(d1, d2, d3) && dot(d1, d3, d2) >= 0;
}
int inner_check_bi_search(const std::vector<Pos>& H, const Pos& p) {//convex
	int sz = H.size();
	if (!sz) return -1;
	if (sz == 1) return p == H[0] ? 0 : -1;
	if (sz == 2) return on_seg_strong(H[0], H[1], p) ? 0 : -1;
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
	C.erase(unique(C.begin(), C.end()), C.end());
	int sz = C.size();
	for (int i = 0; i < sz; i++) {
		while (H.size() >= 2 && ccw(H[H.size() - 2], H.back(), C[i]) <= 0)
			H.pop_back();
		H.push_back(C[i]);
	}
	return H;
}
std::vector<Pos> UR, UL, DR, DL;
Polygon HUR, HUL, HDR, HDL;
void solve() {
	std::cin.tie(0)->sync_with_stdio(0);
	std::cout.tie(0);
	std::cin >> N;
	M = N;
	int x1, y1, x2, y2;
	ll u = INF, r = INF, l = -INF, d = -INF;
	while (M--) {
		std::cin >> x1 >> y1 >> x2 >> y2;
		assert(x1 != x2);
		assert(y1 != y2);
		u = std::min((ll)y2, u);
		r = std::min((ll)x2, r);
		l = std::max((ll)x1, l);
		d = std::max((ll)y1, d);
		UR.push_back(Pos(x2, y2));
		UL.push_back(Pos(x1, y2));
		DR.push_back(Pos(x2, y1));
		DL.push_back(Pos(x1, y1));
	}

	if (N < 3 || u >= d || r >= l) { std::cout << "possible\n"; return; }

	Pos ex1, ex2;

	ex1 = *std::min_element(UR.begin(), UR.end()), ex1.y = MOD;
	ex2 = *std::max_element(UR.begin(), UR.end()), ex2.y = MOD;
	UR.push_back(ex1); UR.push_back(ex2);
	HUR = graham_scan(UR);

	ex1 = *std::min_element(UL.begin(), UL.end()), ex1.y = MOD;
	ex2 = *std::max_element(UL.begin(), UL.end()), ex2.y = MOD;
	UL.push_back(ex1); UL.push_back(ex2);
	HUL = graham_scan(UL);

	ex1 = *std::min_element(DR.begin(), DR.end()), ex1.y = -MOD;
	ex2 = *std::max_element(DR.begin(), DR.end()), ex2.y = -MOD;
	DR.push_back(ex1); DR.push_back(ex2);
	HDR = graham_scan(DR);

	ex1 = *std::min_element(DL.begin(), DL.end()), ex1.y = -MOD;
	ex2 = *std::max_element(DL.begin(), DL.end()), ex2.y = -MOD;
	DL.push_back(ex1); DL.push_back(ex2);
	HDL = graham_scan(DL);

	l = 0, r = 0;

	for (const Pos& p : HUR) {
		if (inner_check_bi_search(HDL, p) > 0) {
			l = 1;
			break;
		}
	}
	if (!l) {
		for (const Pos& p : HDL) {
			if (inner_check_bi_search(HUR, p) > 0) {
				l = 1;
				break;
			}
		}
	}

	for (const Pos& p : HUL) {
		if (inner_check_bi_search(HDR, p) > 0) {
			r = 1;
			break;
		}
	}
	if (!r) {
		for (const Pos& p : HDR) {
			if (inner_check_bi_search(HUL, p) > 0) {
				r = 1;
				break;
			}
		}
	}

	std::cout << (l && r ? "impossible\n" : "possible\n");
	return;
}
int main() { solve(); return 0; }//Faster Than Light NWERC 2022 F boj26179


//#define _CRT_SECURE_NO_WARNINGS
//#include <iostream>
//#include <algorithm>
//#include <cmath>
//#include <cstring>
//#include <cassert>
//#include <vector>
//#include <random>
//#include <array>
//#include <tuple>
//typedef long long ll;
//typedef double ld;
//const ll INF = 1e17;
//const int LEN = 1e5 + 1;
//const ld TOL = 1e-7;
//const ll MOD = 1'000'000'007;
//int N, M, Q, T;
//bool zero(const ld& x) { return std::abs(x) < TOL; }
//int sign(const ld& x) { return x < -TOL ? -1 : x > TOL; }
//int sign(const ll& x) { return x < 0 ? -1 : !!x; }
//
////#define AUTO_CHECK
//
//struct Pos {
//	int x, y;
//	Pos(int X = 0, int Y = 0) : x(X), y(Y) {}
//	bool operator == (const Pos& p) const { return x == p.x && y == p.y; }
//	bool operator != (const Pos& p) const { return x != p.x || y != p.y; }
//	bool operator < (const Pos& p) const { return x == p.x ? y < p.y : x < p.x; }
//	bool operator <= (const Pos& p) const { return x == p.x ? y <= p.y : x <= p.x; }
//	Pos operator + (const Pos& p) const { return { x + p.x, y + p.y }; }
//	Pos operator - (const Pos& p) const { return { x - p.x, y - p.y }; }
//	Pos operator * (const int& n) const { return { x * n, y * n }; }
//	Pos operator / (const int& n) const { return { x / n, y / n }; }
//	ll operator * (const Pos& p) const { return { (ll)x * p.x + (ll)y * p.y }; }
//	ll operator / (const Pos& p) const { return { (ll)x * p.y - (ll)y * p.x }; }
//	ll operator ^ (const Pos& p) const { return { (ll)x * p.y - (ll)y * p.x }; }
//	Pos& operator += (const Pos& p) { x += p.x; y += p.y; return *this; }
//	Pos& operator -= (const Pos& p) { x -= p.x; y -= p.y; return *this; }
//	Pos& operator *= (const int& scale) { x *= scale; y *= scale; return *this; }
//	Pos& operator /= (const int& scale) { x /= scale; y /= scale; return *this; }
//	Pos operator - () const { return { -x, -y }; }
//	Pos operator ~ () const { return { -y, x }; }
//	Pos operator ! () const { return { y, x }; }
//	ll xy() const { return (ll)x * y; }
//	ll Euc() const { return (ll)x * x + (ll)y * y; }
//	friend std::istream& operator >> (std::istream& is, Pos& p) { is >> p.x >> p.y; return is; }
//	friend std::ostream& operator << (std::ostream& os, const Pos& p) { os << p.x << " " << p.y; return os; }
//}; const Pos O = Pos(0, 0);
//typedef std::vector<Pos> Polygon;
//std::vector<Pos> C, H;
//ll cross(const Pos& d1, const Pos& d2, const Pos& d3) { return (d2 - d1) ^ (d3 - d2); }
//ll dot(const Pos& d1, const Pos& d2, const Pos& d3) { return (d2 - d1) * (d3 - d2); }
//int ccw(const Pos& d1, const Pos& d2, const Pos& d3) {
//	ll ret = cross(d1, d2, d3);
//	return !ret ? 0 : ret > 0 ? 1 : -1;
//}
//bool on_seg_strong(const Pos& d1, const Pos& d2, const Pos& d3) {
//	return !ccw(d1, d2, d3) && dot(d1, d3, d2) >= 0;
//}
//int inner_check_bi_search(const std::vector<Pos>& H, const Pos& p) {//convex
//	int sz = H.size();
//	if (!sz) return -1;
//	if (sz == 1) return p == H[0] ? 0 : -1;
//	if (sz == 2) return on_seg_strong(H[0], H[1], p) ? 0 : -1;
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
//	C.erase(unique(C.begin(), C.end()), C.end());
//	int sz = C.size();
//	for (int i = 0; i < sz; i++) {
//		while (H.size() >= 2 && ccw(H[H.size() - 2], H.back(), C[i]) <= 0)
//			H.pop_back();
//		H.push_back(C[i]);
//	}
//	return H;
//}
//std::vector<Pos> UR, UL, DR, DL;
//Polygon HUR, HUL, HDR, HDL;
//
//#ifdef AUTO_CHECK
//#include <fstream>
//std::string solve(const std::string& input_file) {
//#else
//void solve() {
//	std::cin.tie(0)->sync_with_stdio(0);
//	std::cout.tie(0);
//#endif
//	UR.clear();
//	UL.clear();
//	DR.clear();
//	DL.clear();
//#ifdef AUTO_CHECK
//	std::ifstream input(input_file);
//	input >> N;
//	M = N;
//	int x1, y1, x2, y2;
//	ll u = INF, r = INF, l = -INF, d = -INF;
//	while (M--) {
//		input >> x1 >> y1 >> x2 >> y2;
//#else
//	//freopen("../../../input_data/faster/005-gen_jorke1.in", "r", stdin);
//	std::cin >> N;
//	M = N;
//	int x1, y1, x2, y2;
//	ll u = INF, r = INF, l = -INF, d = -INF;
//	while (M--) {
//		std::cin >> x1 >> y1 >> x2 >> y2;
//#endif
//		assert(x1 != x2);
//		assert(y1 != y2);
//		u = std::min((ll)y2, u);
//		r = std::min((ll)x2, r);
//		l = std::max((ll)x1, l);
//		d = std::max((ll)y1, d);
//		UR.push_back(Pos(x2, y2));
//		UL.push_back(Pos(x1, y2));
//		DR.push_back(Pos(x2, y1));
//		DL.push_back(Pos(x1, y1));
//	}
//
//	if (N < 3 || u >= d || r >= l) {
//#ifdef AUTO_CHECK
//		return "possible";
//#else
//		std::cout << "possible\n"; return; 
//#endif
//	}
//
//	Pos ex1, ex2;
//
//	ex1 = *std::min_element(UR.begin(), UR.end()), ex1.y = MOD;
//	ex2 = *std::max_element(UR.begin(), UR.end()), ex2.y = MOD;
//	UR.push_back(ex1); UR.push_back(ex2);
//	HUR = graham_scan(UR);
//
//	ex1 = *std::min_element(UL.begin(), UL.end()), ex1.y = MOD;
//	ex2 = *std::max_element(UL.begin(), UL.end()), ex2.y = MOD;
//	UL.push_back(ex1); UL.push_back(ex2);
//	HUL = graham_scan(UL);
//
//	ex1 = *std::min_element(DR.begin(), DR.end()), ex1.y = -MOD;
//	ex2 = *std::max_element(DR.begin(), DR.end()), ex2.y = -MOD;
//	DR.push_back(ex1); DR.push_back(ex2);
//	HDR = graham_scan(DR);
//
//	ex1 = *std::min_element(DL.begin(), DL.end()), ex1.y = -MOD;
//	ex2 = *std::max_element(DL.begin(), DL.end()), ex2.y = -MOD;
//	DL.push_back(ex1); DL.push_back(ex2);
//	HDL = graham_scan(DL);
//
//	l = 0, r = 0;
//	for (const Pos& p : HUR) {
//		if (inner_check_bi_search(HDL, p) > 0) {
//			l = 1;
//			break;
//		}
//	}
//	if (!l) {
//		for (const Pos& p : HDL) {
//			if (inner_check_bi_search(HUR, p) > 0) {
//				l = 1;
//				break;
//			}
//		}
//	}
//
//	for (const Pos& p : HUL) {
//		if (inner_check_bi_search(HDR, p) > 0) {
//			r = 1;
//			break;
//		}
//	}
//	if (!r) {
//		for (const Pos& p : HDR) {
//			if (inner_check_bi_search(HUL, p) > 0) {
//				r = 1;
//				break;
//			}
//		}
//	}
//
//#ifdef AUTO_CHECK
//	return l && r ? "impossible" : "possible";
//#else
//	std::cout << (l && r ? "impossible\n" : "possible\n");
//#endif
//}
//
//std::vector<std::string> file_names;
//
//int main(int argc, char* argv[]) {
//#ifdef AUTO_CHECK
//	if (argc < 2) {
//		std::cerr << "Usage: " << argv[0] << " <input_file>" << std::endl;
//		return 1;
//	}
//
//	std::ifstream file_list(argv[1]);
//	std::vector<std::string> file_names;
//	std::string file_name;
//	while (file_list >> file_name) {
//		// fuck
//		file_names.push_back(file_name);
//	}
//
//	std::ofstream result_file("fucked_results.txt");  
//	
//	for (int i = 0; i < file_names.size(); i += 2) {
//		std::string output_file = file_names[i];
//		std::string input_file = file_names[i + 1];
//
//		std::ifstream output_stream(output_file);
//		std::string answer;
//		output_stream >> answer;
//
//		std::string result;
//		result = solve(input_file);
//
//		std::cout << input_file << ' ' << output_file << '\n';
//
//		std::cout << answer << ' ' << result << ' ' << (answer == result) << '\n';
//		if (answer != result) {
//			std::cout << "what the fuck?! wrong answer is returned!!\n";
//			std::cout << "the file name is...\n";
//			std::cout << "	" + input_file << ", you idiot.\n";
//			result_file << input_file << ' ' << N << '\n';
//			result_file << answer << ' ' << result << '\n';
//		}
//	}
//	result_file.close();
//#else
//	solve(); 
//#endif
//	return 0;
//}//Faster Than Light boj26179
