import sys

t = int(sys.stdin.readline().strip())
for _ in range(t):
    V, N = map(int, sys.stdin.readline().strip().split())
    trash = [list(map(int, sys.stdin.readline().strip().split())) for _ in range(N)]
    dist = 0
    start = 0

    while True:
        cur = 0
        tmp = 0
        for i in range(N):
            if trash[i][1] and cur + trash[i][1] <= V:
                cur += trash[i][1]
                trash[i][1] = 0
                tmp = trash[i][0]
                if cur == V:
                    dist += 2 * tmp
                    break
            elif trash[i][1] and cur + trash[i][1] > V:
                tmp = trash[i][0]
                dist += 2 * tmp
                break
            if i == N-1:
                dist += 2 * tmp
        if not cur:
            break
    print(dist)
