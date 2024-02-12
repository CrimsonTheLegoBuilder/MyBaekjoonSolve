import sys
INPUT = sys.stdin.readline

if __name__ == '__main__':  # boj7579
    LEN = 10001
    N, M = map(int, INPUT().split())
    app = [list(map(int, INPUT().split())) for _ in range(2)]
    dp = [0] * LEN
    for i in range(N):
        m, c = app[0][i], app[1][i]
        for j in range(LEN - 1, c-1, -1):  # j = current cost
            dp[j] = max(dp[j-c] + m, dp[j])
    for i in range(LEN):
        if dp[i] >= M:
            # print(i)
            break
