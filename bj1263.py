import sys

N = int(sys.stdin.readline().strip())
table = [tuple(map(int, sys.stdin.readline().strip().split())) for _ in range(N)]
table.sort(key=lambda x: x[1], reverse=True)

ans = table[0][1] - table[0][0]
for i in range(1, N):
    if ans < table[i][1]:
        ans -= table[i][0]
    else:
        ans = table[i][1] - table[i][0]

if ans < 0:
    print(-1)
else:
    print(ans)
