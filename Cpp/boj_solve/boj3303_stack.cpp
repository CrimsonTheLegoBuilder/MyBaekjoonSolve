#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <algorithm>
#include <cmath>
#include <cstring>
#include <cassert>
#include <vector>
typedef long long ll;
//typedef double ld;
const ll INF = 1e17;
const int LEN = 2e5 + 1;
int N;
bool V[LEN];

struct Pos {
	int x, y, i;
	Pos(int X = 0, int Y = 0, int I = 0) : x(X), y(Y), i(I) {}
	bool operator == (const Pos& p) const { return x == p.x && y == p.y; }
	bool operator != (const Pos& p) const { return x != p.x || y != p.y; }
	ll operator * (const Pos& p) const { return { (ll)x * p.x + (ll)y * p.y }; }
	ll operator / (const Pos& p) const { return { (ll)x * p.y - (ll)y * p.x }; }
	bool operator < (const Pos& p) const { return *this / p > 0; }
	bool operator <= (const Pos& p) const { return *this / p >= 0; }
	Pos operator + (const Pos& p) const { return { x + p.x, y + p.y }; }
	Pos operator - (const Pos& p) const { return { x - p.x, y - p.y }; }
	ll Euc() const { return (ll)x * x + (ll)y * y; }
	friend std::istream& operator >> (std::istream& is, Pos& p) {
		is >> p.x >> p.y;
		return is;
	}
	friend std::ostream& operator << (std::ostream& os, const Pos& p) {
		os << p.x << " " << p.y;
		return os;
	}
}; const Pos O = { 0, 0 };
std::vector<Pos> C, H;
ll cross(const Pos& d1, const Pos& d2, const Pos& d3) { return (d2 - d1) / (d3 - d2); }
int ccw(const Pos& d1, const Pos& d2, const Pos& d3) {
	ll ret = cross(d1, d2, d3);
	return !ret ? 0 : ret > 0 ? 1 : -1;
}
ll area(const std::vector<Pos>& H) {
	ll ret = 0;
	int sz = H.size();
	for (int i = 0; i < sz; i++) {
		Pos cur = H[i], nxt = H[(i + 1) % sz];
		ret += cross(O, cur, nxt);
	}
	return ret;
}
bool norm(std::vector<Pos>& H) {
	if (area(H) > 0) {
		std::reverse(H.begin(), H.end());
		return 1;
	}
	return 0;
}
int invisible(const Pos& p1, const Pos& p2, const Pos& t) {
	if (p1 / t == 0 && p1.Euc() <= t.Euc()) return 1;
	if (p2 / t == 0 && p2.Euc() <= t.Euc()) return 1;
	return ccw(O, p1, t) < 0 && ccw(O, p2, t) > 0 && ccw(p1, p2, t) > 0;
}
std::vector<int> stack;
void solve() {
	std::cin.tie(0)->sync_with_stdio(0);
	std::cout.tie(0);
	memset(V, 0, sizeof V);
	std::cin >> N;
	H.resize(N);
	for (int i = 0; i < N; i++) std::cin >> H[i], H[i].i = i + 1;
	norm(H);//normalization cw

	int r = 0, l = 0;
	ll LMIN = H[0].Euc(), RMIN = H[0].Euc();
	for (int i = 0; i < N; i++) {
		if (H[r] / H[i] < 0 ||
			(H[r] / H[i] == 0 && RMIN > H[i].Euc())) {
			r = i, RMIN = H[i].Euc();
		}
		if (H[l] / H[i] > 0 ||
			(H[l] / H[i] == 0 && LMIN > H[i].Euc())) {
			l = i, LMIN = H[i].Euc();
		}
	}

	stack.clear();
	stack.push_back(r);
	V[H[r].i] = 1;

	bool fvis = 1, bvis = 1, rvs = 0;
	for (int i = r; i < r + N; i++) {
		if (i % N == l) break;
		if (fvis && bvis) {
			int CCW = ccw(H[(i - 1 + N) % N], H[i % N], H[(i + 1) % N]);
			if (H[i % N] / H[(i + 1) % N] < 0) {//move backward
				if (!rvs && CCW < 0) {
					rvs = 1;
					fvis = 0;
					continue;
				}
				rvs = 1;
				while (stack.size() && invisible(H[i % N], H[(i + 1) % N], H[stack.back() % N]))
					stack.pop_back();
			}
			else if (H[i % N] / H[(i + 1) % N] == 0) {//move vertical
				if (H[i % N].Euc() > H[(i + 1) % N].Euc()) {
					rvs = 0;
					if (stack[stack.size() - 1] == i) stack.pop_back();
					if (stack.size() < 1 || H[stack.back() % N] / H[(i + 1) % N] > 0)
						stack.push_back(i + 1);
				}
			}
			else if (H[i % N] / H[(i + 1) % N] > 0) {//move forward
				if (rvs && CCW > 0) {
					stack.push_back(i);
					rvs = 0;
					bvis = 0;
					continue;
				}
				rvs = 0;
				if (stack.size() < 1 || H[stack.back() % N] / H[(i + 1) % N] > 0)
					stack.push_back(i + 1);
			}
		}
		else if (!fvis) {
			if (H[stack.back() % N] / H[(i + 1) % N] > 0) {
				rvs = 0;
				fvis = 1;
				stack.push_back(i + 1);
			}
		}
		else if (!bvis) {
			if (H[stack.back() % N] / H[(i + 1) % N] < 0) {
				stack.pop_back();
				rvs = 1;
				bvis = 1;
				while (stack.size() && invisible(H[i % N], H[(i + 1) % N], H[stack.back() % N]))
					stack.pop_back();
			}
		}
	}
	for (const int& i : stack) V[H[i % N].i] = 1;
	std::cout << stack.size() << "\n";
	for (int i = 1; i <= N; i++) if (V[i]) std::cout << i << " ";
	return;
}
int main() { solve(); return 0; }//boj3303 Printed Circuit Board

