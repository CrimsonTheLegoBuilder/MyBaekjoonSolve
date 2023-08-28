import sys


def bt(x):
    global width
    if x == N:
        # print(*perm)
        r1 = balls[perm[0]]
        stack = [(r1, 0.0)]
        for j in perm[1:]:
            r = balls[j]
            r_ = stack[-1][0]
            d = 2 * (r * r_) ** .5 + stack[-1][1]
            while stack:
                r_ = stack[-1][0]
                if r_ > r:
                    break
                else:
                    stack.pop()
            stack.append((r, d))
        # print(r1, stack[-1])
        cur = r1 + stack[-1][0] + stack[-1][1]
        width = min(cur, width)
        return
    for i in range(N):
        if not visited[i]:
            perm[x] = i
            visited[i] = 1
            bt(x+1)
            visited[i] = 0


for _ in range(int(sys.stdin.readline().strip())):
    temp = list(map(float, sys.stdin.readline().strip().split()))
    N = int(temp[0])
    balls = temp[1:]
    # print(N)
    # print(balls)
    perm = [0] * N
    visited = [0] * N
    width = 12e12
    bt(0)
    print(str(width).lstrip('0'))
