import sys

N = int(sys.stdin.readline().strip())
ans = 0
y = 0
x = N-1
t = int(N / 2**.5)
while x >= t:
    while x**2 + (y+1)**2 <= N**2:
        ans += 1
        print(x, y, ans)
        y += 1
    x -= 1
print(ans)
print(((ans-1)*8)+4)
