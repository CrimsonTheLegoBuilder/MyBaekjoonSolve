import sys

N = int(sys.stdin.readline().strip())
ans = 0
y = 0
x = N-1
while x > y:
    while x**2 + y**2 < N**2:
        ans += 1
        print(x, y, ans)
        y += 1
    x -= 1
print(ans)
print(ans*8)
