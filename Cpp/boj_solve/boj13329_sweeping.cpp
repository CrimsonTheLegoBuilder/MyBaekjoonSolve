#include <iostream>
#include <vector>
#include <algorithm>
#include <set>
#include <cmath>
typedef long long ll;
const int LEN = 100'001;
int N, M, v;
bool visible[LEN];

struct Pos {
	int x, y;
	int i;
	bool operator < (const Pos& p) const { return (ll)y * p.x < (ll)x * p.y; }
	//bool operator <= (const Pos& p) const { return y * p.x <= x * p.y; }
}s, e, S[LEN], E[LEN];
std::vector<Pos> meteor, A;

ll cross(const Pos& d1, const Pos& d2, const Pos& d3) {
	return (ll)(d2.x - d1.x) * (ll)(d3.y - d2.y) - (ll)(d2.y - d1.y) * (ll)(d3.x - d2.x);
}

struct Idx {
	int i;
	bool operator < (const Idx& x) const {
		if (i == x.i) return 0;
		//if (S[i] <= S[x.i] && E[x.i] <= E[i]) return cross(E[i], S[i], S[x.i]) > 0;
		//else if (S[x.i] <= S[i] && E[i] <= E[x.i]) return cross(E[x.i], S[x.i], S[i]) < 0;
		if (S[i] < S[x.i]) return cross(E[i], S[i], S[x.i]) > 0;
		else return cross(E[x.i], S[x.i], S[i]) < 0;
	}
};
std::set<Idx> I;



int main() {
	std::cin.tie(0)->sync_with_stdio(0);
	std::cout.tie(0);
	std::cin >> N;
	for (int i = 1; i <= N; i++) {
		std::cin >> M;
		meteor.resize(M);
		for (int j = 0; j < M; j++) { std::cin >> meteor[j].x >> meteor[j].y; }
		s = *std::min_element(meteor.begin(), meteor.end());
		e = *std::max_element(meteor.begin(), meteor.end());
		S[i] = s, E[i] = e;
		A.push_back({ s.x, s.y, i });
		A.push_back({ e.x, e.y, -i });
	}
	std::sort(A.begin(), A.end());

	for (const Pos& a : A) {
		if (a.i > 0) {
			I.insert({ a.i });
			v = (*I.begin()).i;
			visible[v] = 1;
		}
		else {
			I.erase({ -a.i });
			if (I.empty()) continue;
			v = (*I.begin()).i;
			visible[v] = 1;
		}
	}
	int cnt = 0;
	for (int i = 1; i <= N; i++) if (!visible[i]) cnt++;
	std::cout << cnt << "\n";
	return 0;
}


//struct Pos {
//	ll x, y;
//	int i;
//	bool operator < (const Pos& p) const {
//		if (y * p.x == x * p.y) return hypot(x, y) < hypot(p.x, p.y);
//		//if (y * p.x == x * p.y) {  //if the line connecting any two vertices of all convex polygons pass through the origin
//		//	if (i * p.i < 0) return hypot(x, y) < hypot(p.x, p.y);
//		//	if (i > 0) return hypot(x, y) < hypot(p.x, p.y);
//		//	if (i < 0) return hypot(x, y) > hypot(p.x, p.y);
//		//}
//		return y * p.x < x * p.y;
//	}
//	bool operator <= (const Pos& p) const { return y * p.x <= x * p.y; }
//}s, e, S[LEN], E[LEN];
//ll cross(const Pos& d1, const Pos& d2, const Pos& d3) {
//return (d2.x - d1.x) * (d3.y - d2.y) - (d2.y - d1.y) * (d3.x - d2.x);
//}
//struct Idx {
//	int i;
//	bool operator < (const Idx& x) const {
//		if (i == x.i) return 0;
//		if (S[i] <= S[x.i] && E[x.i] <= E[i]) return cross(E[i], S[i], S[x.i]) > 0;
//		else if (S[x.i] <= S[i] && E[i] <= E[x.i]) return cross(E[x.i], S[x.i], S[i]) < 0;
//		if (S[i] < S[x.i]) return cross(E[i], S[i], S[x.i]) > 0;
//		else return cross(E[x.i], S[x.i], S[i]) < 0;
//	}
//};
//std::set<Idx> I;
//Idx i = *I.begin();
/*
//for (const Pos& x : A) {
//	std::cout << x.x << " " << x.y << " " << x.i << "\n";
//}
//for (int i = 1; i <= N; i++) {
//	std::cout << S[i].x << " " << S[i].y << "\n";
//}
//std::cout << "DEBUG\n";
for (const Pos& a : A) {
	//std::cout << a.i << " start\n";
	if (a.i > 0) {
		I.insert({ a.i });
		//std::cout << a.i << " DEBUG 1\n";
		Idx i = *I.begin();
		//std::cout << i.i << " DEBUG 2\n";
		visible[i.i] = 1;
	}
	else {
		//std::cout << -a.i << " DEBUG 3\n";
		I.erase({ -a.i });
		if (I.empty()) continue;
		Idx i = *I.begin();
		//std::cout << i.i << " DEBUG 4\n";
		visible[i.i] = 1;
	}
}
//std::cout << "DEBUG\n";
int cnt = 0;
for (int i = 1; i <= N; i++) {
	//std::cout << i << " " << visiable[i] << "\n";
	if (!visible[i]) cnt++;
}
std::cout << cnt << "\n";
return 0;
}
*/