/*
121 2 12 22 46 47 50 51 52 53 54
*/

//void solve() {
//	std::cin.tie(0)->sync_with_stdio(0);
//	std::cout.tie(0);
//	memset(V, 0, sizeof V);
//	std::cin >> N;
//	H.resize(N);
//	for (int i = 0; i < N; i++) std::cin >> H[i], H[i].i = i + 1;
//	norm(H);//normalization cw
//	int r = 0, l = 0;
//	ll LMIN = H[0].Euc(), RMIN = H[0].Euc();
//	for (int i = 0; i < N; i++) {
//		if (H[r] / H[i] < 0 ||
//			(H[r] / H[i] == 0 && RMIN > H[i].Euc())) {
//			r = i, RMIN = H[i].Euc();
//		}
//		if (H[l] / H[i] > 0 ||
//			(H[l] / H[i] == 0 && LMIN > H[i].Euc())) {
//			l = i, LMIN = H[i].Euc();
//		}
//	}
//
//	bool fvis = 1, bvis = 1, rvs = 0;
//	stack.clear();
//	stack.push_back(r);
//	V[H[r].i] = 1;// , V[H[l].i] = 1;
//	for (int i = r; i < r + N; i++) {
//		if (i % N == l) break;
//		//std::cout << i % N + 2 << " ";
//		//for (const int& j : stack) std::cout << H[j % N].i + 1 << " crd: " << H[j % N] << " :: ";
//		//std::cout << "\n";
//		//std::cout << fvis << "\n";
//		if (fvis && bvis) {
//			int CCW = ccw(H[(i - 1 + N) % N], H[i % N], H[(i + 1) % N]);
//			if (H[i % N] / H[(i + 1) % N] < 0) {//move backward
//				//std::cout << "DEBUG:: <\n";
//				if (bvis && !rvs && CCW < 0) {
//					rvs = 1;
//					fvis = 0;
//					continue;
//				}
//				//rvs = 1;
//				//if (bvis || (!bvis && H[stack.back() % N] / H[(i + 1) % N] < 0)) {
//				//	if (!bvis) stack.pop_back(), bvis = 1;
//				//	while (stack.size() && invisible(H[i % N], H[(i + 1) % N], H[stack.back() % N])) stack.pop_back();
//				//}
//				rvs = 1;
//				while (stack.size() && invisible(H[i % N], H[(i + 1) % N], H[stack.back() % N])) stack.pop_back();
//				
//			}
//			if (!bvis) continue;
//			else if (H[i % N] / H[(i + 1) % N] == 0) {//move vertical
//				//std::cout << "DEBUG:: ==\n";
//				if (H[i % N].Euc() > H[(i + 1) % N].Euc()) {
//					if (rvs) rvs = 0;
//					if (stack[stack.size() - 1] == i) stack.pop_back();
//					if (stack.size() < 1 || H[stack.back() % N] / H[(i + 1) % N] > 0)
//						stack.push_back(i + 1);
//				}
//			}
//			else if (H[i % N] / H[(i + 1) % N] > 0) {//move forward
//				//std::cout << "DEBUG:: >\n";
//				if (rvs && CCW > 0) {
//					//std::cout << "DEBUG:: rvs > CCW\n";
//					stack.push_back(i);
//					rvs = 0;
//					bvis = 0;
//					continue;
//				}
//				//std::cout << "DEBUG:: push_back(i)\n";
//				if (rvs && CCW < 0) {
//					//if (stack.size() < 1 || H[stack.back() % N] / H[(i + 1) % N] > 0) stack.push_back(i + 1);
//					rvs = 0;
//				}
//				//std::cout << "DEBUG:: push_back(i + 1)\n";
//				//std::cout << stack.size() << "\n";
//				if (stack.size() < 1 || H[stack.back() % N] / H[(i + 1) % N] > 0)
//					stack.push_back(i + 1);
//			}
//		}
//		else if (!fvis) {
//			if (H[stack.back() % N] / H[(i + 1) % N] > 0) {
//				rvs = 0;
//				fvis = 1;
//				stack.push_back(i + 1);
//			}
//		}
//		else if (!bvis) {
//			if (H[stack.back() % N] / H[(i + 1) % N] < 0) {
//				stack.pop_back();
//				rvs = 1;
//				bvis = 1;
//				while (stack.size() && invisible(H[i % N], H[(i + 1) % N], H[stack.back() % N])) stack.pop_back();
//			}
//		}
//	}
//	//for (const int& i : stack) std::cout << H[i % N].i + 1 << " ";
//	//std::cout << "\n";
//	for (const int& i : stack) V[H[i % N].i] = 1;
//	std::cout << stack.size() << "\n";
//	for (int i = 1; i <= N; i++) if (V[i]) std::cout << i << " ";
//	return;
//}
//int main() { solve(); return 0; }//boj3303 Printed Circuit Board

