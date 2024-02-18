#include <iostream>
#include <algorithm>
#include <vector>
#include <queue>
#include <cmath>
typedef long long ll;
typedef long double ld;
const ld TOL = 1e-9;
const int LEN = 2000;
int graph[LEN][LEN];
bool MST[LEN]{};

std::vector<int> W;
struct Pos {
	int x, y;
}flowers[LEN];
int N;
struct Seq {
	int w, n;
	bool operator < (const Seq& s) const {
		if (w == s.w) return n < s.n;
		return w > s.w;
	}
};
std::priority_queue<Seq> H;

int dist(const Pos& u, const Pos& v) {
	return (u.x - v.x) * (u.x - v.x) + (u.y - v.y) * (u.y - v.y);
}
void prim(int v, int N) {
	int cnt = 0;
	H.push({ 0, v });

	while (!H.empty()) {
		Seq p = H.top(); H.pop();
		//std::cout << p.w << " " << p.n << " DEBUG \n";
		if (MST[p.n]) continue;

		//std::cout << p.w << " " << p.n << " DEBUG \n";
		MST[p.n] = 1;
		W.push_back(p.w);

		cnt++;
		if (cnt == N) return;

		for (int w = 0; w < N; w++) {
			if (graph[p.n][w] > .5 && !MST[w]) {
				H.push({ graph[p.n][w], w });
			}
		}
	}
}


int main() {
	std::cin.tie(0)->sync_with_stdio(0);
	std::cout.tie(0);
	std::cout << std::fixed;
	std::cout.precision(2);
	int F, B;
	std::cin >> F >> B;
	for (int i = 0; i < F; i++) {
		std::cin >> flowers[i].x >> flowers[i].y;
	}
	for (int i = 0; i < F; i++) {
		for (int j = i; j < F; j++) {
			if (i == j) graph[i][j] = -1;
			else {
				int D = dist(flowers[i], flowers[j]);
				graph[i][j] = D;
				graph[j][i] = D;
				//std::cout << D << " DEBUG\n";
			}
		}
	}
	//std::cout << "DEBUG\n";
	prim(0, F);
	//std::cout << "DEBUG\n";
	//for (const ld w : W) {
	//	std::cout << w << " DEBUG\n";
	//}
	std::sort(W.begin(), W.end());
	int I = F - B;
	std::cout << sqrtl(W[I]) << "\n";
	return 0;
}

//#include <iostream>
//#include <algorithm>
//#include <vector>
//#include <queue>
//#include <cmath>
//typedef long long ll;
//typedef long double ld;
//const ld TOL = 1e-9;
//const int LEN = 2000;
//ld graph[LEN][LEN];
//bool MST[LEN]{};
//
//std::vector<ld> W;
//struct Pos {
//	ld x, y;
//}flowers[LEN];
//int N;
//struct Seq {
//	ld w;
//	int n;
//	bool operator < (const Seq& s) const {
//		if (std::abs(w - s.w) < TOL) return n < s.n;
//		return w > s.w;
//	}
//};
//std::priority_queue<Seq> H;
//
//ld dist(const Pos& u, const Pos& v) {
//	return sqrtl((u.x - v.x) * (u.x - v.x) + (u.y - v.y) * (u.y - v.y));
//}
//void prim(int v, int N) {
//	int cnt = 0;
//	H.push({0, v});
//
//	while (!H.empty()) {
//		Seq p = H.top(); H.pop();
//		//std::cout << p.w << " " << p.n << " DEBUG \n";
//		if (MST[p.n]) continue;
//
//		//std::cout << p.w << " " << p.n << " DEBUG \n";
//		MST[p.n] = 1;
//		W.push_back(p.w);
//
//		cnt++;
//		if (cnt == N) return;
//
//		for (int w = 0; w < N; w++) {
//			if (graph[p.n][w] > .5 && !MST[w]) {
//				H.push({ graph[p.n][w], w });
//			}
//		}
//	}
//}
//
//
//int main() {
//	std::cin.tie(0)->sync_with_stdio(0);
//	std::cout.tie(0);
//	std::cout << std::fixed;
//	std::cout.precision(2);
//	int F, B;
//	std::cin >> F >> B;
//	for (int i = 0; i < F; i++) {
//		std::cin >> flowers[i].x >> flowers[i].y;
//	}
//	for (int i = 0; i < F; i++) {
//		for (int j = i; j < F; j++) {
//			if (i == j) graph[i][j] = -1;
//			else {
//				ld D = dist(flowers[i], flowers[j]);
//				graph[i][j] = D;
//				graph[j][i] = D;
//				//std::cout << D << " DEBUG\n";
//			}
//		}
//	}
//	//std::cout << "DEBUG\n";
//	prim(0, F);
//	//std::cout << "DEBUG\n";
//	//for (const ld w : W) {
//	//	std::cout << w << " DEBUG\n";
//	//}
//	std::sort(W.begin(), W.end());
//	int I = F - B;
//	std::cout << W[I] << "\n";
//	return 0;
//}

/*
meadow.in
3 2
1 1
2 3
3 2
meadow.out
1.41
meadow.in
5 3
1 1
1 4
1 5
5 1
5 5
meadow.out
3.00
meadow.in
7 4
1 1
3 9
9 4
2 2
6 4
5 5
6 9
meadow.out
3.00
*/