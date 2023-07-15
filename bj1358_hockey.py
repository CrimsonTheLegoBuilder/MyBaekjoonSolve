import sys

W, H, X, Y, P = map(int, sys.stdin.readline().strip().split())
R = H // 2
dot1 = [X, Y + R]
dot2 = [X + W, Y + R]
cnt = 0

for _ in range(P):
    x1, y1 = map(int, sys.stdin.readline().strip().split())
    r1 = ((x1 - dot1[0]) ** 2 + (y1 - dot1[1]) ** 2) ** 0.5
    r2 = ((x1 - dot2[0]) ** 2 + (y1 - dot2[1]) ** 2) ** 0.5
    if (r1 <= R) | (r2 <= R) | ((X <= x1 <= X+W) & (Y <= y1 <= Y+H)):
        cnt += 1

print(cnt)
