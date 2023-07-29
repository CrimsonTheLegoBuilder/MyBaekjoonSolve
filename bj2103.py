import sys

n = int(sys.stdin.readline().strip())
dots = [tuple(map(int, sys.stdin.readline().strip().split())) for _ in range(n)]
ans = 0

dots.sort(key=lambda x: (x[0], x[1]))
for i in range(0, len(dots) - 1, 2):
    ans += abs(dots[i][1] - dots[i+1][1])

dots.sort(key=lambda x: (x[1], x[0]))
for i in range(0, len(dots) - 1, 2):
    ans += abs(dots[i][0] - dots[i+1][0])

print(ans)

