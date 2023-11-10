#include <iostream>
#include <queue>
#include <cstring>
const int LEN = 500'001;
int N, K, V[2][LEN], B[LEN];

struct P {
    int i, d;
};
std::queue<P> Q;

int BFS(int v, int K) {
    memset(V, -1, sizeof V);
    memset(B, -1, sizeof B);
    //for (int i = 0; i < 2; i++) {
    //    for (int j = 0; j < LEN; j++) {
    //        V[i][j] = LEN + 1;
    //        B[j] = LEN + 1;
    //    }
    //}
    V[0][v] = 0;
    Q.push({ v, 0 });
    while (!Q.empty()) {
        P p = Q.front(); Q.pop();
        if (0 <= p.i - 1 && !~V[p.d + 1 & 1][p.i - 1]) {
            V[p.d + 1 & 1][p.i - 1] = V[p.d & 1][p.i] + 1;
            Q.push({ p.i - 1, p.d + 1 });
        }
        if (p.i + 1 < LEN && !~V[p.d + 1 & 1][p.i + 1]) {
            V[p.d + 1 & 1][p.i + 1] = V[p.d & 1][p.i] + 1;
            Q.push({ p.i + 1, p.d + 1 });
        }
        if ((p.i << 1) < LEN && !~V[p.d + 1 & 1][p.i << 1]) {
            V[p.d + 1 & 1][p.i << 1] = V[p.d & 1][p.i] + 1;
            Q.push({ p.i << 1, p.d + 1 });
        }
    }
    int sec = 0, step = 1;
    while (K < LEN) {
        B[K] = sec;
        //std::cout << B[K] << " " << K << " " << V[sec + 1 & 1][K] << "\n";
        if (B[K] >= V[sec++ & 1][K]) return B[K];
        K += step++;
    }
    return -1;
}


int main() {
    std::cin.tie(0)->sync_with_stdio(0);
    std::cout.tie(0);
    std::cin >> N >> K;
    std::cout << BFS(N, K);
    return 0;
}



//int BFS(int v, int K) {
//    for (int i = 0; i < 2; i++) {
//        for (int j = 0; j < LEN; j++) {
//            V[i][j] = LEN;
//            B[j] = -1;
//        }
//    }
//    V[0][v] = 0;
//    Q.push({ v, 0 });
//    while (!Q.empty()) {
//        P p = Q.front(); Q.pop();
//        if (0 <= p.i - 1 && p.d + 1 < V[p.d + 1 & 1][p.i - 1]) {
//            V[p.d + 1 & 1][p.i - 1] = V[p.d & 1][p.i] + 1;
//            Q.push({ p.i - 1, p.d + 1 });
//        }
//        if (p.i + 1 < LEN && p.d + 1 < V[p.d + 1 & 1][p.i + 1]) {
//            V[p.d + 1 & 1][p.i + 1] = V[p.d & 1][p.i] + 1;
//            Q.push({ p.i + 1, p.d + 1 });
//        }
//        if (p.i << 1 < LEN && p.d + 1 < V[p.d + 1 & 1][p.i << 1]) {
//            V[p.d + 1 & 1][p.i << 1] = V[p.d & 1][p.i] + 1;
//            Q.push({ p.i << 1, p.d + 1 });
//        }
//    }
//    int sec = 0, step = 1;
//    while (K < LEN) {
//        B[K] = sec;
//        //std::cout << B[K] << " " << K << " " << V[sec + 1 & 1][K] << "\n";
//        if (B[K] >= V[sec++ & 1][K]) return B[K];
//        K += step++;
//    }
//    return -1;
//}