//#define _CRT_SECURE_NO_WARNINGS
//#include <iostream>
//#include <algorithm>
//#include <cmath>
//#include <cstring>
//#include <cassert>
//#include <vector>
//typedef long long ll;
////typedef double ld;
//const ll INF = 1e17;
//const int LEN = 2e5 + 1;
//int N;
//bool V[LEN];
//
//struct Pos {
//	int x, y, i;
//	Pos(int X = 0, int Y = 0, int I = 0) : x(X), y(Y), i(I) {}
//	bool operator == (const Pos& p) const { return x == p.x && y == p.y; }
//	bool operator != (const Pos& p) const { return x != p.x || y != p.y; }
//	ll operator * (const Pos& p) const { return { (ll)x * p.x + (ll)y * p.y }; }
//	ll operator / (const Pos& p) const { return { (ll)x * p.y - (ll)y * p.x }; }
//	bool operator < (const Pos& p) const { return *this / p > 0; }
//	bool operator <= (const Pos& p) const { return *this / p >= 0; }
//	Pos operator + (const Pos& p) const { return { x + p.x, y + p.y }; }
//	Pos operator - (const Pos& p) const { return { x - p.x, y - p.y }; }
//	ll Euc() const { return (ll)x * x + (ll)y * y; }
//	friend std::istream& operator >> (std::istream& is, Pos& p) {
//		is >> p.x >> p.y;
//		return is;
//	}
//	friend std::ostream& operator << (std::ostream& os, const Pos& p) {
//		os << p.x << " " << p.y;
//		return os;
//	}
//}; const Pos O = { 0, 0 };
//std::vector<Pos> C, H;
//ll cross(const Pos& d1, const Pos& d2, const Pos& d3) { return (d2 - d1) / (d3 - d2); }
//int ccw(const Pos& d1, const Pos& d2, const Pos& d3) {
//	ll ret = cross(d1, d2, d3);
//	return !ret ? 0 : ret > 0 ? 1 : -1;
//}
//ll area(const std::vector<Pos>& H) {
//	ll ret = 0;
//	int sz = H.size();
//	for (int i = 0; i < sz; i++) {
//		Pos cur = H[i], nxt = H[(i + 1) % sz];
//		ret += cross(O, cur, nxt);
//	}
//	return ret;
//}
//bool norm(std::vector<Pos>& H) {
//	if (area(H) > 0) {
//		std::reverse(H.begin(), H.end());
//		return 1;
//	}
//	return 0;
//}
//int invisible(const Pos& p1, const Pos& p2, const Pos& t) {
//	if (p1 / t == 0 && p1.Euc() <= t.Euc()) return 1;
//	if (p2 / t == 0 && p2.Euc() <= t.Euc()) return 1;
//	return ccw(O, p1, t) < 0 && ccw(O, p2, t) > 0 && ccw(p1, p2, t) > 0;
//}
//std::vector<int> stack;
//void solve() {
//	std::cin.tie(0)->sync_with_stdio(0);
//	std::cout.tie(0);
//	memset(V, 0, sizeof V);
//	std::cin >> N;
//	H.resize(N);
//	for (int i = 0; i < N; i++) std::cin >> H[i], H[i].i = i + 1;
//	norm(H);//sort cw
//	int r = 0, l = 0;
//	ll LMIN = H[0].Euc(), RMIN = H[0].Euc();
//	for (int i = 0; i < N; i++) {
//		if (H[r] / H[i] < 0 ||
//			(H[r] / H[i] == 0 && RMIN > H[i].Euc())) {
//			r = i, RMIN = H[i].Euc();
//		}
//		if (H[l] / H[i] > 0 ||
//			(H[l] / H[i] == 0 && LMIN > H[i].Euc())) {
//			l = i, LMIN = H[i].Euc();
//		}
//	}
//
//	bool fvis = 1, bvis = 1, rvs = 0;
//	stack.clear();
//	stack.push_back(r);
//	V[H[r].i] = 1;// , V[H[l].i] = 1;
//	for (int i = r; i < r + N; i++) {
//		if (i % N == l) break;
//		std::cout << i % N + 2 << " ";
//		for (const int& j : stack) std::cout << H[j % N].i + 1 << " crd: " << H[j % N] << " :: ";
//		std::cout << "\n";
//		std::cout << fvis << "\n";
//		if (fvis) {
//			int CCW = ccw(H[(i - 1 + N) % N], H[i % N], H[(i + 1) % N]);
//			if (H[i % N] / H[(i + 1) % N] < 0) {//move backward
//				std::cout << "DEBUG:: <\n";
//				if (bvis && !rvs && CCW < 0) {
//					rvs = 1;
//					fvis = 0;
//					continue;
//				}
//				rvs = 1;
//				if (bvis || (!bvis && H[stack.back() % N] / H[(i + 1) % N] < 0)) {
//					if (!bvis) stack.pop_back(), bvis = 1;
//					while (stack.size() && invisible(H[i % N], H[(i + 1) % N], H[stack.back() % N])) stack.pop_back();
//				}
//			}
//			if (!bvis) continue;
//			else if (H[i % N] / H[(i + 1) % N] == 0) {//move vertical
//				std::cout << "DEBUG:: ==\n";
//				if (H[i % N].Euc() > H[(i + 1) % N].Euc()) {
//					if (rvs) rvs = 0;
//					if (stack[stack.size() - 1] == i) stack.pop_back();
//					if (stack.size() < 1 || H[stack.back() % N] / H[(i + 1) % N] > 0)
//						stack.push_back(i + 1);
//				}
//			}
//			else if (H[i % N] / H[(i + 1) % N] > 0) {//move forward
//				std::cout << "DEBUG:: >\n";
//				if (rvs && CCW > 0) {
//					//std::cout << "DEBUG:: rvs > CCW\n";
//					stack.push_back(i);
//					rvs = 0;
//					bvis = 0;
//					continue;
//				}
//				//std::cout << "DEBUG:: push_back(i)\n";
//				if (rvs && CCW < 0) {
//					//if (stack.size() < 1 || H[stack.back() % N] / H[(i + 1) % N] > 0) stack.push_back(i + 1);
//					rvs = 0;
//				}
//				//std::cout << "DEBUG:: push_back(i + 1)\n";
//				//std::cout << stack.size() << "\n";
//				if (stack.size() < 1 || H[stack.back() % N] / H[(i + 1) % N] > 0)
//					stack.push_back(i + 1);
//			}
//		}
//		if (!fvis) {
//			if (H[stack.back() % N] / H[(i + 1) % N] > 0) {
//				rvs = 0;
//				fvis = 1;
//				stack.push_back(i + 1);
//			}
//		}
//	}
//	for (const int& i : stack) std::cout << H[i % N].i + 1 << " ";
//	std::cout << "\n";
//	for (const int& i : stack) V[H[i % N].i] = 1;
//	std::cout << stack.size() << "\n";
//	for (int i = 1; i <= N; i++) if (V[i]) std::cout << i << " ";
//	return;
//}
//int main() { solve(); return 0; }//boj3303 Printed Circuit Board