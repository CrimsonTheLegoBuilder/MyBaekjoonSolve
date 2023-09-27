import sys


def rhombus(x, y):
    global N, M, ans
    if x == y:
        # print(x, y)
        ans += (N - y + 1) * (M - x + 1) * (x + (x - 1)//2 * 2)
        # print((N - y + 1) * (M - x + 1) * x)
    # elif x > y:
    #     for i in range(1, x):
    #         if y**2 + i**2 == (x - i)**2:
    #             # print(x, y)
    #             ans += (N - y + 1) * (M - x + 1) * 2
    #             # print((N - y + 1) * (M - x + 1))
    # elif x < y:
    #     for j in range(1, y):
    #         if x**2 + j**2 == (y - j)**2:
    #             ans += (N - y + 1) * (M - x + 1) * 2
    if x != y:
        for i in range(1, x):
            for j in range(1, y):
                if i**2 + j**2 == (x-i)**2 + (y-j)**2:
                    ans += (N - y + 1) * (M - x + 1)

    # if x != y and x % 2 == 0 and y % 2 == 0:
    #     # print(x, y)
    #     ans += (N - y + 1) * (M - x + 1)
    #     # print((N - y + 1) * (M - x + 1))
    return 0


N, M = map(int, sys.stdin.readline().strip().split())
ans = 0

for X in range(1, M + 1):
    for Y in range(1, N + 1):
        rhombus(X, Y)

print(ans)
