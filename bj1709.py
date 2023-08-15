import sys

N = int(sys.stdin.readline().strip())
ans = 0
y = 0
x = N
while x >= 0:
    while x**2 + y**2 < N**2:
        ans += 1
        y += 1
    x -= 1
print(ans)
