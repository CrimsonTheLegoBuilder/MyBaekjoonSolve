#include <iostream>
#include <unordered_set>
 
std::unordered_set<int> S;
int P[4][4]{};
int dfs(int r, int c, int depth, int t) {
    if (depth > 6) {
        S.insert(t);
        return 0;
    }
    if (0 <= r && r < 4 && 0 <= c && c < 4) {
        dfs(r+1, c, depth+1, t*10+P[r][c]);
        dfs(r, c+1, depth+1, t*10+P[r][c]);
        dfs(r-1, c, depth+1, t*10+P[r][c]);
        dfs(r, c-1, depth+1, t*10+P[r][c]);
    }
    return 0;
}
  
int main() {
    int N;
    std::cin >> N;
    for (int tc = 1; tc <= N; tc++) {
        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < 4; j++) {
                std::cin >> P[i][j];
            }
        }
        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < 4; j++) {
                dfs(i, j, 0, 0);
            }
        }
        std::cout << "#" << tc << " " << S.size() << "\n";
        S.clear();
    }
    return 0;
}