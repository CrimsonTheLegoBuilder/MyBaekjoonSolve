import sys
INPUT = sys.stdin.readline
INF: int = int(1e9)


def solve():
    N, M = map(int, INPUT().strip().split())
    min_dist: list[list[int]] = [list(INF for _ in range(N)) for _ in range(N)]
    max_dist: list[list[int]] = [list(-1 for _ in range(N)) for _ in range(N)]

    for i in range(N):
        min_dist[i][i] = 0

    for _ in range(M):
        s, e, l = map(int, INPUT().strip().split())
        s -= 1
        e -= 1
        l <<= 1
        if min_dist[s][e] > l:
            min_dist[s][e] = min_dist[e][s] = l
        if max_dist[s][e] < l:
            max_dist[s][e] = max_dist[e][s] = l

    # for k in range(N):
    #     for i in range(N):
    #         for j in range(N):
    #             min_dist[i][j] = min(min_dist[i][j], min_dist[i][k] + min_dist[k][j])

    for k in range(N):
        for i in range(N):
            for j in range(i + 1, N):
                min_dist[i][j] = min_dist[j][i] = min(min_dist[i][j], min_dist[i][k] + min_dist[k][j])

    ret: int = INF
    for s in range(N):
        tmp: int = 0
        for i in range(N):
            for j in range(N):
            # for j in range(i, N):
                if max_dist[i][j] == -1:
                    continue
                remain: int = max_dist[i][j]
                diff: int = min_dist[s][j] - min_dist[s][i]
                if diff >= remain:
                    continue
                tmp = max(tmp, ((remain - diff) >> 1) + min_dist[s][j])
        ret = min(ret, tmp)
    print(f'{ret >> 1}.{5 if ret & 1 else 0}')


if __name__ == '__main__':
    solve()  # boj13141 ignition
