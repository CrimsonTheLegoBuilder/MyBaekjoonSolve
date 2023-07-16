import sys
# from math import gcd


def gcd(a, b):
    if b == 0:
        return a
    if not a % b:
        return b
    c = a % b
    return gcd(b, c)


n = int(sys.stdin.readline().strip())
arr = [list(map(int, sys.stdin.readline().strip().split())) for _ in range(n)]
result = 0

for i in arr:
    dic = {}
    for j in arr:
        if i == j:
            continue
        else:
            X, Y = j[0] - i[0], j[1] - i[1]
            gcd_ = gcd(abs(X), abs(Y))
            if X * Y < 0:
                temp = (-(abs(X) // gcd_), abs(Y) // gcd_)
            else:
                temp = (abs(X) // gcd_, abs(Y) // gcd_)

            if temp in dic:
                dic[temp] += 1
            else:
                dic[temp] = 1

    for (x, y) in dic.keys():
        if dic.get((-y, x)):
            result += dic[(x, y)] * dic[(-y, x)]

print(result)
