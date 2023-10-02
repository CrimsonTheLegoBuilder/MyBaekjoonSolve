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
ans = 0
zeros = 0
for i in range(N):
    x, y = map(int, sys.stdin.readline().strip().split())
    m = gcd(abs(x), abs(y))
    if not x and not y:
        # print('zero!')
        ans += N - 1
        zeros += 1
        # print(N - i - 1)
        continue
    if not y:
        seg.setdefault((1, 0), 0)
        seg[(1, 0)] += 1
    if not x:
        seg.setdefault((0, 1), 0)
        seg[(0, 1)] += 1
    if x * y > 0:
        seg.setdefault((abs(x)//m, abs(y)//m), 0)
        seg[(abs(x)//m, abs(y)//m)] += 1
    elif x * y < 0:
        seg.setdefault((-abs(x)//m, abs(y)//m), 0)
        seg[(-abs(x)//m, abs(y)//m)] += 1
# print(seg)
for vx, vy in seg:
    if seg.get((-vy, vx)):
        ans += seg[(vx, vy)] * seg[(-vy, vx)]
print(ans - (zeros * (zeros - 1) // 2))
'''
10
0 0
1 2
3 5
3 6
2 -1
-4 2
4 -2
-5 0
-8 -16
0 3

3
0 0
1 1
1 -1

3
2
1 1
1 -1

2
1 1
0 0
'''