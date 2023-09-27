import sys

limit = 256
N, M, B = map(int, sys.stdin.readline().strip().split())
ground = [list(map(int, sys.stdin.readline().strip().split())) for _ in range(N)]

low = 257
total = 0
for i in range(N):
    for j in range(M):
        total += ground[i][j]
        if low > ground[i][j]:
            low = ground[i][j]
high = (total + B) // (N*M)
if high > limit:
    high = limit
# print(low, high)

ans = []
for height in range(low, high+1):
    minus = 0
    plus = 0
    time = 0
    for i in range(N):
        for j in range(M):
            if height < ground[i][j]:
                minus += ground[i][j] - height
            if height > ground[i][j]:
                plus += height - ground[i][j]
    # print(plus, minus)
    time += minus * 2 + plus
    # print(time, height)
    ans.append((time, height))
# print(ans)
ans.sort(key=lambda x: (x[0], -x[1]))
print(*ans[0])
