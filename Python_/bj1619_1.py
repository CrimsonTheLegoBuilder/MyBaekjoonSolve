import sys
from math import gcd

# def gcd(a, b):
#     if b == 0:
#         return a
#     if not a % b:
#         return b
#     c = a % b
#     return gcd(b, c)

n = int(sys.stdin.readline().strip())
arr = [list(map(int, sys.stdin.readline().strip().split())) for _ in range(n)]
result = 0

for i in arr:  # 모든 점에 대해 경우를 보겠다
    dic = {}
    for j in arr:  # 다른 겹치지 않는 모든 점을 살펴보겠다
        if i == j:
            continue
        else:
            X, Y = j[0] - i[0], j[1] - i[1]  # 다른 점들로 향하는 벡터
            gcd_ = gcd(abs(X), abs(Y))
            temp = (X // gcd_, Y // gcd_)  # 정규화

            if temp in dic:  # 딕셔너리 구성
                dic[temp] += 1
            else:
                dic[temp] = 1

    for (x, y) in dic.keys():  # 모든 점들에 대해 이 점이 최대값을 내는가? 선택할 것인가?
        if dic.get((-x, -y)):
            ans = dic.get((-x, -y)) + dic.get((x, y)) + 1
            result = max(result, ans)
        elif dic.get((x, y)) >= 2:
            ans = dic.get((x, y)) + 1
            result = max(result, ans)

if result == 0:
    print(-1)
else:
    print(result)
