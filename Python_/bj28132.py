import sys
from math import gcd

# N = int(sys.stdin.readline().strip())
# seg = {}
# for _ in range(N):
#     x, y = map(int, sys.stdin.readline().strip().split())
#     m = gcd(abs(x), abs(y))
#     seg.setdefault((x//m, y//m), 0)
#     seg[(x//m, y//m)] += 1
# print(seg)
# ans = 0
# for vx, vy in seg:
#     if seg.get((vy, -vx)):
#         ans += seg[(vx, vy)] * seg[(vy, -vx)]
#     if seg.get((-vy, vx)):
#         ans += seg[(vx, vy)] * seg[(-vy, vx)]
# print(ans//2)

N = int(sys.stdin.readline().strip())
seg = {}
for _ in range(N):
    x, y = map(int, sys.stdin.readline().strip().split())
    m = gcd(abs(x), abs(y))
    if x * y >= 0:
        seg.setdefault((abs(x)//m, abs(y)//m), 0)
        seg[(abs(x)//m, abs(y)//m)] += 1
    else:
        seg.setdefault((-abs(x)//m, abs(y)//m), 0)
        seg[(-abs(x) // m, abs(y)//m)] += 1
# print(seg)
ans = 0
for vx, vy in seg:
    if seg.get((-vy, vx)):
        ans += seg[(vx, vy)] * seg[(-vy, vx)]
print(ans)
